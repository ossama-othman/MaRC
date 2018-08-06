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
#include "NullGeometricCorrection.h"
#include "Log.h"
#include "config.h"  // For NDEBUG

/**
 * @todo Should this be an equivalent MaRC include directive?
 */
#include <spdlog/fmt/ostr.h>

#include <cmath>
#include <limits>
#include <stdexcept>
#include <cassert>

#ifndef MARC_DEFAULT_GEOM_CORR_STRATEGY
# define MARC_DEFAULT_GEOM_CORR_STRATEGY MaRC::NullGeometricCorrection
#endif  /* MARC_DEFAULT_GEOM_CORR_STRATEGY */


namespace
{
    static constexpr double not_a_number =
        std::numeric_limits<double>::signaling_NaN();

#ifndef NDEBUG
    void
    dump_vectors(MaRC::DVector const & original,
                 MaRC::DVector const & rotated,
                 MaRC::DMatrix const & transformation,
                 std::string name,
                 std::string coordinate_type)
    {
        auto const computed = transformation * rotated;
        std::string const title =
            name + " Vector in " + coordinate_type + " Coordinates";

        /*
          Dump a table of the form:
          |=========================================|
          |  Range Vector in Observer Coordinates   |
          |=========================================|
          |      Original      |From Transformation |
          |-----------------------------------------|
          |            -62692.5|            -62692.5|
          |        -1.20769e+06|        -1.20769e+06|
          |             16742.4|             16742.4|
          |-----------------------------------------|
          |         1.20943e+06|         1.20943e+06|
          |-----------------------------------------|
        */
        MaRC::debug("\n"
                    "|{:=^45}|\n"
                    "|{:^45}|\n"
                    "|{:=^45}|\n"
                    "|{:^22}|{:^22}|\n"
                    "|{:-^45}|\n"
                    "|{:22}|{:22}|\n"
                    "|{:22}|{:22}|\n"
                    "|{:22}|{:22}|\n"
                    "|{:-^45}|\n"
                    "|{:22}|{:22}|\n"
                    "|{:-^45}|\n",
                    "",
                    title,
                    "",
                    "Original", "From Transformation",
                    "",
                    original[0], computed[0],
                    original[1], computed[1],
                    original[2], computed[2],
                    "",
                    original.magnitude(), computed.magnitude(),
                    "");
    }
#endif
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
    , use_terminator_(false)
    , geometric_correction_(
        std::make_unique<MARC_DEFAULT_GEOM_CORR_STRATEGY>())
{
}

void
MaRC::ViewingGeometry::geometric_correction(
    std::unique_ptr<GeometricCorrection> strategy)
{
    if (!strategy) {
        std::invalid_argument(
            "Null geometric correction strategy argument.");
    }

    this->geometric_correction_ = std::move(strategy);
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
MaRC::ViewingGeometry::finalize_setup(std::size_t samples,
                                      std::size_t lines)
{
    /**
     * @todo We really shouldn't set the optical axis like this.  The
     *       user should provide suitable values.  This is kept around
     *       for backward compatibility.
     */
    // Make sure optical axis set.
    if (std::isnan(this->OA_s_))
        this->optical_axis_sample(samples / 2.0);

    if (std::isnan(this->OA_l_))
        this->optical_axis_line(lines / 2.0);

    // All necessary image values and attributes should be set by now!

    if (std::isnan(this->km_per_pixel_))
        this->set_km_per_pixel();

    // Set Body center to observer vectors.

    /*
      Range vector in body coordinates.

      "Looking" at the near side of the body:
        - Origin at center of the body.
        - x-axis increasing from left-to-right.
        - y-axis increasing away from observer (toward far side of the
          body), aligned with the plane containing the sub-observation
          longitude and polar axis.  For example, "y" at
          sub-observation longitude on near side of a body modeled as
          an oblate spheroid would be -(equatorial radius).
        - z-axis increasing toward the North pole.
    */
    this->range_b_[0] =  0;
    this->range_b_[1] = -this->range_ * std::cos(this->sub_observ_lat_);
    this->range_b_[2] =  this->range_ * std::sin(this->sub_observ_lat_);

    if (std::isnan(this->lat_at_center_)
        || std::isnan(this->lon_at_center_)) {
        /**
         * @todo Verify that these values are consistent with the
         *       coordinate system, including directions of all axes.
         */
        /*
          Range vector in observer (camera) coordinates.

          With the camera pointed at the body:
            - Origin at center of the body.
            - x-axis increasing from left-to-right.
            - y-axis increasing away from observer (in to the image
              plane), parallel to the optical axis.
            - z-axis increasing toward the top of the image.
         */
        DVector range_O;

        range_O[0] =
            (this->OA_s_ - this->sample_center_) * this->km_per_pixel_;

        // range_O[1] will be calculated below.

        range_O[2] =
            (this->line_center_ - this->OA_l_) * this->km_per_pixel_;
        // Since line numbers increase top to bottom (e.g. VICAR images)

        double const mag = range_O.magnitude();

        // Perpendicular distance from observer to image plane.
        this->normal_range_ =
            std::sqrt(this->range_ * this->range_ - mag * mag);

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
        OA_hat.to_unit_vector();

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
    r_o.to_unit_vector();

    // Compute transformation matrices
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
        /*
          No real roots.

          Unable to find roots corresponding to sub-observation
          latitudes when calculating suitable rotation matrices to go
          between observer and body coordinates.
        */
        MaRC::error("unable to calculate suitable rotation matrices");

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
        (this->range_b_ - o2b * range_o).magnitude();

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
        (this->range_b_ - o2b * range_o).magnitude();

    if (diff_magnitude > test_diff_magnitude) {
        diff_magnitude = test_diff_magnitude;
        observ2body = o2b;
    }

    double const percent_diff =
        diff_magnitude / this->range_b_.magnitude() * 100;

    static constexpr double tolerance = 1e-8;
    if (percent_diff > tolerance) {
        /*
          Warn if greater than tolerance.

          Results may be incorrect since a "suitable" transformation
          matrix was not found for the given image.  Percent
          difference between vectors was greater than tolerance.
        */
        MaRC::warn("suitable transformation matrix "
                   "not found for given image");
        MaRC::warn("results may be incorrect");

        MaRC::debug("percent difference between test "
                    "vectors {}% is greater than {}",
                    percent_diff,
                    tolerance);
    }

    // Body to observer transformation
    // Get reverse transformation by taking transpose since
    // transformation matrices are orthogonal.
    body2observ = MaRC::transpose(observ2body);

#ifndef NDEBUG
    dump_vectors(this->range_b_, range_o, observ2body, "Range", "Body");
    dump_vectors(range_o, this->range_b_, body2observ, "Range", "Observer");

    MaRC::debug("position_angle_ = {} degrees (positive is CCW)\n"
                "SubLatMod = {}\n"
                "Ztwist = {}\n",
                this->position_angle_ / C::degree,
                SubLatMod / C::degree,
                Ztwist / C::degree);

    // North pole unit vector.
    DVector const body_north {0, 0, 1};

    // North pole vector in camera (observer) coordinates.
    DVector const camera_north(body2observ * body_north);

    MaRC::debug("computed NORAZ = {} degrees (positive is CCW)\n"
                "computed North pole vector in camera space = {}\n"
                "observ2body = {}\n"
                "body2observ = {}\n",
                std::atan2(-camera_north[0],
                           camera_north[2]) / C::degree,
                camera_north,
                observ2body,
                body2observ);
#endif  // !NDEBUG

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
     * @todo Shouldn't this be @c OA_O[1] @c = @c -OA.magnitude() ?
     */
    // OA_O is the optical axis vector in observer coordinates
    OA_O[1] = OA.magnitude(); // Magnitude of optical axis.

    DVector UnitOpticalAxis(OA); // Optical axis in body coordinates
    UnitOpticalAxis.to_unit_vector();

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
    R_b.to_unit_vector();

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

    double diff_magnitude = (OA_O - o2b * UnitOpticalAxis).magnitude();

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

#ifndef NDEBUG
    double Ztwist, SubLatModified;
#endif  /* !NDEBUG */

    double const mag = (OA_O - o2b * UnitOpticalAxis).magnitude();
    if (diff_magnitude > mag) {
        diff_magnitude = mag;
        observ2body = o2b;

#ifndef NDEBUG
        SubLatModified = SubLatMod[1];
        Ztwist = Ztwist2;
    } else {
        SubLatModified = SubLatMod[0];
        Ztwist = Ztwist1;
#endif  /* !NDEBUG */
    }

    double const percent_diff =
        diff_magnitude / UnitOpticalAxis.magnitude() * 100;

    static constexpr double tolerance = 1e-8;

    if (percent_diff > tolerance) {
        /*
          Warn if greater than tolerance.

          Results may be incorrect since a "suitable" transformation
          matrix was not found for the given image.  Percent
          difference between vectors was greater than tolerance.
        */
        MaRC::warn("suitable transformation matrix "
                   "not found for given image");
        MaRC::warn("results may be incorrect");

        MaRC::debug("percent difference between test "
                    "vectors {}% is greater than {}",
                    percent_diff,
                    tolerance);
    }

    // Body to observer transformation
    // Get reverse transformation by taking transpose since
    // transformation matrices are orthogonal.
    body2observ = MaRC::transpose(observ2body);

#ifndef NDEBUG
    dump_vectors(UnitOpticalAxis,
                 body2observ * UnitOpticalAxis,
                 observ2body,
                 "Unit Optical Axis",
                 "Body");
    dump_vectors(OA_O,
                 observ2body * OA_O,
                 body2observ,
                 "Optical Axis",
                 "Observer");

    MaRC::debug("position_angle_ = {} degrees (positive is CCW)\n"
                "SubLatModified = {}\n"
                "Ztwist = {}\n",
                position_angle_ / C::degree,
                SubLatModified / C::degree,
                Ztwist / C::degree);

    // North pole unit vector.
    DVector const body_north{0, 0, 1};

    // North pole vector in camera (observer) coordinates.
    DVector const camera_north(body2observ * body_north);

    MaRC::debug("computed NORAZ = {} degrees (positive is CCW)\n"
                "computed North pole vector in camera space = {}\n"
                "observ2body = {}\n"
                "body2observ = {}\n",
                std::atan2(-camera_north[0],
                           camera_north[2]) / C::degree,
                camera_north,
                observ2body,
                body2observ);
#endif  // !NDEBUG
}

void
MaRC::ViewingGeometry::arcsec_per_pixel(double a)
{
    /**
     * @note @c range_ should be in units of kilometers at this point.
     *
     * @todo It should not be necessary to expect the caller to set
     *       the range prior to calling this function.
     */
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

    auto const max_range =
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
        throw std::domain_error("invalid emission angle limit");

    this->mu_limit_ = std::cos(angle * C::degree);
}

void
MaRC::ViewingGeometry::use_terminator(bool u)
{
    this->use_terminator_ = u;
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
     *       Remember that the optical axis may not coincide with
     *       sub-observation point.
     *       @par
     *       UPDATE: This isn't necessarily true.  Depending the
     *       viewing angle, a point on the surface of the body could
     *       indeed be visible to the observer, and still be "behind"
     *       the image plane.  Confirm.
     */
    // if (Rotated[1] > this->normal_range_)
    //     return false;  // On other side of image plane / body.

    // Drop the "y" component since it is zero in the image plane.
    x = Rotated[0] / Rotated[1] * this->focal_length_pixels_;
    z = Rotated[2] / Rotated[1] * this->focal_length_pixels_;

    // Convert from object space to image space.
    this->geometric_correction_->object_to_image(z, x);

    x += this->OA_s_;
    z  = this->OA_l_ - z; // Assumes line numbers increase top to
                          // bottom.
    return true;
}

bool
MaRC::ViewingGeometry::pix2latlon(double sample,
                                  double line,
                                  double & lat,
                                  double & lon) const
{
    sample -= this->sample_center_;
    line    = this->line_center_ - line; // Negative since line
                                         // numbers increase top to
                                         // bottom (?)

    // Convert from image space to object space.
    this->geometric_correction_->image_to_object(sample, line);

    // ---------------------------------------------

    // Convert from observer coordinates to body coordinates
    DVector coord;
    coord[0] = sample;
    coord[1] = 0;
    coord[2] = line;

    // Do the transformation
    DVector rotated(this->observ2body_ * coord);
    rotated *= this->km_per_pixel_;

    // ---------------------------------------------

    // Vector from observer to point on image
    DVector const dVec(rotated - this->range_b_);

    int const success =
        this->body_->ellipse_intersection(this->range_b_,
                                          dVec,
                                          lat,
                                          lon);

    assert(success != -1);  // -1 == bad parameters

    if (success == 0) {
        /**
         * @todo Verify!
         */
        if (this->body_->prograde())
            lon  = this->sub_observ_lon_ - lon;
        else
            lon -= this->sub_observ_lon_;
    }

    return success == 0;
}

std::vector<bool>
MaRC::ViewingGeometry::body_mask(std::size_t samples,
                                 std::size_t lines) const
{
    /**
     * @todo This routine is currently oblate spheroid specific.
     *
     * @todo Find a way to make the scan more efficient, or avoid it
     *       entirely.
     */

    std::vector<bool> mask(samples * lines, false);

    for (std::size_t k = 0; k < lines; ++k) {
        std::size_t const offset = k * samples;

        for (std::size_t i = 0; i < samples; ++i) {
            double lat, lon;

            if (this->pix2latlon(i, k, lat, lon)) {
                // On body
                std::size_t const index = offset + i;
                mask[index] = true;
            }
        }
    }

    return mask;
}
