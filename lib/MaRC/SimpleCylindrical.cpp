/**
 * @file SimpleCylindrical.cpp
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017  Ossama Othman
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

#include "MaRC/SimpleCylindrical.h"
#include "MaRC/Constants.h"
#include "MaRC/BodyData.h"

#include <limits>
#include <cmath>


template <typename T>
MaRC::SimpleCylindrical<T>::SimpleCylindrical(
    std::unique_ptr<BodyData> body,
    double lo_lat,
    double hi_lat,
    double lo_lon,
    double hi_lon,
    bool graphic_lat)
    : MapFactory<T>()
    , body_(std::move(body))
    , lo_lat_(C::pi_2)
    , hi_lat_(-C::pi_2)
    , lo_lon_(0)
    , hi_lon_(C::_2pi)
    , graphic_lat_(graphic_lat)
{
    // Set latitude range.
    // All latitudes are fed to SimpleCylindrical as CENTRIC.
    if (lo_lat >= -90 && lo_lat <= 90)
        this->lo_lat_ = lo_lat * C::degree;

    if (hi_lat >= -90 && hi_lat <= 90)
        this->hi_lat_ = hi_lat * C::degree;

    // Convert to GRAPHIC latitude if requested.
    if (graphic_lat) {
        this->lo_lat_ = this->body_->graphic_latitude(this->lo_lat_);
        this->hi_lat_ = this->body_->graphic_latitude(this->hi_lat_);
    }

    // Set longitude range
    if (lo_lon >= -360 && lo_lon <= 360)
        this->lo_lon_ = lo_lon * C::degree;

    if (hi_lon >= -360 && hi_lon <= 360)
        this->hi_lon_ = hi_lon * C::degree;

    if (this->lo_lon_ > this->hi_lon_)
        this->lo_lon_ -= C::_2pi;
}

template <typename T>
MaRC::SimpleCylindrical<T>::~SimpleCylindrical()
{
}

template <typename T>
char const *
MaRC::SimpleCylindrical<T>::projection_name() const
{
    static const char name[] = "Simple Cylindrical";

    return name;
}

template <typename T>
void
MaRC::SimpleCylindrical<T>::plot_map(SourceImage const & source,
                                     std::size_t samples,
                                     std::size_t lines,
                                     double minimum,
                                     double maximum,
                                     map_type & map)
{
    // Conversion factor -- latitudes per line
    double const cf = (this->hi_lat_ - this->lo_lat_) / lines;

    std::size_t const nelem = samples * lines;

    std::size_t offset = 0;

    for (std::size_t k = 0; k < lines; ++k) {
        // Compute latitude at center of pixel.
        double lat = (k + 0.5) * cf + this->lo_lat_;

        // Convert to CENTRIC Latitude
        if (this->graphic_lat_)
            lat = this->body_->centric_latitude(lat);

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double const lon = this->get_longitude(i, samples);

            unsigned char const percent_complete =
                static_cast<unsigned char>((offset + 1) * 100 / nelem);

            double data = 0;
            if (this->plot(source,
                           lat,
                           lon,
                           minimum,
                           maximum,
                           percent_complete,
                           data))
                map[offset] = static_cast<T>(data);
        }
    }
}

template <typename T>
void
MaRC::SimpleCylindrical<T>::plot_grid(std::size_t samples,
                                      std::size_t lines,
                                      float lat_interval,
                                      float lon_interval,
                                      grid_type & grid)
{
    // Convert back to degrees
    double const lo_lat = this->lo_lat_ /= C::degree;
    double const hi_lat = this->hi_lat_ /= C::degree;
    double const lo_lon = this->lo_lon_ /= C::degree;
    double const hi_lon = this->hi_lon_ /= C::degree;

    // Line-to-latitude ratio.
    double const lr = lines / (hi_lat - lo_lat);

    static constexpr auto white =
        std::numeric_limits<typename grid_type::data_type>::max();

    // Draw latitude lines
    for (float n = -90 + lat_interval; n < 90; n += lat_interval) {
        double const k = ::rint((n - lo_lat) * lr);

        if (k >= 0 && k < static_cast<double>(lines)) {
            std::size_t const offset =
                static_cast<std::size_t>(k) * samples;

            for (std::size_t i = 0; i < samples; ++i)
                grid[offset + i] = white;
        }
    }

    // Sample-to-longitude ratio.
    double const sr = samples / (hi_lon - lo_lon);

    // Draw longitude lines.
    for (float m = 360; m > 0; m -= lon_interval) {
        // lo_lon_2 is a work-around for lo_lon_ > hi_lon_ problems
        double lo_lon_2;

        if (m - lo_lon > 360)
            lo_lon_2 = lo_lon + 360;
        else
            lo_lon_2 = lo_lon;

        int i;

        if (this->body_->prograde())
            i = samples - static_cast<int>(::rint((m - lo_lon_2) * sr));
        else
            i = static_cast<int>(::rint((m - lo_lon_2) * sr));

        if (i >= 0 && static_cast<std::size_t>(i) < samples) {
            for (std::size_t k = 0; k < lines; ++k)
                grid[k * samples + static_cast<std::size_t>(i)] = white;
        }
    }
}

template <typename T>
inline double
MaRC::SimpleCylindrical<T>::get_longitude(std::size_t i,
                                          std::size_t samples) const
{
    double lon;

    // PROGRADE ----> longitudes increase to the left
    // RETROGRADE --> longitudes increase to the right

    // Compute longitude at center of pixel.

    if (this->body_->prograde ())
        lon = (i + 0.5) / samples * (lo_lon_ - hi_lon_) + hi_lon_;
    else
        lon = (i + 0.5) / samples * (hi_lon_ - lo_lon_) + lo_lon_;

    return lon;
}
