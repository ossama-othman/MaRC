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

    constexpr int ulps = 4;
    if (MaRC::almost_equal(std::abs(sub_observ_lat), 90., ulps)) {
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
            std::ostringstream s;
            s << "Desired LATITUDE (" << center.sample_lat_center
              << ") at center of image is not visible.";

            throw std::invalid_argument(s.str());
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
            std::ostringstream s;
            s << "Desired LONGITUDE (" << center.line_lon_center
              << ") at center of image is not visible.";

            throw std::invalid_argument(s.str());
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
            (k + 0.5 - line_center) * km_per_pixel;

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double x =
                (i + 0.5 - sample_center) * km_per_pixel;
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
    int i, k;
    double low_bound, high_bound, x, z;
    DVector Coord, T_Coord;
    double m, mm, mm2, n, nn, imax = 2000;

    // Tranformation matrix to rotate about x than new y
    DMatrix const body2obs(Geometry::RotYMatrix(-this->PA_) *
                           Geometry::RotXMatrix(this->sub_observ_lat_));

    static constexpr auto white =
        std::numeric_limits<typename grid_type::value_type>::max();

    double km_per_pixel  = std::numeric_limits<double>::signaling_NaN();
    double sample_center = std::numeric_limits<double>::signaling_NaN();
    double line_center   = std::numeric_limits<double>::signaling_NaN();

    this->map_parameters(samples,
                         lines,
                         km_per_pixel,
                         sample_center,
                         line_center);

    // Draw latitude lines
    for (n = -90; n <= 90; n += lat_interval) {
        nn =  n * C::degree; // Convert to radians

        if (n == 0
            && std::abs(std::abs(this->sub_observ_lat_) - C::pi_2) < 1e-6) {
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

        double const radius = this->body_->centric_radius(nn);

        for (m = 0; m < imax; ++m) {
            mm = m / imax * C::degree * 360;

            if (mm < low_bound)
                mm += C::_2pi;
            else if (mm > high_bound)
                mm -= C::_2pi;

            if (mm >= low_bound && mm <= high_bound) {
                if (this->body_->prograde())
                    mm = this->sub_observ_lon_ + C::pi - mm;
                else
                    mm -= C::pi - this->sub_observ_lon_;

                Coord[0] = radius * std::cos(nn) * std::sin(mm);
                Coord[1] =-radius * std::cos(nn) * std::cos(mm);
                Coord[2] = radius * std::sin(nn);

                T_Coord = body2obs * Coord;

                x = T_Coord[0] / km_per_pixel;
                z = T_Coord[2] / km_per_pixel;

                i = static_cast<int>(std::round(sample_center - x));
                k = static_cast<int>(std::round(line_center + z));

                if (i >= 0 && static_cast<std::size_t> (i) < samples
                    && k >= 0 && static_cast<std::size_t> (k) < lines) {
                    auto const index =
                        static_cast<std::size_t>(k) * samples +
                        static_cast<std::size_t>(i);

                    grid[index] = white;
                }
            }
        }
    } // End draw latitude lines

    // Draw longitude lines
    imax = 1000;
    for (m = 0 + lon_interval; m <= 360; m += lon_interval) {
        mm = m * C::degree;
        for (n = 0; n < imax; ++n) {
            mm2 = mm;
            nn = (n / imax * 180 - 90) * C::degree;

            // tan (graphic lat) * tan (sub observ lat)
            double const cosine =
                this->body_->eq_rad() * this->body_->eq_rad()
                / this->body_->pol_rad() / this->body_->pol_rad()
                * std::tan(nn) * std::tan(this->sub_observ_lat_);

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

                Coord[0] = radius * std::cos(nn) * std::sin(mm2);
                Coord[1] =-radius * std::cos(nn) * std::cos(mm2);
                Coord[2] = radius * std::sin(nn);

                T_Coord = body2obs * Coord;

                x = T_Coord[0] / km_per_pixel;
                z = T_Coord[2] / km_per_pixel;

                i = static_cast<int>(std::round(sample_center - x));
                k = static_cast<int>(std::round(line_center + z));

                if (i >= 0 && static_cast<std::size_t>(i) < samples
                  && k >= 0 && static_cast<std::size_t>(k) < lines) {
                    auto const index =
                        static_cast<std::size_t>(k) * samples +
                        static_cast<std::size_t>(i);

                    grid[index] = white;
                }
            }
        }
    } // End draw longitude lines
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

// ------------------------------------

MaRC::OrthographicCenter::OrthographicCenter()
    : geometry(DEFAULT)
    , sample_lat_center(std::numeric_limits<double>::signaling_NaN())
    , line_lon_center(std::numeric_limits<double>::signaling_NaN())
{
}
