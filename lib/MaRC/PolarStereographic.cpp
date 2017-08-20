/**
 * @file PolarStereographic.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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

#include "MaRC/PolarStereographic.h"
#include "MaRC/Constants.h"
#include "MaRC/OblateSpheroid.h"
#include "MaRC/root_find.h"

#include <limits>
#include <cmath>


template <typename T>
MaRC::PolarStereographic<T>::PolarStereographic(
    std::shared_ptr<OblateSpheroid> body,
    double max_lat,
    bool north_pole)
    : MapFactory<T>()
    , body_(std::move(body))
    , max_lat_(std::isnan(max_lat)
               ? 0
               : (north_pole ? max_lat : -max_lat) * C::degree)
    , rho_coeff_(2 * body->eq_rad() *
                 std::pow(1 + body->first_eccentricity(),
                          -0.5 * (1 - body->first_eccentricity())) *
                std::pow(1 - body->first_eccentricity(),
                         -0.5 * (1 + body->first_eccentricity())))
    , distortion_coeff_(std::pow((1 + body->first_eccentricity()),
                                 (1 - 2 * body->first_eccentricity())) *
                        std::pow((1 - body->first_eccentricity()),
                                 (1 + 2 * body->first_eccentricity())) / 4 /
                        body->eq_rad() / body->eq_rad())
    , north_pole_(north_pole)
{
    if (!std::isnan(max_lat) && std::abs(max_lat) >= 90) {
        std::ostringstream s;
        s << "Maximum polar stereographic latitude ("
          << max_lat << ") >= 90.";

        throw std::out_of_range(s.str());
    }
}

template <typename T>
MaRC::PolarStereographic<T>::~PolarStereographic()
{
}

template <typename T>
const char *
MaRC::PolarStereographic<T>::projection_name() const
{
  static char const name[] = "Polar Stereographic (Conformal)";

  return name;
}

template <typename T>
void
MaRC::PolarStereographic<T>::plot_map(std::size_t samples,
                                      std::size_t lines,
                                      plot_type plot)
{
    std::size_t const nelem = samples * lines;

    std::size_t offset = 0;

    double const rho_max =
        this->stereo_rho(this->body_->graphic_latitude(this->max_lat_));
    double const min_dim = std::min(samples, lines);
    double const pix_conv_val = 2 * rho_max / min_dim;

    // Whether longitudes increase counter-clockwise in map.
    bool const ccw =
        ((this->north_pole_ && this->body_->prograde())
         || (!this->north_pole_ && !this->body_->prograde()));

    for (std::size_t k = 0; k < lines; ++k) {
        double const X = k + 0.5 - lines / 2.0;

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double const Y   = i + 0.5 - samples / 2.0;
            double const rho = pix_conv_val * std::hypot(Y, X);

//           if (rho > rho_max)
//             continue;

            double const old_latg =
                C::pi_2 - 2 * std::atan(rho / 2 / this->body_->eq_rad());

            double const old_rho = this->stereo_rho(old_latg);

            // bodyGRAPHIC latitude.
            double const latg =
                MaRC::root_find(rho,
                                old_rho,
                                old_latg,
                                &PolarStereographic<T>::stereo_rho,
                                this);

            // Convert to bodyCENTRIC latitude
            double const lat =
                this->body_->centric_latitude(this->north_pole_
                                              ? latg : -latg);

            double const lon =
                std::atan2((ccw ? Y : -Y), X);

            unsigned char const percent_complete =
                static_cast<unsigned char>((offset + 1) * 100 / nelem);

            plot(lat, lon, percent_complete, offset);
        }
    }
}

template <typename T>
void
MaRC::PolarStereographic<T>::plot_grid(std::size_t samples,
                                       std::size_t lines,
                                       float lat_interval,
                                       float lon_interval,
                                       grid_type & grid)
{
    static constexpr std::size_t imax = 2000;

    double const rho_max =
        this->stereo_rho(this->body_->graphic_latitude(this->max_lat_));
    double const min_dim = std::min(samples - 1, lines - 1);
    double const pix_conv_val = 2 * rho_max / min_dim;

    static constexpr auto white =
        std::numeric_limits<typename grid_type::value_type>::max();

    // Draw latitude lines
    for (float n = -90 + lat_interval; n < 90; n += lat_interval) {
        // Convert to bodygraphic latitude
        double const nn = this->body_->graphic_latitude(n * C::degree);

        for (std::size_t m = 0; m < imax; ++m) {
            double const mm =
                static_cast<double>(m) / imax * C::degree * 360;

            double const z = this->stereo_rho(nn) * std::cos(mm);
            double const x = this->stereo_rho(nn) * std::sin(mm);

//           if (z > rho_max || x > rho_max)
//             continue;

            double const k = std::round(z / pix_conv_val + lines / 2.0);
            double const i = std::round(x / pix_conv_val + samples / 2.0);

            if (i >= 0 && i < static_cast<double>(samples)
                && k >= 0 && k < static_cast<double>(lines)) {
                std::size_t const offset =
                    static_cast<std::size_t>(k) * samples;

                grid[offset + static_cast<std::size_t>(i)] = white;
            }
        }
    }

    // Draw longitude lines.
    for (float m = 360; m > 0; m -= lon_interval) {
        double const mm = m * C::degree;  // Convert to radians

        for (std::size_t n = 0; n < imax; ++n) {
            double const nn =
                static_cast<double>(n) / imax * C::degree * 360;

            double const z = this->stereo_rho(nn) * std::cos(mm);
            double const x = this->stereo_rho(nn) * std::sin(mm);

//           if (z > rho_max || x > rho_max)
//             continue;

            double const k = std::round(z / pix_conv_val + lines / 2.0);
            double const i = std::round(x / pix_conv_val + samples / 2.0);

            if (i >= 0 && i < static_cast<double>(samples)
                && k >= 0 && k < static_cast<double>(lines)) {
                std::size_t const offset =
                    static_cast<std::size_t>(k) * samples;

                grid[offset + static_cast<std::size_t>(i)] = white;
            }
        }
    }
}

template <typename T>
double
MaRC::PolarStereographic<T>::stereo_rho(double latg) const
{
    double const rho =
        this->rho_coeff_ * std::tan(C::pi_4 - latg / 2)
        * std::pow((1 + this->body_->first_eccentricity () * std::sin(latg))
                   / (1 - this->body_->first_eccentricity ()
                          * std::sin(latg)),
                   this->body_->first_eccentricity() / 2);

    return rho;
}

template <typename T>
double
MaRC::PolarStereographic<T>::distortion(double latg) const
{
    // Note that latitude is bodyGRAPHIC.
    return 1 + distortion_coeff_ * std::pow(this->stereo_rho(latg), 2.0);
}
