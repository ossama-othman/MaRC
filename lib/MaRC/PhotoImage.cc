// $Id: PhotoImage.cc,v 1.15 2005/11/14 08:53:44 othman Exp $

#include "PhotoImage.h"
#include "PhotoImageParameters.h"
#include "Constants.h"
#include "OblateSpheroid.h"
#include "misc.h"
#include "NullGeometricCorrection.h"
#include "NullPhotometricCorrection.h"
#include "NullInterpolationStrategy.h"
#include "PhotoInterpolationStrategy.h"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <numeric>
#include <iostream>
#include <sstream>

MaRC::PhotoImage::PhotoImage (PhotoImageParameters & params)
  : SourceImage (),
    filename_ (params.filename_),
    body_ (params.body_),
    image_ (params.image_),
    samples_ (params.samples_),
    lines_ (params.lines_),
    geometric_correction_ (params.geometric_correction_),
    photometric_correction_ (params.photometric_correction_),
    interpolation_strategy_ (params.interpolation_strategy_),
    sub_observ_lat_ (params.sub_observ_lat_),
    sub_observ_lon_ (params.sub_observ_lon_),
    sub_solar_lat_  (params.sub_solar_lat_),
    sub_solar_lon_  (params.sub_solar_lon_),
    range_          (params.range_),
    position_angle_ (params.position_angle_),
    km_per_pixel_   (params.get_km_per_pixel() ),
    focal_length_   (params.focal_length_),
    focal_length_pixels_ (params.get_focal_length_pixels() ),
    scale_         (params.scale_),
    OA_s_          (params.OA_s_),
    OA_l_          (params.OA_l_),
    sky_mask_      (params.get_sky_mask() ),
    image_unread_mask_ (params.get_image_unread_mask() ),
    range_b_       (params.get_range_b() ),
    observ2body_   (params.get_observ2body() ),
    body2observ_   (params.get_body2observ() ),
    nibble_left_   (params.nibble_left_),
    nibble_right_  (params.nibble_right_),
    nibble_top_    (params.nibble_top_),
    nibble_bottom_ (params.nibble_bottom_),
    sample_center_ (params.sample_center_),
    line_center_   (params.line_center_),
    lat_at_center_ (params.lat_at_center_),
    lon_at_center_ (params.lon_at_center_),
    mu_limit_ (params.mu_limit_),
    flags_ (params.flags_)
{
}

MaRC::PhotoImage::~PhotoImage (void)
{
}

// Supersampling verification routine.
// Currently just messages to stdout if there's a problem.
void
MaRC::PhotoImage::check_image_unread_mask () const
{
  if (!flags::check(this->flags_, SS_VERIFY)) {
    // Under these circumstances, the data wasn't gathered;
    // bail out or risk a segfault.
    return;
  }
  // image_unread_mask was initialized to sky_mask;
  // meaning that the sky was all at 0, and the body all at 1.
  // Then, any body pixel which was read was turned to a 0.
  // So if there are unread pixels left, there is a problem!

  const bool* const mask = this->image_unread_mask_.get();
  const bool* const end = mask + this->samples_ * this->lines_;

  for (const bool* i = mask; i < end; i++) {
    if (*i) {
      std::cerr << "ERROR: Failure to supersample in " << this->filename_
                << std::endl
                << "starting at pixel " << (i - mask) << std::endl;
      return;
    }
  }
}

bool
MaRC::PhotoImage::operator== (PhotoImage const & img)
{
  return (this->samples_ == img.samples_
          && this->lines_ == img.lines_
          && this->flags_ == img.flags_
          && this->km_per_pixel_ == img.km_per_pixel_
          && this->nibble_left_   == img.nibble_left_
          && this->nibble_right_  == img.nibble_right_
          && this->nibble_top_    == img.nibble_top_
          && this->nibble_bottom_ == img.nibble_bottom_
          && this->sample_center_ == img.sample_center_
          && this->line_center_ == img.line_center_
          && this->sub_observ_lat_ == img.sub_observ_lat_
          && this->sub_observ_lon_ == img.sub_observ_lon_
          && this->sub_solar_lat_  == img.sub_solar_lat_
          && this->sub_solar_lon_  == img.sub_solar_lon_
          && this->range_ == img.range_
          && this->position_angle_ == img.position_angle_) ? true : false;

  //  return 1; // DEBUGGING PURPOSES ONLY!!!!
}

bool
MaRC::PhotoImage::is_visible (double const & lat,
                              double const & lon) const
{
  /* mu is the cosine of the angle between
   * -- the vector from the given point to the observer
   * -- the normal vector to the surface at the given point
   * For a convex body, if this is positive, the point is
   * on the visible side of the planet, and if it's negative,
   * the point is on the far side of the planet.
   */
  if (this->body_.mu (this->sub_observ_lat_,
                      this->sub_observ_lon_,
                      lat,
                      lon,
                      this->range_) < 0

      /* mu0 is the angle between
       * -- the vector from the given point to the sun
       * -- the normal vector to the surface at the given point
       * The sun is assumed to be an infinite distance away.
       * For a convex body, if this is positive, the point is
       * on the lit side of the planet, and if it's negative,
       * the point is on the dark side of the planet.
       */
      || (flags::check (this->flags_, USE_TERMINATOR)
          && this->body_.mu0 (this->sub_solar_lat_,
                              this->sub_solar_lon_,
                              lat,
                              lon) < 0))
    {
      return false;
    }

  // Passed both the far-side and (if requested) the dark-side check.
  return true;
}

bool
MaRC::PhotoImage::read_data (const double & lat,
                             const double & lon,
                             double & data)
{
  unsigned int weight = 1;  // Unused.

  static const bool scan = false; // Do not scan for data weight.

  return this->read_data (lat, lon, data, weight, scan);
}

bool
MaRC::PhotoImage::read_data (const double & lat,
                             const double & lon,
                             double & data,
                             unsigned int & weight,
                             bool scan)
{
  if (!this->is_visible (lat, lon))
    return false;  // Failure

  // Do not plot data close to limb
  if (flags::check (this->flags_, EMI_ANG_LIMIT)
      && this->body_.mu (this->sub_observ_lat_,
                          this->sub_observ_lon_,
                          lat,
                          lon,
                          this->range_) < this->mu_limit_)
    {
      return false;  // Outside the configured emission angle limit.
    }

  double x = 0, z = 0;
  this->latlon2pix (lat, lon, x, z);

  if (x < 0 || z < 0)
    return false;  // Prevent integer underflow since we're casting to
                   // an unsigned integer below.

  // x and z are 'pixel coordinates'.  In 'pixel coordinates',
  // 0..1 is inside pixel 0, 1..2 is inside pixel 1, etc.
  const unsigned int i = static_cast<unsigned int> (::floor (x));
  const unsigned int k = static_cast<unsigned int> (::floor (z));

  // OK, we decided to get the pixel (i,k).  For verification
  // that we're oversampling, we register in a bitmap that we read
  // that pixel.  (For this sort of verification, we don't care if
  // we decide to skip the pixel for some later reason.)
  if (flags::check(this->flags_, SS_VERIFY)) {
    if (i < this->samples_ && k < this->lines_) {
      this->image_unread_mask_[k * this->samples_ + i] = false;
    }
  }

  // e.g., if (i < 0 || i >= samples_ || k < 0 || k >= lines_)
  // The following assumes that line numbers increase downward.
  // CHECK ME!
  if (i < this->nibble_left_   || i >= this->samples_ - this->nibble_right_
      || k < this->nibble_top_ || k >= this->lines_ - this->nibble_bottom_
      || (flags::check(this->flags_, REMOVE_SKY)
          && !this->sky_mask_(i, k)))
    return false;

  data = this->image_(i, k);

  if (!this->interpolation_strategy_->interpolate (this->image_.get (),
                                                   x,
                                                   z,
                                                   data)
      || this->photometric_correction_->correct (this->body_,
                                                 this->sub_observ_lat_,
                                                 this->sub_observ_lon_,
                                                 this->sub_solar_lat_,
                                                 this->sub_solar_lon_,
                                                 lat,
                                                 lon,
                                                 this->range_,
                                                 data) != 0
      || isnan (data))
    {
      return false;
    }

  // Scan across image for "off-planet/image" pixels and compute
  // averaging weights.
  //
  // Note that a weight is computed regardless of whether or not sky
  // removal is enabled.
  if (scan)
    {
      unsigned int & shortest_distance = weight;

      // Give less weight to pixels close to an edge of the image.
      //
      // No need to include nibble values in this calculation since
      // we're guaranteed to be within the non-nibbled image area due
      // to the nibble value check earlier in this method.
      //
      // For most purposes, this quickly computed weight should be
      // sufficient.  If the image has gaps, determining weights
      // through the below sky mask scanning code below may be a
      // better choice in terms of quality.
      shortest_distance =
        std::min (i,
                  std::min (this->samples_ - i,
                            std::min (k,
                                      this->lines_ - k)));

      // Scan across image for "off-body/image" pixels.
      if (flags::check(this->flags_, REMOVE_SKY))
        {
          bool const * const sky_mask = this->sky_mask_.get ();

          unsigned int const offset = k * this->samples_;

          // Search from i to nibble_right.
          bool const * begin = sky_mask + (offset + i);
          bool const * end   = sky_mask +
            (offset + (this->samples_ - this->nibble_right_));

          bool const * result = std::find (begin, end, true);

          shortest_distance =
            std::min (static_cast<unsigned int> (result - begin),
                      shortest_distance);

          // Search from nibble_left to i.
          begin = sky_mask + (offset + this->nibble_left_);
          end   = sky_mask + (i + 1);

          result = std::find (begin, end, true);

          shortest_distance =
            std::min (static_cast<unsigned int> (result - begin),
                      shortest_distance);

          // Search from k to nibble_bottom.
          const unsigned int kend = this->lines_ - this->nibble_bottom_;

          unsigned int kk;
          for (kk = k;
               kk < kend && sky_mask[kk * this->samples_ + i];
               ++kk)
            ; // Nothing

          shortest_distance = std::min (kk - k, shortest_distance);

          // Search from nibble_top to k.
          for (kk = k;
               kk >= this->nibble_top_
                 && sky_mask[kk * this->samples_ + i];
               --kk)
            ; // Nothing

          shortest_distance = std::min (k - kk, shortest_distance);
        }
    }

  return true;  // Success
}

void
MaRC::PhotoImage::latlon2pix (const double & lat,
                              const double & lon,
                              double & x,
                              double & z) const
{
  const double radius = this->body_.centric_radius (lat);

  double longitude;

  if (this->body_.prograde ())
    longitude = this->sub_observ_lon_ - lon;
  else
    longitude = lon - this->sub_observ_lon_;

  DVector Coord;
  Coord[0] =  radius * ::cos (lat) * ::sin (longitude);
  Coord[1] = -radius * ::cos (lat) * ::cos (longitude);
  Coord[2] =  radius * ::sin (lat);

  const DVector Obs (Coord - this->range_b_);

  // Convert to observer coordinates.
  const DVector Rotated (this->body2observ_ * Obs);

  x = Rotated[0] / Rotated[1] * this->focal_length_pixels_;
  z = Rotated[2] / Rotated[1] * this->focal_length_pixels_;

  // Convert from object space to image space.
  this->geometric_correction_->object_to_image (x, z);

  x += this->OA_s_;
  z = this->OA_l_ - z; // Assumes line numbers increase top to bottom.

  //std::cout << "* x = " << x << "\tz = " << z << std::endl;

}

