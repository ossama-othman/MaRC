/**
 * @file ViewingGeometry.cpp
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

#include "ViewingGeometry.h"
#include "Constants.h"
#include "OblateSpheroid.h"
#include "Mathematics.h"
#include "Validate.h"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <iostream>


namespace
{
    static constexpr double not_a_number =
        std::numeric_limits<double>::signaling_NaN();
}


MaRC::ViewingGeometry::ViewingGeometry(
    std::shared_ptr<OblateSpheroid> body)
    : body_(body)
    , sub_observ_lat_(not_a_number)
    , sub_observ_lon_(not_a_number)
    , sub_solar_lat_ (not_a_number)
    , sub_solar_lon_ (not_a_number)
    , range_         (not_a_number)
    , position_angle_(not_a_number)
    , km_per_pixel_  (not_a_number)
    , focal_length_  (not_a_number)
    , focal_length_pixels_(not_a_number)
    , scale_         (not_a_number)
    , normal_range_  (not_a_number)
    , OA_s_          (not_a_number)
    , OA_l_          (not_a_number)
    , range_b_       ()
    , observ2body_   ()
    , body2observ_   ()
    , sample_center_ (not_a_number)
    , line_center_   (not_a_number)
    , lat_at_center_ (not_a_number)
    , lon_at_center_ (not_a_number)
    , mu_limit_      (0) // cos(90 * C::degree) emission angle limit
{
}

MaRC::ViewingGeometry::~ViewingGeometry()
{
}

bool
MaRC::ViewingGeometry::is_visible(double lat, double lon) const
{
    /*
      mu is the cosine of the angle between:

      -- the vector from the given point to the observer
      -- the normal vector to the surface at the given point

      For a convex body, if this is positive, the point is on the
      visible side of the planet, and if it's negative, the point is
      on the far side of the planet.

      Take into account an emission angle limit potentially set by the
      user as well.
    */
    return
        this->body_->mu(this->sub_observ_lat_,
                        this->sub_observ_lon_,
                        lat,
                        lon,
                        this->range_) > this->mu_limit_

        /*
          mu0 is the angle between:

          -- the vector from the given point to the sun
          -- the normal vector to the surface at the given point

          The sun is assumed to be an infinite distance away.  For a
          convex body, if this is positive, the point is on the lit
          side of the planet, and if it's negative, the point is on
          the dark side of the planet.
        */
        && (!this->use_terminator_
            || this->body_->mu0(this->sub_solar_lat_,
                                this->sub_solar_lon_,
                                lat,
                                lon) > 0);

     // Visible if both the far-side and (if requested) the dark-side
     // checks passed.
}

void
MaRC::ViewingGeometry::sub_observ(double lat, double lon)
{
    this->sub_observ_lat(lat);
    this->sub_observ_lon(lon);
}

void
MaRC::ViewingGeometry::sub_observ_lat(double lat)
{
    this->sub_observ_lat_ = MaRC::validate_latitude(lat);
}

void
MaRC::ViewingGeometry::sub_observ_lon(double lon)
{
    this->sub_observ_lon_ = MaRC::validate_longitude(lon);

    if (this->sub_observ_lon_ < 0)
        this->sub_observ_lon_ += C::_2pi;  // Equivalent positive
                                           // longitude.
}

void
MaRC::ViewingGeometry::sub_solar(double lat, double lon)
{
    this->sub_solar_lat(lat);
    this->sub_solar_lon(lon);
}

void
MaRC::ViewingGeometry::sub_solar_lat(double lat)
{
    this->sub_solar_lat_ = MaRC::validate_latitude(lat);
}

void
MaRC::ViewingGeometry::sub_solar_lon(double lon)
{
    this->sub_solar_lon_ = MaRC::validate_longitude(lon);

    if (this->sub_solar_lon_ < 0)
        this->sub_solar_lon_ += C::_2pi;  // Equivalent positive
                                          // longitude.
}

void
MaRC::ViewingGeometry::position_angle(double north)
{
    this->position_angle_ = MaRC::validate_position_angle(north);
}

void
MaRC::ViewingGeometry::finalize_setup()
{
    // All necessary image values and attributes should be set by now!

    if (std::isnan(this->km_per_pixel_))
        this->set_km_per_pixel();

    // Set Body center to observer vectors.

    // Range vector in body coordinates.
    this->range_b_[0] =  0;
    this->range_b_[1] = -this->range_ * std::cos(this->sub_observ_lat_);
    this->range_b_[2] =  this->range_ * std::sin(this->sub_observ_lat_);

    if (std::isnan(this->lat_at_center_)
        || std::isnan(this->lon_at_center_)) {
        /**
         * @todo Verify that these values are consistent with the
         *       coordinate system, including directions of all axes.
         */
        DVector range_O;  // range_ vector in observer coordinates

        range_O[0] =
            (this->OA_s_ - this->sample_center_) * this->km_per_pixel_;

        // range_O[1] is default initialized to zero.  Set below.

        range_O[2] =
            (this->line_center_ - this->OA_l_) * this->km_per_pixel_;
        // Since line numbers increase top to bottom (e.g. VICAR images)

        double const magRo = MaRC::magnitude(range_O);

        // Perpendicular distance from observer to image plane.
        this->normal_range_ =
            std::sqrt(this->range_ * this->range_ - magRo * magRo);

        // In case focal length and scale are not set or used.
        range_O[1] = -this->normal_range_;

        this->rot_matrices(range_O,
                           this->observ2body_,
                           this->body2observ_);
    } else {
        double lon;

        if (this->body_->prograde())
            lon = this->sub_observ_lon_ - this->lon_at_center_;
        else
            lon = this->lon_at_center_ - this->sub_observ_lon_;

        double const radius =
            this->body_->centric_radius(this->lat_at_center_);

        DVector r0;
        r0[0] =  radius * std::cos(this->lat_at_center_) * std::sin(lon);
        r0[1] = -radius * std::cos(this->lat_at_center_) * std::cos(lon);
        r0[2] =  radius * std::sin(this->lat_at_center_);

        DVector const OA_prime(r0 - this->range_b_);

        DVector OA_hat(OA_prime);
        MaRC::to_unit_vector(OA_hat);

        // Dot product.
        double const dp = MaRC::dot_product(r0, OA_hat);

        DVector r_OA(dp * OA_hat);

        // Optical axis in body coordinates
        DVector const OpticalAxis(OA_prime - r_OA);

        this->rot_matrices(this->range_b_,
                           OpticalAxis,
                           this->observ2body_,
                           this->body2observ_);

        // Set perpendicular distance from observer to image plane in
        // case focal length and scale are not set or used.
        this->normal_range_ = -(this->body2observ_ * this->range_b_)[1];
    }

    if (std::isnan(this->focal_length_pixels_)) {
        this->focal_length_pixels_ =
            this->normal_range_ / this->km_per_pixel_;
    }
}

// Get rotation matrices for case when body centers were given
bool
MaRC::ViewingGeometry::rot_matrices(DVector const & range_o,
                                    DMatrix & observ2body,
                                    DMatrix & body2observ)
{
    DVector r_o(range_o);

    // ----------- TRY THE MOST POSITIVE ROOT ----------

    // Compute transformation matrices
    MaRC::to_unit_vector(r_o);

    DVector rotated;
    Geometry::RotY(-this->position_angle_, r_o, rotated);
    r_o = rotated;
    DVector const temp2(rotated);

    double const a = r_o[2] * r_o[2] + r_o[1] * r_o[1];
    double const b = 2 * r_o[1] * std::sin(this->sub_observ_lat_);
    double const c =
        std::sin(this->sub_observ_lat_) * std::sin(this->sub_observ_lat_)
        - r_o[2] * r_o[2];

    std::pair<double, double> SubLatModSin;

    if (!MaRC::quadratic_roots(a, b, c, SubLatModSin)) {
        // No real roots.
        std::cerr
            << "ERROR: Unable to find roots corresponding to\n"
               "       sub-observation latitudes when calculating\n"
               "       suitable rotation matrices to go between\n"
               "       observer and body coordinates.\n";

        return false;  // Failure
    }

    // ------------- TRY THE FIRST ROOT ------------
    double SubLatMod = std::asin(SubLatModSin.first);

    Geometry::RotX(SubLatMod, r_o, rotated);
    r_o = rotated;

    double Ztwist = std::atan2(r_o[0], -r_o[1]);

    // Observer to body transformation
    DMatrix o2b(Geometry::RotZMatrix(Ztwist)
                 * (Geometry::RotXMatrix(SubLatMod) *
                    Geometry::RotYMatrix(-this->position_angle_)));

    observ2body = o2b;

    double diff_magnitude =
        MaRC::magnitude(this->range_b_ - o2b * range_o);

    // ----------- TRY THE SECOND ROOT -------------
    r_o = temp2; // Reset to value of vector after first rotation

    SubLatMod = std::asin(SubLatModSin.second);
    Geometry::RotX(SubLatMod, r_o, rotated);
    r_o = rotated;

    Ztwist = std::atan2(r_o[0], -r_o[1]);

    // Observer to body transformation
    o2b =
        Geometry::RotZMatrix(Ztwist)
        * (Geometry::RotXMatrix(SubLatMod)
           * Geometry::RotYMatrix(-this->position_angle_));

    double const test_diff_magnitude =
        MaRC::magnitude(this->range_b_ - o2b * range_o);

    if (diff_magnitude > test_diff_magnitude) {
        diff_magnitude = test_diff_magnitude;
        observ2body = o2b;
    }

    double const percent_diff =
        diff_magnitude / MaRC::magnitude(this->range_b_);

    static constexpr double tolerance = 1e-8;
    if (percent_diff * 100 > tolerance) {
        // If greater than tolerance, warn.
        std::cerr
            << "\n"
            "WARNING: Results may be incorrect since a\n"
            "         \"suitable\" transformation matrix was\n"
            "         not found for the given image.\n"
            "         There was a "
            <<  percent_diff * 100 << "%\n"
            "         difference between the two test vectors."
            "\n"
            "         This warning occured since the percent\n"
            "         difference between the vectors was\n"
            "         greater than "
            << tolerance << "%.\n";
    }

    // Get reverse transformation by taking transpose since
    // transformation matrices are orthogonal
    body2observ = MaRC::transpose(observ2body);
    // Body to observer transformation

#ifdef DEBUG
    for (std::size_t i = 0; i < 3; ++i)
        std::cout
            << std::abs((this->range_b_[i] - (observ2body * range_o)[i])
                        / (this->range_b_[i] + (observ2body * range_o)[i])
                        * 2)
            << '\n';

    std::cout
        << "range_b_ = " << this->range_b_ << "\n"
        "range_b_ from transformation = " << observ2body * range_o
        << "\n"
        "range_o = " << range_o << "\n"
        "range_o from transformation = " << body2observ * this->range_b_
        << '\n';

    std::cout
        << "position_angle_ = "
        << this->position_angle_ / C::degree
        << " degrees (positive is CCW)\n"
        "SubLatMod = " << SubLatMod / C::degree << "\n"
        "Ztwist = " << Ztwist / C::degree << '\n';

    DVector test;
    test[2] = 1; // Unit vector along z-axis
    DVector result(body2observ * test); // Test vector in camera
                                            // coordinates.

    std::cout
        << "Computed NORAZ = "
        << std::atan2(-result[0], result[2]) / C::degree
        << " degrees (positive is CCW)\n"
        "Computed North pole vector in camera space = " << result
        << "\n"
           "observ2body = " << observ2body << "\n"
           "body2observ = " << body2observ << '\n';
#endif

    return true;
}

// Get rotation matrices for case when lat/lon at optical axis were given
void
MaRC::ViewingGeometry::rot_matrices(DVector const & range_b,
                                    DVector const & OA,
                                    DMatrix & observ2body,
                                    DMatrix & body2observ)
{
    double SubLatMod[2];

    DVector NPole, rotated;
    DVector OA_O;

    // Unit vector representing North pole in body coordinates
    NPole[2] = 1;

    /**
     * @todo Shouldn't this be @c OA_O[1] @c = @c -MaRC::magnitude(OA) ?
     */
    // OA_O is the optical axis vector in observer coordinates
    OA_O[1] = MaRC::magnitude(OA); // Magnitude of optical axis.

    DVector UnitOpticalAxis(OA); // Optical axis in body coordinates
    MaRC::to_unit_vector(UnitOpticalAxis);

    // Cosine of the angle between the North pole and the optical
    // axis.  No need to divide by the product of the vector
    // magnitudes since they're both unit vectors (magnitudes of 1).
    double const dotProd = MaRC::dot_product(NPole, UnitOpticalAxis);

    /**
     * @todo Shouldn't this be @c (C::pi_2 @c - @c std::acos(dotProd)) ?
     */
    SubLatMod[0] = std::asin(-dotProd);  // Angle between equatorial
                                         // plane and OA.

    DVector R_b(range_b);
    MaRC::to_unit_vector(R_b);

    /**
     * @bug This value is the same as the one computed above.
     */
    // Try first possibility
    SubLatMod[0] = std::asin(-dotProd);  // Angle between equatorial
                                         // plane and OA.
    Geometry::RotX(-SubLatMod[0], range_b, rotated);
    R_b = rotated;

    double const Ztwist1 = std::atan2(R_b[0], -R_b[1]);

    // Observer to body transformation
    DMatrix o2b(Geometry::RotZMatrix(Ztwist1)
                * (Geometry::RotXMatrix(SubLatMod[0])
                   * Geometry::RotYMatrix(-this->position_angle_)));

    observ2body = o2b;

    double diff_magnitude =
        MaRC::magnitude(OA_O - o2b * UnitOpticalAxis);

    /**
     * @todo This seems like a hack.  Why do we need to check a second
     *       value?  There should only be one value with this approach
     *       since.
     */
    // Try second possibility
    SubLatMod[1] = C::pi - SubLatMod[0];
    Geometry::RotX(-SubLatMod[1], range_b, rotated);
    R_b = rotated;

    double const Ztwist2 = std::atan2(R_b[0], -R_b[1]);

    // Observer to body transformation
    o2b =
        Geometry::RotZMatrix(Ztwist2)
        * (Geometry::RotXMatrix(SubLatMod[1])
           * Geometry::RotYMatrix(-this->position_angle_));

#ifdef DEBUG
    double Ztwist, SubLatModified;
#endif  /* DEBUG */

    if (diff_magnitude > MaRC::magnitude(OA_O
                                         - o2b * UnitOpticalAxis)) {
        diff_magnitude =
            MaRC::magnitude(OA_O - o2b * UnitOpticalAxis);
        observ2body = o2b;

#ifdef DEBUG
        SubLatModified = SubLatMod[1];
        Ztwist = Ztwist2;
    } else {
        SubLatModified = SubLatMod[0];
        Ztwist = Ztwist1;
#endif  /* DEBUG */
    }

    double const percent_diff =
        diff_magnitude / MaRC::magnitude(UnitOpticalAxis);

    static constexpr double tolerance = 1e-8;

    if (percent_diff * 100 > tolerance)
        // If greater than tolerance, warn.
        std::cerr
            << '\n'
            << "WARNING: Results may be incorrect since a\n"
            << "         \"suitable\" transformation matrix was\n"
            << "         not found for the given image.\n"
            << "         There was a "
            << percent_diff * 100 << "%"<< '\n'
            << "         difference between the two test vectors.\n"
            << "         This warning occured since the percent\n"
            << "         difference between the vectors was\n"
            << "         greater than "
            << tolerance << "%.\n";

    // Body to observer transformation
    // Get reverse transformation by taking transpose since
    // transformation matrices are orthogonal.
    body2observ = MaRC::transpose(observ2body);

#ifdef DEBUG
    for (std::size_t i = 0; i < 3; ++i)
        std::cout
            << std::abs((UnitOpticalAxis[i] - (observ2body * OA_O)[i])
                        / (UnitOpticalAxis[i] + (observ2body * OA_O)[i])
                        * 2)
            << '\n';

    std::cout
        << "UnitOpticalAxis = " << UnitOpticalAxis << "\n"
           "OpticalAxis from transformation = "
        << observ2body * OA_O << "\n"
           "OA_O = " << OA_O << "\n"
           "OA_O from transformation = " << body2observ * UnitOpticalAxis
        << '\n';
//   output_ << "position_angle_ = " << position_angle_ / C::degree << " degrees (positive is CCW)\n"
//       << "SubLatModified = " << SubLatModified / C::degree << '\n'
//       << "Ztwist = " << Ztwist / C::degree << '\n';
//   test[2] = 1; // Unit vector along z-axis
//   result = body2observ * test; // test vector in camera coordinates.
//   output_ << "Computed NORAZ = " << std::atan2(-result[0], result[2]) / C::degree
//       << " degrees (positive is CCW)\n"
//       << "Computed North pole vector in camera space = " << result << '\n';
//   output_ << "observ2body = " << observ2body << '\n'
//       << "body2observ = " << body2observ << '\n';
#endif
}

void
MaRC::ViewingGeometry::arcsec_per_pixel(double a)
{
    // NOTE: range_ should be in units of kilometers at this point.
    if (a <= 0)
        throw std::invalid_argument("invalid number of arcseconds");
    else if (std::isnan(this->range_))
        throw std::logic_error("range not previously set");

    /*
      This conversion assumes that the observer-to-body range is much
      larger than the distance viewed in the image so that the small
      angle approximation, tan(theta) = theta in radians, applies.
      We end up with:

                 648000 arcseconds   kilometers in image
         theta = ----------------- * -------------------
                     pi radians             range

      Solving for kilometers in the image:

                                 pi
         kilometers in image = ------ * theta * range
                               648000

      Supplying a value of arcseconds per pixel instead of arcseconds
      in this case simply results in a value of kilometers per pixel,
      which is what we calcule below.
    */

    // pi radians per 648000 arcseconds, i.e.:
    //       C::circle / C::arcsec
    this->km_per_pixel_ = C::pi / 648000 * a * this->range_;
}

void
MaRC::ViewingGeometry::km_per_pixel(double value)
{
    if (value <= 0)
        throw std::invalid_argument("invalid kilometers per pixel");

    this->km_per_pixel_ =  value;
}

void
MaRC::ViewingGeometry::set_km_per_pixel()
{
    if (!std::isnan(this->km_per_pixel_))
        return;  // Nothing to be done.

    if (std::isnan(this->focal_length_) || std::isnan(this->scale_))
        throw std::logic_error("cannot set kilometers per pixel "
                               "without focal length and scale");

    // Focal length and scale are available.

    this->focal_length_pixels_ = this->focal_length_ * this->scale_;

    this->km_per_pixel_ =
        this->range_ /
        MaRC::hypot(this->OA_s_ - this->sample_center_,
                    this->focal_length_pixels_,
                    this->OA_l_ - this->line_center_);
}

void
MaRC::ViewingGeometry::range(double r)
{
    // Range value should be in kilometers!

    auto const min_range =
        std::min(this->body_->eq_rad(), this->body_->pol_rad());

    constexpr auto max_range =
        std::sqrt(std::numeric_limits<decltype(this->range_)>::max());

    // Check if the observer is too close or too far.
    if (r <= min_range || r >= max_range)
        throw std::invalid_argument("invalid range");

    this->range_ = r;
}

void
MaRC::ViewingGeometry::focal_length(double len)
{
    if (len <= 0)
        throw std::invalid_argument("invalid focal length");

    this->focal_length_ = len;
}

void
MaRC::ViewingGeometry::scale(double s)
{
    if (s <= 0)
        throw std::invalid_argument("invalid image scale");

    this->scale_ = s;
}

void
MaRC::ViewingGeometry::body_center(double sample, double line)
{
    this->body_center_sample(sample);
    this->body_center_line(line);
}

void
MaRC::ViewingGeometry::body_center_sample(double sample)
{
    if (std::isnan(sample))
        throw std::invalid_argument("invalid body center sample");

    this->sample_center_ = sample;
}

void
MaRC::ViewingGeometry::body_center_line(double line)
{
    if (std::isnan(line))
        throw std::invalid_argument("invalid body center line");

    this->line_center_ = line;
}

void
MaRC::ViewingGeometry::lat_lon_center(double lat, double lon)
{
    this->lat_at_center(lat);
    this->lon_at_center(lon);
}

void
MaRC::ViewingGeometry::lat_at_center(double lat)
{
    this->lat_at_center_ = MaRC::validate_latitude(lat);
}

void
MaRC::ViewingGeometry::lon_at_center(double lon)
{
    this->lon_at_center_ = MaRC::validate_longitude(lon);

    if (this->lon_at_center_ < 0)
        this->lon_at_center_ += C::_2pi; // Equivalent positive
                                         // longitude.
}

void
MaRC::ViewingGeometry::optical_axis(double sample, double line)
{
    this->optical_axis_sample(sample);
    this->optical_axis_line(line);
}

void
MaRC::ViewingGeometry::optical_axis_sample(double sample)
{
    if (std::isnan(sample))
        throw std::invalid_argument("invalid optical axis sample");

    this->OA_s_ = sample;
}

void
MaRC::ViewingGeometry::optical_axis_line(double line)
{
    if (std::isnan(line))
        throw std::invalid_argument("invalid optical axis line");

    this->OA_l_ = line;
}

void
MaRC::ViewingGeometry::emi_ang_limit(double angle)
{
    // Any emission angle beyond 90 degrees isn't visible.
    if (angle < -90 || angle > 90)
        throw std::range_error("invalid emission angle limit");

    this->mu_limit_ = std::cos(angle * C::degree);
}            

bool
MaRC::ViewingGeometry::latlon2pix(double lat,
                                  double lon,
                                  double & x,
                                  double & z) const
{
    if (!this->is_visible(lat, lon))
        return false;  // Failure

    double const radius = this->body_->centric_radius(lat);

    if (this->body_->prograde())
        lon  = this->sub_observ_lon_ - lon;
    else
        lon -= this->sub_observ_lon_;

    // Vector from center of the body to a point at the given latitude
    // and longitude on the surface of the body in the body coordinate
    // system.
    /**
     * @todo Confirm that Coord[0] and Coord[1] are not swapped,
     *       i.e. 90 degrees off in the x-y plane!
     *       @par
     *       Swapping the two prevents the below @c assert() from
     *       being triggered, but it's not yet clear if the
     *       @c normal_range_ value is incorrect or if the @c Coord
     *       vector is incorrect.
     */
    DVector Coord;
    Coord[0] =  radius * std::cos(lat) * std::sin(lon);
    Coord[1] = -radius * std::cos(lat) * std::cos(lon);
    Coord[2] =  radius * std::sin(lat);

    DVector const Obs(Coord - this->range_b_);

    // Convert to observer coordinates.
    DVector const Rotated(this->body2observ_ * Obs);

    /**
     * @todo Rotated[1] should never be larger than
     *       @c normal_range_ since we verified that the point at the
     *       given latitude and longitude is visible before getting
     *       here.  If that is a correct assumption figure out what is
     *       triggering the vector in observer coordinates to have a
     *       y-component that is larger than the @c normal_range_.
     */
    if (Rotated[1] > this->normal_range_)
        return false;  // On other side of image plane / body.

    x = Rotated[0] / Rotated[1] * this->focal_length_pixels_;
    z = Rotated[2] / Rotated[1] * this->focal_length_pixels_;

    x += this->OA_s_;
    z  = this->OA_l_ - z; // Assumes line numbers increase top to
                          // bottom.

    //std::cout << "* x = " << x << "\tz = " << z << '\n';

    // x and z are both positive if the point at the given latitude
    // and longitude is in the image.
    return x >= 0 && z >= 0;
}
