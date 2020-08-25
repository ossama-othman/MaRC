/**
 * @file Orthographic.cpp
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017-2020  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "Orthographic.h"
#include "OblateSpheroid.h"
#include "Constants.h"
#include "Geometry.h"
#include "Mathematics.h"
#include "Validate.h"
#include "Log.h"

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <limits>

#include <sys/types.h>


namespace MaRC
{
    /**
     * @class ortho_map_parameters
     *
     * @brief Map parameters specific to a chosen map size.
     *
     * While other parameters in the @c Orthographic projection
     * remain fixed for the lifetime of the @c MaRC::Orthographic
     * instance, the parameters in this class will vary depending
     * on the size of the map being generated in the @c plot_map()
     * method.
     */
    class ortho_map_parameters
    {
    public:

        /// Constructor.
        ortho_map_parameters()
            : km_per_pixel_(0)
            , sample_center_(0)
            , line_center_(0)
        {
        }

        /// Get number of kilometers per pixel in the projection.
        double km_per_pixel()  const { return this->km_per_pixel_; }

        /// Get body center sample in projection.
        double sample_center() const { return this->sample_center_; }

        /// Get body center line in projection.
        double line_center()   const { return this->line_center_; }

        /// Set number of kilometers per pixel in the projection.
        void km_per_pixel(double k)  { this->km_per_pixel_ = k; }

        /// Set body center sample in projection.
        void sample_center(double s) { this->sample_center_ = s; }

        /// Set body center line in projection.
        void line_center(double l)   { this->line_center_ = l; }

    private:

        /// The number of kilometers per pixel in the projection.
        double km_per_pixel_;

        /**
         * @brief Body center sample in projection (measured from
         *        left edge).
         */
        double sample_center_;

        /**
         * @brief Body center line in projection (measured from
         *        bottom edge).
         */
        double line_center_;

    };

    /**
     * @class ortho_grid_parameters
     *
     * @brief Map grid parameters specific to a chosen map size.
     */
    class ortho_grid_parameters
    {
    public:

        /// Constructor.
        ortho_grid_parameters(DMatrix const & body2obs,
                              double lat_interval,
                              double lon_interval,
                              ortho_map_parameters const & parameters)
            : body2obs_(body2obs)
            , lat_interval_(lat_interval)
            , lon_interval_(lon_interval)
            , parameters_(parameters)
        {
        }

        /// Get map grid latitude line interval.
        auto const & body2obs() const { return this->body2obs_; }

        /// Get map grid latitude line interval.
        auto lat_interval() const { return this->lat_interval_; }

        /// Get map grid longitude line interval.
        auto lon_interval() const { return this->lon_interval_; }

        /// Get underlying map size specific map parameters.
        auto const & parameters() const
        {
            return this->parameters_;
        }

        /// Value to be plotted on map grid.
        constexpr auto white() const
        {
            return std::numeric_limits<
                typename Orthographic::grid_type::value_type>::max();
        }

    private:

        /// Body-to-observer coordinate transformation matrix.
        DMatrix const body2obs_;

        /// Map grid latitude interval.
        double const lat_interval_;

        /// Map grid longitude interval.
        double const lon_interval_;

        /// Map parameters.
        ortho_map_parameters const & parameters_;

    };
}

// -------------------------------------------------------------------

MaRC::Orthographic::Orthographic (
    std::shared_ptr<OblateSpheroid> body,
    double sub_observ_lat,
    double sub_observ_lon,
    double PA,
    double km_per_pixel,
    OrthographicCenter const & center)
    : MapFactory()
    , body_(std::move(body))
    , sub_observ_lat_(MaRC::validate_latitude(sub_observ_lat))
    , sub_observ_lon_(MaRC::validate_longitude(sub_observ_lon))
    , PA_(MaRC::validate_position_angle(PA))
    , km_per_pixel_(-1)
    , sample_center_(std::numeric_limits<double>::signaling_NaN())
    , line_center_(std::numeric_limits<double>::signaling_NaN())
    , lat_at_center_(std::numeric_limits<double>::signaling_NaN())
    , lon_at_center_(std::numeric_limits<double>::signaling_NaN())
    , polar_(false)
{
    if (this->sub_observ_lon_ < 0)
        this->sub_observ_lon_ += C::_2pi;

    constexpr double pole = 90;
    constexpr int ulps = 4;
    if (MaRC::almost_equal(std::abs(sub_observ_lat), pole, ulps)) {
        MaRC::info("assuming POLAR ORTHOGRAPHIC projection");

        if ((sub_observ_lat > 0 && this->body_->prograde())
            || (sub_observ_lat < 0 && !this->body_->prograde())) {
            if (this->body_->prograde())
                this->PA_ = C::pi;
            else
                this->PA_ = 0;
        } else {
            if (this->body_->prograde())
                this->PA_ = 0;
            else
                this->PA_ = C::pi;
        }

        if (sub_observ_lat > 0)
            this->sub_observ_lat_ = C::pi_2;
        else
            this->sub_observ_lat_ = -C::pi_2;

        this->sub_observ_lon_ = 0;
        this->polar_ = true;
    }

    if (km_per_pixel > 0)
        this->km_per_pixel_ = km_per_pixel;

    if (center.geometry == CENTER_GIVEN) {
        this->sample_center_ = center.sample_lat_center;
        this->line_center_   = center.line_lon_center;
    } else if (center.geometry == LAT_LON_GIVEN) {
        // Latitude and Longitude at center of map given (in addition
        // to KM/pixel)
        this->lat_at_center_ =
            MaRC::validate_latitude(center.sample_lat_center);

        this->lon_at_center_ =
            MaRC::validate_longitude(center.line_lon_center);

        // Check if longitude at center (if supplied) is visible.

        //cosine =
        //  std::tan(this->body_->graphic_latitude(Lat)) *
        //  std::tan(this->sub_observ_lat_);

        double const cosine =
            std::pow(this->body_->eq_rad() / this->body_->pol_rad(), 2)
            * std::tan(this->lat_at_center_)
            * std::tan(this->sub_observ_lat_);

        if (cosine < -1) {
            throw std::invalid_argument(
                fmt::format("Desired LATITUDE ({}) at center of "
                            "image is not visible.",
                            center.sample_lat_center));
        }

        double lower = this->sub_observ_lon_ - C::pi;
        double upper = this->sub_observ_lon_ + C::pi;  // 360

        if (!this->polar_ && std::abs(cosine) <= 1) {
            // Lower boundary
            lower = this->sub_observ_lon_ - std::abs(std::acos(-cosine));

            // Upper boundary
            upper = this->sub_observ_lon_ + std::abs(std::acos(-cosine));
        }

        if (this->lon_at_center_ < lower)
            this->lon_at_center_ += C::_2pi;
        else if (this->lon_at_center_ > upper)
            this->lon_at_center_ -= C::_2pi;

        if (this->lon_at_center_ < lower
            || this->lon_at_center_ > upper) {
            throw std::invalid_argument(
                fmt::format("Desired LONGITUDE ({}) at center of "
                            "image is not visible.",
                            center.line_lon_center));
        }

        double const shift = this->sub_observ_lon_ - this->lon_at_center_;
        double const radius =
            this->body_->centric_radius(this->lat_at_center_);

        double pos[] = {
            // X
            radius * std::cos(this->lat_at_center_) * std::sin(shift),

            // Y
            -radius * std::cos(this->lat_at_center_) * std::cos(shift),

            // Z
            radius * std::sin(this->lat_at_center_)
        };

        if (!this->body_->prograde())
            pos[0] = -pos[0];

        // Centers in kilometers.
        this->sample_center_ =
            pos[0] * std::cos(this->PA_) + pos[1] * std::sin(this->PA_)
            * std::sin(-this->sub_observ_lat_) - pos[2] * std::sin(this->PA_)
            * std::cos(-this->sub_observ_lat_);

        //   YCenter = pos[1] * std::cos(-this->sub_observ_lat_) + pos[2] *
        //      std::sin(-this->sub_observ_lat_);
        // Drop the Y Center (not needed).

        this->line_center_ =
            pos[0] * std::sin(this->PA_)
            - pos[1] * std::sin(-this->sub_observ_lat_) * std::cos(this->PA_)
            + pos[2] * std::cos(-this->sub_observ_lat_) * std::cos(this->PA_);
    }
}

char const *
MaRC::Orthographic::projection_name() const
{
    return "Orthographic";
}

void
MaRC::Orthographic::plot_map(std::size_t samples,
                             std::size_t lines,
                             plot_type const & plot) const
{
    ortho_map_parameters mp;

    this->map_parameters(samples, lines, mp);

    DVector ImgCoord, Rotated;

    DMatrix const rotY(MaRC::Geometry::RotYMatrix(-this->PA_));
    DMatrix const rotX(MaRC::Geometry::RotXMatrix(this->sub_observ_lat_));

    double const a2   = this->body_->eq_rad() * this->body_->eq_rad();
    double const c2   = this->body_->pol_rad() * this->body_->pol_rad();

    /*
      Reduce cancellation due to subtraction from being catastrophic
      to benign by using the form (a-c)(a+c) instead of (a*a - c*c).
     */
    double const diff =
        (this->body_->eq_rad() - this->body_->pol_rad())
        * (this->body_->eq_rad() + this->body_->pol_rad());

    // "a" coefficient of the Quadratic Formula.
    double const CA =
        diff * std::pow(std::sin(this->sub_observ_lat_), 2) + c2;

    std::size_t offset = 0;

    for (std::size_t k = 0; k < lines; ++k) {
        double const z =
            (k + 0.5 - mp.line_center()) * mp.km_per_pixel();

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double x =
                (i + 0.5 - mp.sample_center()) * mp.km_per_pixel();
            double zz;

            if (!this->polar_) {
                ImgCoord[0] = x;
                ImgCoord[1] = 0;
                ImgCoord[2] = z;
                Rotated = rotY * ImgCoord;
                x  = Rotated[0];
                zz = Rotated[2];
            } else {
                zz = z;
            }

            double const CB =
                diff * zz * std::sin(2 * this->sub_observ_lat_);
            double const CC =
                a2 * zz * zz + c2 * x * x - a2 * c2 - diff *
                zz * zz * std::pow(std::sin(this->sub_observ_lat_), 2);

            std::pair<double, double> roots;

            if (MaRC::quadratic_roots(CA, CB, CC, roots)) {
                double y = std::min(roots.first, roots.second);
                ImgCoord[0] = x;
                ImgCoord[1] = y;
                ImgCoord[2] = zz;
                Rotated = rotX * ImgCoord;
                if (this->polar_) {
                    // Rotate about z-axis by (-this->PA_).
                    x =  Rotated[0] * std::cos(-this->PA_) +
                        Rotated[1] * std::sin(-this->PA_);

                    y = -Rotated[0] * std::sin(-this->PA_) +
                        Rotated[1] * std::cos(-this->PA_);
                } else {
                    x = Rotated[0];
                    y = Rotated[1];
                }

                zz = Rotated[2];

                double const lat = std::atan2(zz, std::hypot(x, y));

                double lon;

                if (this->body_->prograde())
                    lon = this->sub_observ_lon_ - std::atan2(-x, y) + C::pi;
                else
                    lon = this->sub_observ_lon_ + std::atan2(-x, y) - C::pi;

                plot(lat, lon, offset);
            }
        }
    }

    /**
     * @bug This is printed after each map plane plotting run, and
     *      interferes with the map progress output.  Print only once
     *      per orthographic map.
     *
     * @todo Inform the user of the kilometers per pixel, and sample and
     *       line center or latitude/longitude at the center,  if the
     *       user didn't provide those values.
     */
    MaRC::debug("Body center in ORTHOGRAPHIC projection "
                "(line, sample): ({}, {})",
                mp.line_center(),
                mp.sample_center());
}

void
MaRC::Orthographic::plot_grid(std::size_t samples,
                              std::size_t lines,
                              double lat_interval,
                              double lon_interval,
                              grid_type & grid) const
{
    ortho_map_parameters mp;

    this->map_parameters(samples, lines, mp);

    // Tranformation matrix to rotate about x than new y
    DMatrix const body2obs(Geometry::RotYMatrix(-this->PA_) *
                           Geometry::RotXMatrix(this->sub_observ_lat_));

    ortho_grid_parameters gp(body2obs, lat_interval, lon_interval, mp);

    plot_lat_lines(samples, lines, gp, grid);
    plot_lon_lines(samples, lines, gp, grid);
}

void
MaRC::Orthographic::plot_lat_lines(std::size_t samples,
                                   std::size_t lines,
                                   ortho_grid_parameters const & p,
                                   grid_type & grid) const
{
    auto const & mp = p.parameters();

    // Draw latitude lines
    for (double n = -90; n <= 90; n += p.lat_interval()) {
        auto const nn =  n * C::degree; // Convert to radians

        double low_bound, high_bound;

        constexpr int ulps = 8;

        if (n == 0
            && MaRC::almost_equal(std::abs(this->sub_observ_lat_),
                                  C::pi_2,
                                  ulps)) {
            // Polar case
            low_bound = -C::pi;
            high_bound= C::pi;
        } else {
            // General case

            // tan(graphic lat) * tan(sub observ lat)
            double const cosine =
                this->body_->eq_rad() * this->body_->eq_rad() /
                this->body_->pol_rad() / this->body_->pol_rad() *
            std::tan(nn) * std::tan(this->sub_observ_lat_);

            if (cosine >= -1 && cosine <= 1) {
                low_bound  =
                    this->sub_observ_lon_ - std::abs(std::acos(-cosine));

                high_bound =
                    this->sub_observ_lon_ + std::abs(std::acos(-cosine));
            } else if (cosine > 1) {
                low_bound  = -C::pi;
                high_bound =  C::pi;
            } else {
                continue;
            }
        }

        auto const radius = this->body_->centric_radius(nn);

        constexpr double imax = 2000;
        for (double m = 0; m < imax; ++m) {
            auto mm = m / imax * C::degree * 360;

            if (mm < low_bound)
                mm += C::_2pi;
            else if (mm > high_bound)
                mm -= C::_2pi;

            if (mm >= low_bound && mm <= high_bound) {
                if (this->body_->prograde())
                    mm = this->sub_observ_lon_ + C::pi - mm;
                else
                    mm -= C::pi - this->sub_observ_lon_;

                DVector const coord = p.body2obs() * DVector(
                     radius * std::cos(nn) * std::sin(mm),
                    -radius * std::cos(nn) * std::cos(mm),
                     radius * std::sin(nn));

                auto const x = coord[0] / mp.km_per_pixel();
                auto const z = coord[2] / mp.km_per_pixel();

                auto const i = static_cast<ssize_t>(
                    std::round(mp.sample_center() - x));
                auto const k = static_cast<ssize_t>(
                    std::round(mp.line_center() + z));

                if (i >= 0 && static_cast<std::size_t>(i) < samples
                    && k >= 0 && static_cast<std::size_t>(k) < lines) {
                    auto const index =
                        static_cast<std::size_t>(k) * samples +
                        static_cast<std::size_t>(i);

                    grid[index] = p.white();
                }
            }
        }
    }
}

void
MaRC::Orthographic::plot_lon_lines(std::size_t samples,
                                   std::size_t lines,
                                   ortho_grid_parameters const & p,
                                   grid_type & grid) const
{
    auto const & mp = p.parameters();

    for (double m = 0 + p.lon_interval();
         m <= 360;
         m += p.lon_interval()) {
        constexpr double imax = 1000;

        auto const mm = m * C::degree;

        for (double n = 0; n < imax; ++n) {
            auto mm2 = mm;
            auto const nn = (n / imax * 180 - 90) * C::degree;

            // tan (graphic lat) * tan (sub observ lat)
            double const cosine =
                this->body_->eq_rad() * this->body_->eq_rad()
                / this->body_->pol_rad() / this->body_->pol_rad()
                * std::tan(nn) * std::tan(this->sub_observ_lat_);

            double low_bound;
            double high_bound;

            if (cosine >= -1 && cosine <= 1) {
                low_bound =
                    this->sub_observ_lon_ - std::abs(std::acos(-cosine));
                high_bound =
                    this->sub_observ_lon_ + std::abs(std::acos(-cosine));
            } else if (cosine > 1) {
                low_bound  = -C::pi;
                high_bound =  C::pi;
            }
            else {
                continue;
            }

            if (mm2 < low_bound)
                mm2 += C::_2pi;
            else if (mm2 > high_bound)
                mm2 -= C::_2pi;

            if (mm2 >= low_bound && mm2 <= high_bound) {
                if (this->body_->prograde())
                    mm2 = this->sub_observ_lon_ + C::pi - mm2;
                else
                    mm2 -= C::pi + this->sub_observ_lon_;

                double const radius = this->body_->centric_radius(nn);

                DVector const coord = p.body2obs() * DVector(
                     radius * std::cos(nn) * std::sin(mm2),
                    -radius * std::cos(nn) * std::cos(mm2),
                     radius * std::sin(nn));

                auto const x = coord[0] / mp.km_per_pixel();
                auto const z = coord[2] / mp.km_per_pixel();

                auto const i = static_cast<ssize_t>(
                    std::round(mp.sample_center() - x));
                auto const k = static_cast<ssize_t>(
                    std::round(mp.line_center() + z));

                if (i >= 0 && static_cast<std::size_t>(i) < samples
                  && k >= 0 && static_cast<std::size_t>(k) < lines) {
                    auto const index =
                        static_cast<std::size_t>(k) * samples +
                        static_cast<std::size_t>(i);

                    grid[index] = p.white();
                }
            }
        }
    } // End draw longitude lines
}

void
MaRC::Orthographic::map_parameters(std::size_t samples,
                                   std::size_t lines,
                                   ortho_map_parameters & p) const
{
    double km_per_pixel  = 0;
    double sample_center = 0;
    double line_center   = 0;

    if (this->km_per_pixel_ <= 0) {
        static constexpr double MAP_FRACTION = 0.9;

        // The largest axis of the spheroid will take up at most
        // MAP_FRACTION of the smallest dimension of the map.
        km_per_pixel =
            2 * std::max(this->body_->eq_rad(), this->body_->pol_rad())
            / (MAP_FRACTION * std::min(samples, lines));
    } else {
        km_per_pixel = this->km_per_pixel_;
    }

    /**
     * @todo We really only need @c lat_at_center_ and
     *       @c lon_at_center_ here to determine if @c sample_center_
     *       and @c line_center_ is in kilometers or pixels by the
     *       time we get here.  Perhaps replace with a simple boolean
     *       value instead (e.g. @c convert_center_)?
     */
    if (!std::isnan(this->lat_at_center_)
        && !std::isnan(this->lon_at_center_)) {
        // Map latitude/longitude center given.  Convert to pixels
        // here instead of in the constructor since the number of
        // kilometers per pixel may depend on the map dimensions.
        sample_center = this->sample_center_ / km_per_pixel; // Pixels
        line_center   = this->line_center_   / km_per_pixel;

        // Shift to center of image.

        // X measured from left edge of image.
        sample_center = samples / 2.0 - sample_center;

        // Z measured from bottom edge of image.
        line_center = lines / 2.0 - line_center;
    } else if (std::isnan(this->sample_center_)
               || std::isnan(this->line_center_)) {
        // No map center given.
        sample_center = samples / 2.0;
        line_center = lines / 2.0;
    } else {
        // Map center in pixels give by user.
        sample_center = this->sample_center_;
        line_center = this->line_center_;
    }

    p.km_per_pixel(km_per_pixel);
    p.sample_center(sample_center);
    p.line_center(line_center);
}

// ------------------------------------

MaRC::OrthographicCenter::OrthographicCenter()
    : geometry(DEFAULT)
    , sample_lat_center(std::numeric_limits<double>::signaling_NaN())
    , line_lon_center(std::numeric_limits<double>::signaling_NaN())
{
}
