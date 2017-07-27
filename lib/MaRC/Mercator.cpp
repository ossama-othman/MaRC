/**
 * @file Mercator.cpp
 *
 * Copyright (C) 1999, 2004, 2017  Ossama Othman
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

#include "MaRC/Mercator.h"
#include "MaRC/Constants.h"
#include "MaRC/OblateSpheroid.h"
#include "MaRC/root_find.h"

#include <limits>
#include <cmath>
#include <memory>


template <typename T>
MaRC::Mercator<T>::Mercator (std::unique_ptr<OblateSpheroid> body)
    : MapFactory<T>()
    ,  body_(body)
{
}

template <typename T>
MaRC::Mercator<T>::~Mercator()
{
}

template <typename T>
char const *
MaRC::Mercator<T>::projection_name (void) const
{
  static char const name[] = "Transverse Mercator (Conformal)";

  return name;
}

template <typename T>
void
MaRC::Mercator<T>::make_map_i(SourceImage const & source,
                              std::size_t samples,
                              std::size_t lines,
                              double minimum,
                              double maximum)
{
    std::size_t const nelem = samples * lines;

    std::size_t offset = 0;

    // No need to take absolute value.  Always positive.
    double const xmax = static_cast<double>(lines) / samples * C::pi;

    for (std::size_t k = 0; k < lines; ++k) {
        double const x = (k + 0.5) / lines * 2 * xmax - xmax;

        double const old_lat = -C::pi_2 + 2 * ::atan(exp(x));
        double const old_x = this->mercator_x(old_lat);

        double const latg =
            MaRC::root_find(x,
                            old_x,
                            old_lat,
                            &MaRC::Mercator<T>::mercator_x,
                            this); // bodyGRAPHIC latitude

        // Convert to bodyCENTRIC latitude
        double const lat = this->body_->centric_latitude(latg);

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double const lon = this->get_longitude(i, samples);

            unsigned char const percent_complete =
                static_cast<unsigned char> ((offset + 1) * 100 / nelem);

            this->plot(source,
                       lat,
                       lon,
                       minimum,
                       maximum,
                       percent_complete,
                       map[offset]);
        }
    }

  return map;
}

template <typename T>
std::unique_ptr<typename MaRC::Mercator<T>::grid_type>
MaRC::Mercator<T>::make_grid(unsigned int samples,
                             unsigned int lines,
                             float lat_interval,
                             float lon_interval)
{
    std::unique_ptr<grid_type> grid(
        std::make_unique<grid_type>(samples, lines));

    // No need to take absolute value.  Always positive.
    double const xmax = static_cast<double>(lines) / samples * C::pi;

    double const pix_conv_val = xmax / lines * 2;

    static const typename grid_type::data_type white =
        std::numeric_limits<typename grid_type::data_type>::max();

    // Draw latitude lines
    for (float n = -90 + lat_interval; n < 90; n += lat_interval) {
        // Convert to bodygraphic latitude
        double const nn = this->body_->graphic_latitude(n * C::degree);

        double const k =
            ::rint(this->mercator_x(nn) / pix_conv_val + lines / 2.0);

        if (k >= 0 && k < static_cast<double>(lines)) {
            std::size_t const offset =
                static_cast<std::size_t>(k) * samples;

            for (std::size_t i = 0; i < samples; ++i)
                (*grid)[offset + i] = white;
        }
    }

    // Draw longitude lines.
    for (float m = 360; m > 0; m -= lon_interval) {
        int i;

        if (this->body_->prograde ())
            i = samples - static_cast<int>(::rint(m * samples / 360.0));
        else
            i = static_cast<int> (::rint (m * samples / 360.0));

        if (i >= 0 && static_cast<unsigned int> (i) < samples) {
            for (unsigned int k = 0; k < lines; ++k)
                (*grid)(static_cast<unsigned int>(i), k) = white;
        }
    }

  return grid;
}

template <typename T>
double
MaRC::Mercator<T>::get_longitude(unsigned int i,
                                 unsigned int samples) const
{
    // Compute longitude at center of pixel.
    double lon = (i + 0.5) / samples * C::_2pi;

    // PROGRADE ----> longitudes increase to the left
    // RETROGRADE --> longitudes increase to the right

    if (this->body_->prograde ())
        lon = C::_2pi - lon;

    return lon;
}

template <typename T>
double
MaRC::Mercator<T>::mercator_x(double latg) const
{
    double const x =
        ::log(::tan(C::pi_4 + latg / 2) *
              ::pow((1 - this->body_->first_eccentricity() * ::sin(latg))
                    / (1 + this->body_->first_eccentricity() * ::sin(latg)),
                    this->body_->first_eccentricity() / 2));

    return x;
}

template <typename T>
double
MaRC::Mercator<T>::distortion(double latg) const
{
    // Note that latitude is bodyGRAPHIC.
    return
        this->body_->eq_rad()
        / this->body_->N(this->body_->centric_latitude(latg))
        / ::cos(latg);
}
