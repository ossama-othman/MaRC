#include "PhotoImageParameters.h"
#include "PhotoImage.h"
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

#ifndef MARC_DEFAULT_GEOM_CORR_STRATEGY
# define MARC_DEFAULT_GEOM_CORR_STRATEGY MaRC::NullGeometricCorrection
#endif  /* MARC_DEFAULT_GEOM_CORR_STRATEGY */

#ifndef MARC_DEFAULT_PHOTO_CORR_STRATEGY
# define MARC_DEFAULT_PHOTO_CORR_STRATEGY MaRC::NullPhotometricCorrection
#endif  /* MARC_DEFAULT_PHOTO_CORR_STRATEGY */

#ifndef MARC_DEFAULT_INTERPOLATION_STRATEGY
# define MARC_DEFAULT_INTERPOLATION_STRATEGY MaRC::NullInterpolationStrategy
#endif  /* MARC_DEFAULT_INTERPOLATION_STRATEGY */


MaRC::PhotoImageParameters::PhotoImageParameters
  (OblateSpheroid const & body,
   double * image,
   unsigned int samples,
   unsigned int lines,
   GeometricCorrection * gc)
  : finalized (false),
    filename_ (),
    body_ (body),
    image_ (image, samples, lines),
    samples_ (samples),
    lines_ (lines),
    geometric_correction_ (gc == 0 ? new MARC_DEFAULT_GEOM_CORR_STRATEGY : gc),
    photometric_correction_ (new MARC_DEFAULT_PHOTO_CORR_STRATEGY),
    interpolation_strategy_ (new MARC_DEFAULT_INTERPOLATION_STRATEGY),
    sub_observ_lat_ (0),
    sub_observ_lon_ (0),
    sub_solar_lat_  (0),
    sub_solar_lon_  (0),
    range_          (::sqrt (std::numeric_limits<double>::max () - 1)),
    position_angle_     (0),
    arcsec_per_pixel_ (-1),      // Force "bad" value until set by caller
    km_per_pixel_   (-1),        // Force "bad" value until set by caller
    focal_length_   (-1),        // Force "bad" value until set by caller
    focal_length_pixels_ (-1),   // Force "bad" value until fully initialized
    scale_ (-1),                 // Force "bad" value until set by caller
    OA_s_          (samples / 2.0), // Default, overridden by method call
    OA_l_          (lines / 2.0), // Default, overridden by method call
    sky_mask_      (0,0,0), // Default disable sky removal.
    range_b_       (),
    observ2body_   (),
    body2observ_   (),
    nibble_left_   (0),
    nibble_right_  (0),
    nibble_top_    (0),
    nibble_bottom_ (0),
    sample_center_ (0),
    line_center_   (0),
    lat_at_center_ (0),
    lon_at_center_ (0),
    mu_limit_ (::cos (90 * C::degree)), // cos() of 90 deg emission angle limit
    flags_ (0)
{
  if (image == 0)
    {
      throw std::invalid_argument ("PhotoImageParameters: NULL source image array.");
    }

  if (samples < 2 || lines < 2)  // Why would there ever be a one
                                 // pixel source image?
    {
      std::ostringstream s;
      s << "Source image samples (" << samples
        << ") and lines (" << lines << ") must both be greater than one.";

      throw std::invalid_argument (s.str ());
    }
}

MaRC::PhotoImageParameters::~PhotoImageParameters (void)
{
}

/*
 * These are run strictly at the end to finalize the parameters.
 */

/* Throw if we don't have a valid set of values. */
void
MaRC::PhotoImageParameters::verify_values (void) const
{
  // Run some sanity checks on nibbling values
  if (this->samples_ - this->nibble_right_ < this->nibble_left_)
    {
      // Impossible situation points to bug in PhotoImageParameters
      throw std::domain_error("Either the left or right nibble value "
                              "(or both) is too large.");
    }
  if (this->lines_ - this->nibble_top_ < this->nibble_bottom_)
    {
      // Impossible situation points to bug in PhotoImageParameters
      throw std::domain_error("Either the top or bottom nibble value "
                              "(or both) is too large.");
    }
}

void
MaRC::PhotoImageParameters::set_km_per_pixel (void)
{
  if (this->focal_length_ > 0 && this->scale_ > 0) // User has set these values
    {
      this->focal_length_pixels_ =
        this->focal_length_ * this->scale_;

      this->km_per_pixel_ =
        this->range_ /
        ::sqrt (this->focal_length_pixels_
                * this->focal_length_pixels_
                + (this->OA_s_ - this->sample_center_)
                * (this->OA_s_ - this->sample_center_)
                + (this->OA_l_ - this->line_center_)
                * (this->OA_l_ - this->line_center_));

//       std::cout << "focal_length = " << this->focal_length_ << std::endl
//                 << "scale        = " << this->scale_        << std::endl
//                 << "OA_s         = " << this->OA_s_         << std::endl
//                 << "OA_l         = " << this->OA_l_         << std::endl
//                 << "sample_center= " << this->sample_center_<< std::endl
//                 << "line_center  = " << this->line_center_  << std::endl
//                 << "km_per_pixel = " << this->km_per_pixel_ << std::endl
//                 << "range        = " << this->range_        << std::endl;
    }
  else if (this->km_per_pixel_ <= 0)
    {
      std::ostringstream s;
      s << "Attempt to compute number of kilometers per pixel"
        << "where no focal length and scale have been set. ";
      throw std::invalid_argument (s.str ());
    }

  // If km_per_pixel_ > 0 than, presumably, km_per_pixel_ has already been set;
  // so do nothing.
}



void
MaRC::PhotoImageParameters::finalize_setup (void)
{
  this->verify_values();

  //////////////////////////////////////
  // All necessary image values and attributes should be set by now!
  //////////////////////////////////////
  // FIXME: We should actually check this and throw errors on failure

  if (this->km_per_pixel_ <= 0)
    this->set_km_per_pixel ();

  // Set Body center to observer vectors
  this->range_b_[0] =  0;
  this->range_b_[1] = -this->range_ * ::cos (this->sub_observ_lat_);
  this->range_b_[2] =  this->range_ * ::sin (this->sub_observ_lat_);

  // Perpendicular distance from observer to image plane.
  double normal_range = 0;

  if (!flags::check (this->flags_, LATLON_AT_CENTER))
    {
      DVector range_O;  // range_ vector in observer coordinates

      range_O[0] = ( this->OA_s_ - this->sample_center_ ) * this->km_per_pixel_;

      range_O[1] =  0;
      // range_O[1] = (-this->focal_length_ * this->scale_ ) * km_per_pixel_; <---- set later

      // range_O[2] =  ( OA_l_ - line_center_   ) * km_per_pixel_;
      range_O[2] = (this->line_center_ - this->OA_l_   ) * this->km_per_pixel_;
      // Since line numbers increase top to bottom (e.g. VICAR images)

      const double magRo = Geometry::Magnitude (range_O);

      normal_range =
        ::sqrt (this->range_ * this->range_ - magRo * magRo);

      // In case focal length and scale are not set or used.
      range_O[1] = -normal_range;

      this->rot_matrices (range_O, this->observ2body_, this->body2observ_);
    }
  else
    {
      double Longitude;

      if (this->body_.prograde ())
        Longitude = this->sub_observ_lon_ - this->lon_at_center_;
      else
        Longitude = this->lon_at_center_ - this->sub_observ_lon_;

      const double radius =
        this->body_.centric_radius (this->lat_at_center_);

      DVector r0;
      r0[0] =  radius * ::cos (this->lat_at_center_) * ::sin (Longitude);
      r0[1] = -radius * ::cos (this->lat_at_center_) * ::cos (Longitude);
      r0[2] =  radius * ::sin (this->lat_at_center_);

      DVector const OA_prime (r0 - this->range_b_);

      DVector OA_hat (OA_prime);
      Geometry::toUnitVector (OA_hat);

      // Dot product.
      double const dp =
        std::inner_product (r0.begin (), r0.end (), OA_hat.begin (), 0.0);

      DVector r_OA;

      for (unsigned int i = 0; i < 3; ++i)
        r_OA[i] = dp * OA_hat[i];

      // Optical axis in body coordinates
      DVector const OpticalAxis (OA_prime - r_OA);

      this->rot_matrices (this->range_b_,
                          OpticalAxis,
                          this->observ2body_,
                          this->body2observ_);

      // In case focal length and scale are not set or used.
      normal_range = -(this->body2observ_ * this->range_b_)[1];
    }

  if (this->focal_length_pixels_ < 0)
    {
      this->focal_length_pixels_ =
        normal_range / this->km_per_pixel_;
    }

  // Scan across and determine where points lie off of the body, i.e.
  // remove the sky from the image.  The image will not actually be
  // modified.
  this->remove_sky (); // Remove sky from source image
  this->finalized = true;
}

// Get rotation matrices for case when body centers were given
int
MaRC::PhotoImageParameters::rot_matrices (const DVector & range_o,
                                DMatrix & observ2body,
                                DMatrix & body2observ)
{
  DVector r_o_unit (range_o);
  Geometry::toUnitVector (r_o_unit);

  // North angle rotation takes us to coordinate system 2

  DVector r_2_unit;
  Geometry::RotY (-this->position_angle_, r_o_unit, r_2_unit);
  const DVector temp2 (r_2_unit);
  DVector r_o (r_2_unit);

  // Rotation around X to bring projected north pole up to north pole
  // takes us into coordinate system 4 (ignoring the translation)

  // Rotation around Z (north pole) lines up y axis with sub-observer
  // longitude to reach body coordinate system.

  // Find second and third matrices: the easy way
  // Math by Nathanael Nerode: see paper FIXME FIXME FIXME

  // First find the final rotation around Z
  DVector r_b_unit (this->range_b_);
  Geometry::toUnitVector (r_b_unit);
  double sin_beta = r_2_unit[0] / ::fabs(r_b_unit[1]);
  double beta = ::asin(sin_beta);

  // And use that to find the rotation around X
  double sin_alpha =   (   r_2_unit[2] * r_b_unit[1] * ::cos(beta)
                         - r_2_unit[1] * r_b_unit[2] )
                     / (   r_2_unit[1] * r_2_unit[1]
                         + r_2_unit[2] * r_2_unit[2] );

  double alpha = ::asin(sin_alpha);

  // Observer to body transformation
  observ2body = Geometry::RotZMatrix (beta)
                * (Geometry::RotXMatrix (alpha)
                   * Geometry::RotYMatrix (-this->position_angle_) );


  // Verify that we did a good job.
  // FIXME: This is essentially unnecessary now.
  const double diff_magnitude =
    Geometry::Magnitude (this->range_b_ - observ2body * range_o);

  const double percent_diff =
    diff_magnitude / Geometry::Magnitude (this->range_b_);

  static const double tolerance = 1e-8;
  if (percent_diff * 100 > tolerance)
    // If greater than tolerance, warn.
    std::cerr << std::endl
              << "WARNING: Results may be incorrect since a" << std::endl
              << "         \"suitable\" transformation matrix was" << std::endl
              << "         not found for the given image." << std::endl
              << ".        There was a "
              << percent_diff * 100 << "%"<< std::endl
              << "         difference between the two test vectors."
              << std::endl
              << "         This warning occured since the percent" << std::endl
              << "         difference between the vectors was" << std::endl
              << "         greater than "
              << tolerance << "%." << std::endl;

  // Body to observer transformation
  // get reverse transformation by taking transpose since transformation
  // matrices are orthogonal
  body2observ = MaRC::transpose (observ2body);

#ifdef DEBUG
  for (unsigned int i = 0; i < 3; ++i)
    std::cout
      << ::fabs ((this->range_b_[i] - (observ2body * range_o)[i]) /
                 (this->range_b_[i] + (observ2body * range_o)[i]) * 2)
      << std::endl;

  std::cout
    << "range_b_ = " << this->range_b_ << std::endl
    << "range_b_ from transformation = " << observ2body * range_o
    << std::endl
    << "range_o = " << range_o << std::endl
    << "range_o from transformation = " << body2observ * this->range_b_
    << std::endl;

  std::cout << "position_angle_ = "
            << this->position_angle_ / C::degree
            << " degrees (positive is CCW)" << std::endl
            << "SubLatMod = " << SubLatMod / C::degree << std::endl
            << "Ztwist = " << Ztwist / C::degree << std::endl;

  DVector test;
  test[2] = 1; // Unit vector along z-axis
  DVector result (body2observ * test); // test vector in camera coordinates.

  std::cout << "Computed NORAZ = "
            << ::atan2 (-result[0], result[2]) / C::degree
            << " degrees (positive is CCW)" << std::endl
            << "Computed North pole vector in camera space = " << result
            << std::endl;
  std::cout << "observ2body = " << observ2body << std::endl
            << "body2observ = " << body2observ << std::endl;
#endif

  return 0;
}

// Get rotation matrices for case when lat/lon at optical axis were given
void
MaRC::PhotoImageParameters::rot_matrices (const DVector & range_b,
                                const DVector & OA,
                                DMatrix & observ2body,
                                DMatrix & body2observ)
{
  double SubLatMod[2];

  DVector NPole, rotated;
  DVector OA_O;

  // Unit vector representing North pole in body coordinates
  NPole[0] = 0;
  NPole[1] = 0;
  NPole[2] = 1;

  // OA_O is the optical axis vector in observer coordinates
  OA_O[0] = 0;
  OA_O[1] = Geometry::Magnitude (OA); // Geometry::Magnitude (OpticalAxis);
  OA_O[2] = 0;

  DVector UnitOpticalAxis (OA); // Optical axis in body coordinates
  Geometry::toUnitVector (UnitOpticalAxis);

  // Dot product.
  double const dotProd =
    std::inner_product (NPole.begin (),
                        NPole.end (),
                        UnitOpticalAxis.begin (),
                        0.0);

  SubLatMod[0] = ::asin (-dotProd);  // Angle between equatorial plane and OA

  DVector R_b (range_b);
  Geometry::toUnitVector (R_b);

  // Try first possibility
  SubLatMod[0] = ::asin (-dotProd);  // Angle between equatorial plane and OA
  Geometry::RotX (-SubLatMod[0], range_b, rotated);
  R_b = rotated;

  const double Ztwist1 = ::atan2 (R_b[0], -R_b[1]);

  // Observer to body transformation
  DMatrix o2b (Geometry::RotZMatrix (Ztwist1)
               * (Geometry::RotXMatrix (SubLatMod[0]) *
                  Geometry::RotYMatrix (-this->position_angle_)));

  observ2body = o2b;

  double diff_magnitude =
    Geometry::Magnitude (OA_O - o2b * UnitOpticalAxis);

  // Try second possibility
  SubLatMod[1] = C::pi - SubLatMod[0];
  Geometry::RotX (-SubLatMod[1], range_b, rotated);
  R_b = rotated;

  const double Ztwist2 = ::atan2 (R_b[0], -R_b[1]);

  // Observer to body transformation
  o2b =
    Geometry::RotZMatrix (Ztwist2)
    * (Geometry::RotXMatrix (SubLatMod[1]) *
       Geometry::RotYMatrix (-this->position_angle_));

#ifdef DEBUG
  double Ztwist, SubLatModified;
#endif  /* DEBUG */

  if (diff_magnitude > Geometry::Magnitude (OA_O -
                                            o2b * UnitOpticalAxis))
    {
      diff_magnitude =
        Geometry::Magnitude (OA_O - o2b * UnitOpticalAxis);
      observ2body = o2b;

#ifdef DEBUG
      SubLatModified = SubLatMod[1];
      Ztwist = Ztwist2;
    }
  else
    {
      SubLatModified = SubLatMod[0];
      Ztwist = Ztwist1;
#endif  /* DEBUG */

    }

  const double percent_diff =
    diff_magnitude / Geometry::Magnitude (UnitOpticalAxis);

  const double tolerance = 1e-8;

  if (percent_diff * 100 > tolerance)
    // If greater than tolerance, warn.
    std::cerr
      << std::endl
      << "WARNING: Results may be incorrect since a" << std::endl
      << "         \"suitable\" transformation matrix was" << std::endl
      << "         not found for the given image." << std::endl
      << "         There was a "
      << percent_diff * 100 << "%"<< std::endl
      << "         difference between the two test vectors." << std::endl
      << "         This warning occured since the percent" << std::endl
      << "         difference between the vectors was" << std::endl
      << "         greater than "
      << tolerance << "%." << std::endl;

  // Body to observer transformation
  // get reverse transformation by taking transpose since transformation
  // matrices are orthogonal
  body2observ = MaRC::transpose (observ2body);

#ifdef DEBUG
  for (unsigned int i = 0; i < 3; ++i)
    std::cout
      << ::fabs ((UnitOpticalAxis[i] - (observ2body * OA_O)[i]) /
                 (UnitOpticalAxis[i] + (observ2body * OA_O)[i]) * 2)
      << std::endl;

  std::cout
    << "UnitOpticalAxis = " << UnitOpticalAxis << std::endl
    << "OpticalAxis from transformation = "
    << observ2body * OA_O << std::endl
    << "OA_O = " << OA_O << std::endl
    << "OA_O from transformation = " << body2observ * UnitOpticalAxis
    << std::endl;
//   output_ << "position_angle_ = " << position_angle_ / C::degree << " degrees (positive is CCW)" << std::endl
//       << "SubLatModified = " << SubLatModified / C::degree << std::endl
//       << "Ztwist = " << Ztwist / C::degree << std::endl;
//   test[2] = 1; // Unit vector along z-axis
//   result = body2observ * test; // test vector in camera coordinates.
//   output_ << "Computed NORAZ = " << ::atan2 (-result[0], result[2]) / C::degree
//       << " degrees (positive is CCW)" << std::endl
//       << "Computed North pole vector in camera space = " << result << std::endl;
//   output_ << "observ2body = " << observ2body << std::endl
//       << "body2observ = " << body2observ << std::endl;
#endif
}

void
MaRC::PhotoImageParameters::remove_sky (void)
{
  // Note that supersample verification implies sky removal.
  if (    flags::check(this->flags_, REMOVE_SKY)
       || flags::check(this->flags_, SS_VERIFY)
     ) {
    this->sky_mask_ = Image<bool> (this->samples_, this->lines_);
  } else {
    // The fast path.
    return;
  }

  // FIXME: This routine is currently oblate spheroid specific.

  const unsigned int llen = this->lines_ - this->nibble_bottom_;
  const unsigned int slen = this->samples_ - this->nibble_right_;

  for (unsigned int k = this->nibble_top_; k < llen; ++k)
    {
      unsigned int const offset = k * this->samples_;

      for (unsigned int i = this->nibble_left_; i < slen; ++i)
        {
          double z = k;  // Reset "z" prior to geometric correction.
                         // Do not move to outer loop!
          double x = i;

          z = this->line_center_ - z;
            // Neg. since line numbers increase top to bottom (?)
          x = x - this->sample_center_;

          // Convert from image space to object space.
          this->geometric_correction_->image_to_object (x, z);

          // ---------------------------------------------
          // Convert from observer coordinates to body coordinates
          DVector coord;
          coord[0] = x;
          coord[1] = 0;
          coord[2] = z;

          // Do the transformation
          DVector rotated (this->observ2body_ * coord);

          for (unsigned int n = 0; n < 3; ++n)
            rotated[n] *= this->km_per_pixel_;

          // ---------------------------------------------

          // Vector from observer to point on image
          DVector const dVec (rotated - this->range_b_);

          double lat = 0, lon = 0;

          unsigned int const index =  offset + i;

          // Any value below this threshold is considered zero.
          static double const zero_threshold = 1e-11;

          // Consider zero/NaN data points invalid, i.e. "off the
          // body".
          if (this->body_.ellipse_intersection (this->range_b_,
                                                dVec,
                                                lat,
                                                lon) == 0
              && !isnan (this->image_[index])
              && fabs (this->image_[index]) >= zero_threshold)
            {
              // On body
              this->sky_mask_[index] = true;

//               if (lat < this->min_lat_)
//                 this->min_lat_ = lat;
//               else if (lat > this->max_lat_)
//                 this->max_lat_ = lat;

//               if (lon < this->min_lon_)
//                 this->min_lon_ = lon;
//               else if (lon > this->max_lon_)
//                 this->max_lon_ = lon;
            }
        }
    }
}
