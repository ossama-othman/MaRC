/**
 * @file Orthographic.cpp
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017-2018  Ossama Othman
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

#include "Orthographic.h"
#include "OblateSpheroid.h"
#include "Constants.h"
#include "Geometry.h"
#include "Mathematics.h"
#include "Validate.h"
#include "Log.h"

#include <sstream>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <memory>


// -------------------------------------------------------------------

namespace
{
    struct grid_info
    {
        grid_info(std::size_t _samples,
                  std::size_t _lines,
                  MaRC::DMatrix const & _body2obs,
                  double _sample_center,
                  double _line_center,
                  double _km_per_pixel) noexcept
            : samples(_samples)
            , lines(_lines)
            , body2obs(_body2obs)
            , km_per_pixel(_km_per_pixel)
            , sample_center(_sample_center)
            , line_center(_line_center)
        {
        }

        std::size_t const samples;
        std::size_t const lines;
        MaRC::DMatrix const & body2obs;
        double const km_per_pixel;
        double const sample_center;
        double const line_center;
    };

    void plot_grid_i(grid_info const & info,
                     double lat,
                     double lon,
                     double radius,
                     MaRC::Orthographic::grid_type & grid)
    {
        // Body coordinates.
        MaRC::DVector const coord(
             radius * std::cos(lat) * std::sin(lon),
            -radius * std::cos(lat) * std::cos(lon),
             radius * std::sin(lat));

        // Observer coordinates.
        MaRC::DVector const obs(info.body2obs * coord);

        double const x = obs[0] / info.km_per_pixel;
        double const z = obs[2] / info.km_per_pixel;

        int const i =
            static_cast<int>(std::round(info.sample_center - x));
        int const k =
            static_cast<int>(std::round(info.line_center + z));

        if (i >= 0 && static_cast<std::size_t>(i) < info.samples
            && k >= 0 && static_cast<std::size_t>(k) < info.lines) {
            auto const index = k * info.samples + i;

            using value_type = MaRC::Orthographic::grid_type::value_type;

            static constexpr auto white =
                std::numeric_limits<value_type>::max();

            grid[index] = white;
        }
    }
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
    , body_(body)
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
        this->sub_observ_lon_ += C::_2pi;  // += 360

    constexpr int ulps = 4;
    if (MaRC::almost_equal(std::abs(sub_observ_lat), 90., ulps)) {
        MaRC::info("assuming POLAR ORTHOGRAPHIC projection");

        if ((sub_observ_lat > 0 && this->body_->prograde())
            || (sub_observ_lat < 0 && !this->body_->prograde())) {
            if (this->body_->prograde())
                this->PA_ = C::pi;  // 180
            else
                this->PA_ = 0;
        } else {
            if (this->body_->prograde())
                this->PA_ = 0;
            else
                this->PA_ = C::pi;  // 180
        }

        if (sub_observ_lat > 0)
            this->sub_observ_lat_ =  C::pi_2;  //  90
        else
            this->sub_observ_lat_ = -C::pi_2;  // -90

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

        if (!this->is_visible(this->lat_at_center_,
                              this->lon_at_center_)) {
            std::ostringstream s;
            s << "Desired latitude and longitude ("
              << center.sample_lat_center
              << ", "
              << center.line_lon_center
              << ") at center of image is not visible.";

            throw std::invalid_argument(s.str());
        }

        double const radius =
            this->body_->centric_radius(this->lat_at_center_);

        double shift = this->sub_observ_lon_ - this->lon_at_center_;
        if (!this->body_->prograde())
            shift = -shift;

        // Body Coordinates
        MaRC::DVector const pos(
             radius * std::cos(this->lat_at_center_) * std::sin(shift),
            -radius * std::cos(this->lat_at_center_) * std::cos(shift),
             radius * std::sin(this->lat_at_center_));

        // Centers in kilometers.
        this->sample_center_ =
              pos[0] * std::cos(this->PA_)
            + pos[1] * std::sin(this->PA_) * std::sin(-this->sub_observ_lat_)
            - pos[2] * std::sin(this->PA_) * std::cos(-this->sub_observ_lat_);

        // Drop the Y Center (not needed).
        // YCenter =
        //       pos[1] * std::cos(-this->sub_observ_lat_)
        //     + pos[2] * std::sin(-this->sub_observ_lat_);

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

bool
MaRC::Orthographic::is_visible(double latitude, double longitude) const
{
    /*
      Check if latitude and longitude is visible in the orthographic
      projection with the configured sub-observation point.

      Since an orthographic projection assumes that the observer
      is an infinite distance away, we can use the cosine of the
      incidence angle to determine if a given latitude and
      longitude is visible since the incidence angle calculation
      also assumes that the sun is an infinite distance away.  A
      cosine less than zero means that the point is on the far
      side of the body, and hence not visible.
    */
    return this->body_->mu0(this->sub_observ_lat_,
                            this->sub_observ_lon_,
                            latitude,
                            longitude) >= 0;
}

void
MaRC::Orthographic::map_parameters(std::size_t samples,
                                   std::size_t lines,
                                   double & km_per_pixel,
                                   double & sample_center,
                                   double & line_center) const
{
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
        line_center   = lines   / 2.0;
    } else {
        // Map center in pixels give by user.
        sample_center = this->sample_center_;
        line_center   = this->line_center_;
    }
}

void
MaRC::Orthographic::plot_map(std::size_t samples,
                             std::size_t lines,
                             plot_type plot) const
{
    double km_per_pixel  = std::numeric_limits<double>::signaling_NaN();
    double sample_center = std::numeric_limits<double>::signaling_NaN();
    double line_center   = std::numeric_limits<double>::signaling_NaN();

    this->map_parameters(samples,
                         lines,
                         km_per_pixel,
                         sample_center,
                         line_center);

    DVector ImgCoord, Rotated;

    DMatrix const rotY(MaRC::Geometry::RotYMatrix(-this->PA_));
    DMatrix const rotX(MaRC::Geometry::RotXMatrix(this->sub_observ_lat_));

    double const a2 = std::pow(this->body_->eq_rad(), 2);
    double const c2 = std::pow(this->body_->pol_rad(), 2);

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
        double const z = (k + 0.5 - line_center) * km_per_pixel;

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double x  = (i + 0.5 - sample_center) * km_per_pixel;
            double zz = z;

            if (!this->polar_) {
                ImgCoord[0] = x;
                ImgCoord[1] = 0;
                ImgCoord[2] = z;

                Rotated = rotY * ImgCoord;

                x  = Rotated[0];
                zz = Rotated[2];
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
                    x = Rotated[0] * std::cos(-this->PA_) +
                        Rotated[1] * std::sin(-this->PA_);

                    y = -Rotated[0] * std::sin(-this->PA_) +
                         Rotated[1] * std::cos(-this->PA_);

                    zz=  Rotated[2];
                } else {
                    x = Rotated[0];
                    y = Rotated[1];
                    zz= Rotated[2];
                }

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
                line_center,
                sample_center);
}

void
MaRC::Orthographic::plot_grid(std::size_t samples,
                              std::size_t lines,
                              double lat_interval,
                              double lon_interval,
                              grid_type & grid) const
{
    // Tranformation matrix to rotate about x than new y
    DMatrix const body2obs(Geometry::RotYMatrix(-this->PA_) *
                           Geometry::RotXMatrix(this->sub_observ_lat_));

    double km_per_pixel  = std::numeric_limits<double>::signaling_NaN();
    double sample_center = std::numeric_limits<double>::signaling_NaN();
    double line_center   = std::numeric_limits<double>::signaling_NaN();

    this->map_parameters(samples,
                         lines,
                         km_per_pixel,
                         sample_center,
                         line_center);

    grid_info const info(samples,
                         lines,
                         body2obs,
                         sample_center,
                         line_center,
                         km_per_pixel);

    // Draw latitude lines
    constexpr int lon_segments = 2000;
    constexpr double lon_segment = C::_2pi / lon_segments;
    lat_interval *= C::degree;  // Convert to radians

    for (double lat = -C::pi_2 + lat_interval;
         lat <= C::pi_2;
         lat += lat_interval) {
        double const radius = this->body_->centric_radius(lat);

        for (int m = 0; m < lon_segments; ++m) {
            double lon = m * lon_segment;

            if (!this->is_visible(lat, lon))
                continue;

            double grid_lon = this->sub_observ_lon_ + C::pi - lon;

            if (!this->body_->prograde())
                grid_lon = -grid_lon;

            plot_grid_i(info, lat, grid_lon, radius, grid);
        }
    }

    // Draw longitude lines
    constexpr int lat_segments = 1000;
    double const lat_segment = C::pi / lat_segments;
    lon_interval *= C::degree;   // Convert to radians.

    for (double lon = lon_interval;
         lon <= C::_2pi;
         lon += lon_interval) {
        for (int n = 0; n < lat_segments; ++n) {
            double const lat = n * lat_segment - C::pi_2;

            if (!this->is_visible(lat, lon))
                continue;

            double const radius = this->body_->centric_radius(lat);

            double grid_lon = this->sub_observ_lon_ + C::pi - lon;

            if (!this->body_->prograde())
                grid_lon = -grid_lon;

            plot_grid_i(info, lat, grid_lon, radius, grid);
        }
    }
}

// ------------------------------------

MaRC::OrthographicCenter::OrthographicCenter()
    : geometry(DEFAULT)
    , sample_lat_center(std::numeric_limits<double>::signaling_NaN())
    , line_lon_center(std::numeric_limits<double>::signaling_NaN())
{
}
