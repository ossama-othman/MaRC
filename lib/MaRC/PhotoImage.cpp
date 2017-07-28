/**
 * @file PhotoImage.cpp
 *
 * Copyright (C) 1998-1999, 2003-2005, 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

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
#include <cassert>

#ifndef MARC_DEFAULT_GEOM_CORR_STRATEGY
# define MARC_DEFAULT_GEOM_CORR_STRATEGY MaRC::NullGeometricCorrection
#endif  /* MARC_DEFAULT_GEOM_CORR_STRATEGY */

#ifndef MARC_DEFAULT_PHOTO_CORR_STRATEGY
# define MARC_DEFAULT_PHOTO_CORR_STRATEGY MaRC::NullPhotometricCorrection
#endif  /* MARC_DEFAULT_PHOTO_CORR_STRATEGY */

#ifndef MARC_DEFAULT_INTERPOLATION_STRATEGY
# define MARC_DEFAULT_INTERPOLATION_STRATEGY MaRC::NullInterpolationStrategy
#endif  /* MARC_DEFAULT_INTERPOLATION_STRATEGY */


MaRC::PhotoImage::PhotoImage(OblateSpheroid const & body,
                             std::vector<double> image,
                             std::size_t samples,
                             std::size_t lines,
                             std::unique_ptr<GeometricCorrection> gc)
    : SourceImage()
    , body_(body)
    , image_(std::move(image))
    , samples_(samples)
    , lines_(lines)
    , geometric_correction_(gc
                            ? std::move(gc)
                            : std::make_unique<MARC_DEFAULT_GEOM_CORR_STRATEGY>())
    , photometric_correction_(
        std::make_unique<MARC_DEFAULT_PHOTO_CORR_STRATEGY>())
    , interpolation_strategy_(
        std::make_unique<MARC_DEFAULT_INTERPOLATION_STRATEGY>())
    , sub_observ_lat_(0)
    , sub_observ_lon_(0)
    , sub_solar_lat_ (0)
    , sub_solar_lon_ (0)
    , range_(::sqrt (std::numeric_limits<double>::max () - 1))
    , position_angle_(0)
    , arcsec_per_pixel_(-1)      // Force "bad" value until set by caller
    , km_per_pixel_  (-1)        // Force "bad" value until set by caller
    , focal_length_  (-1)        // Force "bad" value until set by caller
    , focal_length_pixels_(-1)   // Force "bad" value until fully initialized
    , scale_ (-1)                // Force "bad" value until set by caller
    , normal_range_  (0)
    , OA_s_          (0)
    , OA_l_          (0)
    , sky_mask_      (samples, lines) // Enable sky removal.
    , range_b_       ()
    , observ2body_   ()
    , body2observ_   ()
    , nibble_left_   (0)
    , nibble_right_  (0)
    , nibble_top_    (0)
    , nibble_bottom_ (0)
    , sample_center_ (0)
    , line_center_   (0)
    , lat_at_center_ (0)
    , lon_at_center_ (0)
    , mu_limit_(::cos (90 * C::degree)) // cos() of 90 deg emission angle limit
    // , min_lat_(C::pi_2)  // Initialize to maximum (yes, the maximum!) possible
    // , max_lat_(-C::pi_2) // Initialize to minimum possible
    // , min_lon_(C::_2pi)  // Initialize to maximum possible
    //,  max_lon_(0)        // Initialize to minimum possible
    , flags_(0)
{
    if (image.empty())
        throw std::invalid_argument(
            "PhotoImage: Empty source image array.");

    if (samples < 2 || lines < 2) {
        // Why would there ever be a one pixel source image?
        std::ostringstream s;
        s << "Source image samples (" << samples
          << ") and lines (" << lines
          << ") must both be greater than one.";

        throw std::invalid_argument(s.str ());
    }

    if (image.size() != samples * lines) {
        throw std::invalid_argument(
            "Source image size does not match samples and lines");
    }
}

MaRC::PhotoImage::~PhotoImage (void)
{
}

bool
MaRC::PhotoImage::operator==(PhotoImage const & img)
{
  return (this->samples_ == img.samples_
          && this->lines_ == img.lines_
          && this->flags_ == img.flags_
          && this->arcsec_per_pixel_ == img.arcsec_per_pixel_
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
          && this->position_angle_ == img.position_angle_);
}

bool
MaRC::PhotoImage::is_visible(double lat, double lon) const
{
    //   const double radius = this->body_.centric_radius (lat);
    const double latg   = this->body_.graphic_latitude (lat);

    /*
    const double cosine =
        (radius * ::cos (lat - latg) -
        this->range_ * ::sin (this->sub_observ_lat_) * ::sin (latg)) /
        (this->range_ * ::cos (this->sub_observ_lat_) * ::cos (latg));

    double lower, upper;

    if (cosine >= -1 && cosine <= 1) {
        // Partial range of longitudes are visible.

        const double edge = ::fabs (::acos (cosine));

        lower = this->sub_observ_lon_ - edge;
        upper = this->sub_observ_lon_ + edge;
    }  else if (cosine < -1) {
        // Full 360 degree visible longitude range.
        lower = this->sub_observ_lon_ - C::pi;
        upper = this->sub_observ_lon_ + C::pi;
    } else {
        return false;  // Not visible
    }
    */

    if (flags::check(this->flags_, USE_TERMINATOR)) {
        // The following equation assumes the sun to be an infinite
        // distance away from the observed body.

        // tcosine = ::pow(this->body_.eq_rad() / this->body_.pol_rad(), 2.0) *
        //   ::tan(lat) * ::tan(this->sub_solar_lat_);
        double const tcosine = ::tan(latg) * ::tan(this->sub_solar_lat_);

        if (tcosine >= -1 && tcosine <= 1) {
            double const tedge = ::fabs (::acos (-tcosine));

            double const lower_terminator = this->sub_solar_lon_ - tedge;
            double const upper_terminator = this->sub_solar_lon_ + tedge;

            /*
              if (lower_terminator > lower)
                  lower = lower_terminator;

              if (upper_terminator < upper)
                  upper = upper_terminator;
            */

            return (lon >= lower_terminator && lon <= upper_terminator);
        } else {
            return false;
        }
    }

    /*
    // Wrap around if necessary.
    if (lower > lon)
        lower -= C::_2pi;
    else if (upper < lon)
        upper += C::_2pi;

    // Now check if longitude at given latitude is visible.
    if (lon < lower || lon > upper)
        return false;  // Not visible
    */

    // Assume it is visible regardless of location of terminator.
    return true;
}

int
MaRC::PhotoImage::geometric_correction(
    std::unique_ptr<GeometricCorrection> strategy)
{
    if (strategy) {
        this->geometric_correction_ = std::move(strategy);

        return 0;  // Success
    } else {
        std::cerr
            << "ERROR: Null geometric correction strategy pointer.\n";

        return 1;  // Failure
    }
}

int
MaRC::PhotoImage::photometric_correction(
    std::unique_ptr<PhotometricCorrection> strategy)
{
    if (strategy) {
        this->photometric_correction_ = std::move(strategy);

        return 0;  // Success
    } else {
        std::cerr
            << "ERROR: Null photometric correction strategy pointer.\n";

        return 1;  // Failure
    }
}

int
MaRC::PhotoImage::sub_observ (double lat, double lon)
{
  if (::fabs (lat) <= 90)
    this->sub_observ_lat_ = lat * C::degree;
  else
    {
      std::cerr << "ERROR: Incorrect value for Sub-Observation Latitude: "
                << lat << std::endl;

      return 1;  // Failure
    }

  if (::fabs (lon) <= 360)
    {
      if (lon < 0)
        lon += 360;
      this->sub_observ_lon_ = lon * C::degree;
    }
  else
    {
      std::cerr
        << "ERROR: Incorrect value for Central Meridian: " << lon << std::endl;

      return 1;  // Failure
    }

  return 0;  // Success
}

int
MaRC::PhotoImage::sub_observ_lat (double lat)
{
  if (::fabs (lat) <= 90)
    this->sub_observ_lat_ = lat * C::degree;
  else
    {
      std::cerr << "ERROR: Incorrect value for Sub-Observation Latitude: "
                << lat << std::endl;

      return 1;  // Failure
    }

  return 0;  // Success
}

int
MaRC::PhotoImage::sub_observ_lon (double lon)
{
  if (::fabs (lon) <= 360)
    {
      if (lon < 0)
        lon += 360;
      this->sub_observ_lon_ = lon * C::degree;
    }
  else
    {
      std::cerr
        << "ERROR: Incorrect value for Central Meridian: " << lon << std::endl;

      return 1;  // Failure
    }

  return 0;
}

int
MaRC::PhotoImage::sub_solar (double lat, double lon)
{
  if (::fabs (lat) <= 90)
    this->sub_solar_lat_ = lat * C::degree;
  else
    {
      std::cerr << "ERROR: Incorrect value for Sub-Solar Latitude: "
                << lat << std::endl;

      return 1;  // Failure
    }

  if (::fabs (lon) <= 360)
    {
      if (lon < 0)
        lon += 360;
      this->sub_solar_lon_ = lon * C::degree;
    }
  else
    {
      std::cerr << "ERROR: Incorrect value for Sub-Solar Longitude: " << lon
                << std::endl;

      return 1;  // Failure
    }

  return 0;
}

int
MaRC::PhotoImage::sub_solar_lat (double lat)
{
  if (::fabs (lat) <= 90)
    this->sub_solar_lat_ = lat * C::degree;
  else
    {
      std::cerr << "ERROR: Incorrect value for Sub-Solar Latitude: "
                << lat << std::endl;

      return 1;  // Failure
    }

  return 0;  // Success
}

int
MaRC::PhotoImage::sub_solar_lon (double lon)
{
  if (::fabs (lon) <= 360)
    {
      if (lon < 0)
        lon += 360;
      this->sub_solar_lon_ = lon * C::degree;
    }
  else
    {
      std::cerr << "ERROR: Incorrect value for Sub-Solar Longitude: " << lon
                << std::endl;

      return 1;  // Failure
    }

  return 0;  // Success
}


int
MaRC::PhotoImage::position_angle (double north)
{
  if (::fabs (north) <= 360)
    this->position_angle_ = north * C::degree;
  else
    {
      std::cerr << "ERROR: Incorrect position angle: "
                << north << std::endl;

      return 1;  // Failure
    }

  return 0;  // Success
}

void
MaRC::PhotoImage::finalize_setup (void)
{
  // Run some sanity checks on nibbling values
  if (this->samples_ - this->nibble_right_ < this->nibble_left_)
    {
      std::cerr
        << "WARNING: Either the left or right (or both) nibble value is "
        << "too large." << std::endl
        << "         Both the left and right nibble values will be set to "
        << "zero." << std::endl;
      this->nibble_left_  = 0;
      this->nibble_right_ = 0;
    }
  if (this->lines_ - this->nibble_top_ < this->nibble_bottom_)
    {
      std::cerr
        << "WARNING: Either the top or bottom (or both) nibble value is"
        << " too large." << std::endl
        << "         Both the top and bottom nibble values will be set to "
        << "zero." << std::endl;
      this->nibble_top_    = 0;
      this->nibble_bottom_ = 0;
    }

  //////////////////////////////////////
  // All necessary image values and attributes should be set by now!
  //////////////////////////////////////

  if (this->km_per_pixel_ <= 0)
    this->set_km_per_pixel ();

  // Set optical axis
  if (!flags::check (this->flags_, OA_SET))
    {
      this->OA_s_ = this->samples_ / 2.;
      this->OA_l_ = this->lines_ / 2.;
      flags::set (this->flags_, OA_SET);
    }

  // Set Body center to observer vectors
  this->range_b_[0] =  0;
  this->range_b_[1] = -this->range_ * ::cos (this->sub_observ_lat_);
  this->range_b_[2] =  this->range_ * ::sin (this->sub_observ_lat_);

  /// Perpendicular distance from observer to image plane.
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

      this->normal_range_ =
        ::sqrt (this->range_ * this->range_ - magRo * magRo);

      // In case focal length and scale are not set or used.
      range_O[1] = -this->normal_range_;

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

      for (std::size_t i = 0; i < 3; ++i)
        r_OA[i] = dp * OA_hat[i];

      // Optical axis in body coordinates
      DVector const OpticalAxis (OA_prime - r_OA);

      this->rot_matrices (this->range_b_,
                          OpticalAxis,
                          this->observ2body_,
                          this->body2observ_);

      // In case focal length and scale are not set or used.
      this->normal_range_ = -(this->body2observ_ * this->range_b_)[1];
    }

  if (this->focal_length_pixels_ < 0)
    {
      this->focal_length_pixels_ =
        this->normal_range_ / this->km_per_pixel_;
    }

  // Scan across and determine where points lie off of the body, i.e.
  // remove the sky from the image.  The image will not actually be
  // modified.
  this->remove_sky (); // Remove sky from source image
}

// Get rotation matrices for case when body centers were given
int
MaRC::PhotoImage::rot_matrices (const DVector & range_o,
                                DMatrix & observ2body,
                                DMatrix & body2observ)
{
  DVector r_o (range_o);

  // ----------- TRY THE MOST POSITIVE ROOT ----------

  // Compute transformation matrices
  Geometry::toUnitVector (r_o);

  DVector rotated;
  Geometry::RotY (-this->position_angle_, r_o, rotated);
  r_o = rotated;
  DVector const temp2 (rotated);

  const double a = r_o[2] * r_o[2] + r_o[1] * r_o[1];
  const double b = 2 * r_o[1] * ::sin (this->sub_observ_lat_);
  const double c =
    ::sin (this->sub_observ_lat_) * ::sin (this->sub_observ_lat_) -
    r_o[2] * r_o[2];

  double det = b * b - 4 * a * c;

  double SubLatModSin[2];

  if (det >= 0)
    {
      det = ::sqrt (det);
      SubLatModSin[0] = (-b + det) / (2 * a);
      SubLatModSin[1] = (-b - det) / (2 * a);
    }
  else
    {
      std::cerr
        << "Error occurred in rot_matrices() routine." << std::endl
        << "Attempt to take square root of a negative number." << std::endl;

      return 1;  // Failure
    }

  // ------------- TRY MOST POSITIVE ROOT ------------
  double SubLatMod =
    ::asin (SubLatModSin[0]);  // compute from most positive root

  Geometry::RotX (SubLatMod, r_o, rotated);
  r_o = rotated;

  double Ztwist = ::atan2 (r_o[0], -r_o[1]);

  // Observer to body transformation
  DMatrix o2b (Geometry::RotZMatrix (Ztwist)
               * (Geometry::RotXMatrix (SubLatMod) *
                  Geometry::RotYMatrix (-this->position_angle_)));

  observ2body = o2b;

  double diff_magnitude =
    Geometry::Magnitude (this->range_b_ - o2b * range_o);

  // ----------- TRY MOST NEGATIVE ROOT -------------
  r_o = temp2; // Reset to value of vector after first rotation

  SubLatMod = ::asin (SubLatModSin[1]); // compute from most negative root
  Geometry::RotX (SubLatMod, r_o, rotated);
  r_o = rotated;

  Ztwist = ::atan2 (r_o[0], -r_o[1]);

  // Observer to body transformation
  o2b =
    Geometry::RotZMatrix (Ztwist)
    * (Geometry::RotXMatrix (SubLatMod) *
       Geometry::RotYMatrix (-this->position_angle_));

  const double test_diff_magnitude =
    Geometry::Magnitude (this->range_b_ - o2b * range_o);

  if (diff_magnitude > test_diff_magnitude)
    {
      diff_magnitude = test_diff_magnitude;
      observ2body = o2b;
    }

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

  // get reverse transformation by taking transpose since transformation
  // matrices are orthogonal
  body2observ = MaRC::transpose (observ2body);
  // Body to observer transformation

#ifdef DEBUG
  for (std::size_t i = 0; i < 3; ++i)
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
MaRC::PhotoImage::rot_matrices (const DVector & range_b,
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
  for (std::size_t i = 0; i < 3; ++i)
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

void MaRC::PhotoImage::remove_sky(bool r)
{
    if (r && this->sky_mask_.empty())
        this->sky_mask_.resize(this->samples_ * this->lines_);
    else
        this->sky_mask_.clear();
}

void
MaRC::PhotoImage::remove_sky()
{
    if (this->sky_mask_.empty())
        return;

    /**
     * @todo This routine is currently oblate spheroid specific.
     */

    std::size_t const llen = this->lines_   - this->nibble_bottom_;
    std::size_t const slen = this->samples_ - this->nibble_right_;

    for (std::size_t k = this->nibble_top_; k < llen; ++k) {
        std::size_t const offset = k * this->samples_;

        for (std::size_t i = this->nibble_left_; i < slen; ++i) {
            double z = k;  // Reset "z" prior to geometric
                           // correction. Do not move to outer loop!
            double x = i;

            // Convert from image space to object space.
            this->geometric_correction_->image_to_object(z, x);

            z -= this->line_center_;
            x -= this->sample_center_;

            // ---------------------------------------------
            // Convert from observer coordinates to body coordinates
            DVector coord;
            coord[0] = x;
            coord[1] = 0;
            coord[2] = -z; // Negative since line numbers increase top to
                           // bottom (?)
            //      coord[2] = z;

            // Do the transformation
            DVector rotated(this->observ2body_ * coord);

            for (std::size_t n = 0; n < 3; ++n)
                rotated[n] *= this->km_per_pixel_;

            // ---------------------------------------------

            // Vector from observer to point on image
            DVector const dVec(rotated - this->range_b_);

            double lat = 0, lon = 0;

            std::size_t const index =  offset + i;

            // Any value below this threshold is considered zero.
            static double const zero_threshold = 1e-11;

            // Consider zero/NaN data points invalid, i.e. "off the
            // body".
            if (this->body_.ellipse_intersection (this->range_b_,
                                                  dVec,
                                                  lat,
                                                  lon) == 0
                && !std::isnan(this->image_[index])
                && fabs (this->image_[index]) >= zero_threshold) {
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

int
MaRC::PhotoImage::arcsec_per_pixel(double a)
{
    // NOTE: range_ should be in units of kilometers at this point.
    if (a > 0) {
        this->arcsec_per_pixel_ = a;
        this->km_per_pixel_ =
            C::pi / 648e3 * this->range_ * this->arcsec_per_pixel_;
    } else {
        std::cerr
            << "ERROR: Incorrect number of arcseconds "
               "per pixel entered: " << a << '\n';

        return 1;  // Failure
    }

    return 0;
}


int
MaRC::PhotoImage::km_per_pixel(double value)
{
    if (value > 0)
        this->km_per_pixel_ =  value;
    else {
        std::cerr
            << "ERROR: Incorrect number of kilometers per pixel entered: "
            << value << std::endl;

        return 1;  // Failure
    }

  return 0;  // Success
}

int
MaRC::PhotoImage::set_km_per_pixel()
{
    if (this->focal_length_ > 0 && this->scale_ > 0)  {
        // User has set these values

        if (!flags::check(this->flags_, OA_SET)) {
            // Make sure we have a "good" default value.

            // samples_ and lines_ should be set by now!
            this->OA_s_ = this->samples_ / 2.;
            this->OA_l_ = this->lines_ / 2.;
            flags::set(this->flags_, OA_SET);
        }

        this->focal_length_pixels_ =
            this->focal_length_ * this->scale_;

        this->km_per_pixel_ =
            this->range_ /
            ::sqrt(this->focal_length_pixels_
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
    } else if (this->km_per_pixel_ <= 0) {
        std::cerr
            << "ERROR: Attempt to compute number of kilometers per pixel\n"
            << "       where no focal length and scale have been set.\n";

        return 1;  // Failure
    }

    // If km_per_pixel_ > 0 than, presumably, km_per_pixel_ has
    // already been set; so do nothing.

    return 0; // Success
}

int
MaRC::PhotoImage::focal_length(double len)
{
    if (len > 0)
        this->focal_length_ = len;
    else {
        std::cerr << "Incorrect focal length entered: " << len << '\n';

        return 1;  // Failure
    }

    return 0;
}

int
MaRC::PhotoImage::scale(double s)
{
    if (s > 0)
        this->scale_ = s;
    else {
      std::cerr << "Incorrect scale entered: " << s << '\n';

      return 1;  // Failure
    }

    return 0;  // Success
}

void
MaRC::PhotoImage::body_center(double sample, double line)
{
  this->sample_center_ = sample;
  this->line_center_ = line;
}

void
MaRC::PhotoImage::body_center_sample(double sample)
{
    this->sample_center_ = sample;
}

void
MaRC::PhotoImage::body_center_line(double line)
{
    this->line_center_ = line;
}

int
MaRC::PhotoImage::lat_lon_center(double lat, double lon)
{
    if (::fabs (lat) <= 90) {
        this->lat_at_center_ = lat * C::degree;
        flags::set(this->flags_, LATLON_AT_CENTER);
    } else {
        std::cerr << "INCORRECT Latitude at picture center, entered: "
                  << lat << '\n';

        return 1;  // Failure
    }

    if (::fabs (lon) <= 360) {
        this->lon_at_center_ = lon * C::degree;
        flags::set(this->flags_, LATLON_AT_CENTER);
    } else {
        std::cerr << "INCORRECT Longitude at picture center, entered: "
                  << lon << '\n';

        return 1;  // Failure
    }

    return 0;
}

int
MaRC::PhotoImage::lat_at_center(double lat)
{
    if (::fabs (lat) <= 90) {
        this->lat_at_center_ = lat * C::degree;
        flags::set(this->flags_, LATLON_AT_CENTER);
    } else {
        std::cerr << "INCORRECT Latitude at picture center, entered: "
                  << lat << '\n';

        return 1;  // Failure
    }

    return 0;  // Success
}

int
MaRC::PhotoImage::lon_at_center(double lon)
{
    if (::fabs (lon) <= 360) {
        this->lon_at_center_ = lon * C::degree;
      flags::set (this->flags_, LATLON_AT_CENTER);
    } else {
        std::cerr << "INCORRECT Longitude at picture center, entered: "
                  << lon << '\n';

        return 1;  // Failure
    }

    return 0;  // Success
}

void
MaRC::PhotoImage::optical_axis(double sample, double line)
{
    this->OA_s_ = sample;
    this->OA_l_ = line;
    flags::set(this->flags_, OA_SET);
}

void
MaRC::PhotoImage::optical_axis_sample(double sample)
{
    this->OA_s_ = sample;
    flags::set(this->flags_, OA_SET);
}

void
MaRC::PhotoImage::optical_axis_line(double line)
{
    this->OA_l_ = line;
    flags::set(this->flags_, OA_SET);
}

int
MaRC::PhotoImage::range(double r)
{
    // value should be in kilometers!
    double const mr =
        std::min(this->body_.eq_rad(), this->body_.pol_rad());

    if (r > mr && r < ::sqrt(std::numeric_limits<double>::max ()) - 1)
        this->range_ =  r;
    else {
        std::cerr << "ERROR: Incorrect range entered." << std::endl
                  << "       Range must be greater than "
                  << mr
                  << '\n'
                  << "       and less than "
                  << ::sqrt (std::numeric_limits<double>::max ()) - 1
                  << '\n';

        return 1;  // Failure
    }

    return 0;  // Success
}

void
MaRC::PhotoImage::nibble(std::size_t n)
{
    std::size_t const minimum_dimension =
        (this->samples_ < this->lines_ ? this->samples_ : this->lines_);

    if (n < (minimum_dimension / 2)) {
      this->nibble_left_   = n;
      this->nibble_right_  = n;
      this->nibble_top_    = n;
      this->nibble_bottom_ = n;
    } else {
      std::ostringstream s;
      s << "Invalid overall nibble value (" << n << ")";

      throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_left(std::size_t n)
{
    if (n < (this->samples_ - this->nibble_right_))
        this->nibble_left_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble left value (" << n << ")";

        throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_right(std::size_t n)
{
    if (n < (this->samples_ - this->nibble_left_))
        this->nibble_right_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble right value (" << n << ")";

        throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_top(std::size_t n)
{
    if (n < (this->lines_ - this->nibble_bottom_))
        this->nibble_top_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble top value (" << n << ")";

        throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_bottom(std::size_t n)
{
    if (n < (this->lines_ - this->nibble_top_))
        this->nibble_bottom_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble bottom value (" << n << ")";

        throw std::invalid_argument (s.str ());
    }
}

void
MaRC::PhotoImage::interpolate(bool enable)
{
    if (enable) {
        this->interpolation_strategy_ =
            std::make_unique<PhotoInterpolationStrategy>(
                this->samples_,
                this->lines_,
                this->nibble_left_,
                this->nibble_right_,
                this->nibble_top_,
                this->nibble_bottom_);
    } else {
        this->interpolation_strategy_ =
            std::make_unique<NullInterpolationStrategy>();
    }
}

int
MaRC::PhotoImage::emi_ang_limit(double angle)
{
    if (angle > 0 && angle < 90) {
        this->mu_limit_ = ::cos(angle * C::degree);
        flags::set(this->flags_, EMI_ANG_LIMIT);
    }
    else if (angle == static_cast<double>(90)) {
        // Value equal to 90 means no cut-off, so we don't
        // switch on the emission angle cut-off code.
        flags::unset (this->flags_, EMI_ANG_LIMIT);
    } else {
        std::cerr << "Incorrect value value passed to EmiAngLimit routine: "
                  << angle << '\n';

        return 1;  // Failure
    }

    return 0;  // Success
}

bool
MaRC::PhotoImage::read_data(double lat, double lon, double & data) const
{
    std::size_t weight = 1;  // Unused.

    static const bool scan = false; // Do not scan for data weight.

    return this->read_data (lat, lon, data, weight, scan);
}

bool
MaRC::PhotoImage::read_data(double lat,
                            double lon,
                            double & data,
                            std::size_t & weight,
                            bool scan) const
{
    if (!this->is_visible(lat, lon))
        return false;  // Failure

    // Do not plot data close to limb
    if (flags::check(this->flags_, EMI_ANG_LIMIT)
        && this->body_.mu(this->sub_observ_lat_,
                          this->sub_observ_lon_,
                          lat,
                          lon,
                          this->range_) < this->mu_limit_) {
        return false;  // Outside the configured emission angle limit.
    }

    double x = 0, z = 0;

    if (!this->latlon2pix(lat, lon, x, z)
        || x < 0    // Prevent integer underflow since we're casting
        || z < 0)   // to an unsigned integer below.
        return false;

    std::size_t const i = static_cast<std::size_t>(::rint(x));
    std::size_t const k = static_cast<std::size_t>(::rint(z));

    // e.g., if (i < 0 || i >= samples_ || k < 0 || k >= lines_)
    // The following assumes that line numbers increase downward.
    // CHECK ME!
    if (i < this->nibble_left_   || i >= this->samples_ - this->nibble_right_
        || k < this->nibble_top_ || k >= this->lines_ - this->nibble_bottom_
        || (!this->sky_mask_.empty()
            && !this->sky_mask_[k * this->samples_ + i]))
        return false;

    data = this->image_[k * this->samples_ + i];

    if (!this->interpolation_strategy_->interpolate(this->image_.data(),
                                                    x,
                                                    z,
                                                    data)
        || this->photometric_correction_->correct(this->body_,
                                                  this->sub_observ_lat_,
                                                  this->sub_observ_lon_,
                                                  this->sub_solar_lat_,
                                                  this->sub_solar_lon_,
                                                  lat,
                                                  lon,
                                                  this->range_,
                                                  data) != 0
        || std::isnan (data)) {
        return false;
    }

    // Scan across image for "off-planet/image" pixels and compute
    // averaging weights.
    //
    // Note that a weight is computed regardless of whether or not sky
    // removal is enabled.
    if (scan) {
        std::size_t & shortest_distance = weight;

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
        if (!this->sky_mask_.empty()) {
            auto const sky_mask = this->sky_mask_.cbegin();

            std::size_t const offset = k * this->samples_;

            // Search from i to nibble_right.
            auto begin = sky_mask + (offset + i);
            auto end   = sky_mask +
                (offset + (this->samples_ - this->nibble_right_));

            auto result = std::find(begin, end, true);

            assert(begin < result);

            shortest_distance =
                std::min(
                    static_cast<std::size_t>(std::distance(begin, result)),
                    shortest_distance);

            // Search from nibble_left to i.
            begin = sky_mask + (offset + this->nibble_left_);
            end   = sky_mask + (i + 1);

            result = std::find(begin, end, true);

            shortest_distance =
                std::min(
                    static_cast<std::size_t>(std::distance(begin, result)),
                    shortest_distance);

            // Search from k to nibble_bottom.
            std::size_t const kend = this->lines_ - this->nibble_bottom_;

            std::size_t kk;
            for (kk = k;
                 kk < kend && sky_mask[kk * this->samples_ + i];
                 ++kk)
                ; // Nothing

            shortest_distance = std::min(kk - k, shortest_distance);

            // Search from nibble_top to k.
            for (kk = k;
                 kk >= this->nibble_top_
                     && sky_mask[kk * this->samples_ + i];
                 --kk)
                ; // Nothing

            shortest_distance = std::min(k - kk, shortest_distance);
        }
    }

    return true;  // Success
}

bool
MaRC::PhotoImage::latlon2pix(double lat,
                             double lon,
                             double & x,
                             double & z) const
{
    double const radius = this->body_.centric_radius(lat);

    double longitude;

    if (this->body_.prograde ())
        longitude = this->sub_observ_lon_ - lon;
    else
        longitude = lon - this->sub_observ_lon_;

    DVector Coord;
    Coord[0] =  radius * ::cos(lat) * ::sin(longitude);
    Coord[1] = -radius * ::cos(lat) * ::cos(longitude);
    Coord[2] =  radius * ::sin(lat);

//   Rotated = this->body2observ_ * Coord; // Convert to observer coordinates

//   x = (Rotated[0] / this->km_per_pixel_ - (OA_s_ - this->sample_center_)) * this->normal_range_ /
//     (this->normal_range_ + Rotated[1]);
//   z = (Rotated[2] / this->km_per_pixel_ + (OA_l_ - this->line_center_)) * this->normal_range_ /
//     (this->normal_range_ + Rotated[1]);
                     // Why should we add instead of subtract?
                     // For some reason incorrect results occur if
                     // the supposedly correct subtraction is used.

    DVector const Obs(Coord - this->range_b_);

    // Convert to observer coordinates.
    DVector const Rotated(this->body2observ_ * Obs);

    if (Rotated[1] > this->normal_range_)
        return false;  // On other side of image plane / body.

    x = Rotated[0] / Rotated[1] * this->focal_length_pixels_;
    z = Rotated[2] / Rotated[1] * this->focal_length_pixels_;

    x += this->OA_s_;
    //  z += this->OA_l_;
    z = this->OA_l_ - z; // Assumes line numbers increase top to bottom.

    //std::cout << "* x = " << x << "\tz = " << z << std::endl;

    // Convert from object space to image space.
    this->geometric_correction_->object_to_image(z, x);

    return true;
}

void
MaRC::PhotoImage::use_terminator(bool u)
{
    if (u)
        flags::set (this->flags_, USE_TERMINATOR);
    else
        flags::unset (this->flags_, USE_TERMINATOR);
}
