/**
 * @file PolarStereographic_Test.cpp
 *
 * Copyright (C) 2018 Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <MaRC/PolarStereographic.h>
#include <MaRC/OblateSpheroid.h>
#include <MaRC/LatitudeImage.h>
#include <MaRC/Constants.h>
#include <MaRC/DefaultConfiguration.h>

#include <memory>
#include <cstring>


namespace
{
    constexpr bool   prograde   = true;
    constexpr double eq_rad     = 1234567;
    constexpr double pol_rad    = eq_rad / 2;

    std::shared_ptr<MaRC::OblateSpheroid> body =
        std::make_shared<MaRC::OblateSpheroid>(prograde, eq_rad, pol_rad);

    constexpr double max_lat    = 45;    // Degrees
    constexpr bool   north_pole = false;

    auto projection =
        std::make_unique<MaRC::PolarStereographic>(body,
                                                   max_lat,
                                                   north_pole);

    constexpr std::size_t samples = 50;
    constexpr std::size_t lines   = 60;
}

bool test_projection_name()
{
    static char const name[] = "Polar Stereographic";

    return std::strcmp(projection->projection_name(),
                       name) == 0;
}

bool test_make_map()
{
    /**
     * @todo Test conformal properties of Polar Stereographic
     * projection.
     */

    using namespace MaRC::default_configuration;

    using data_type = double;

    // Scale and offset used to make physical data (latitudes) fit in
    // map data type.
    double map_scale;
    double map_offset;

    if (!MaRC::scale_and_offset<data_type>(latitude_low,
                                           latitude_high,
                                           map_scale,
                                           map_offset))
        return false;

    constexpr bool graphic_latitudes = false;

    auto const image =
        std::make_unique<MaRC::LatitudeImage>(body,
                                              graphic_latitudes,
                                              map_scale,
                                              map_offset);

    constexpr auto minimum = std::numeric_limits<data_type>::lowest();
    constexpr auto maximum = std::numeric_limits<data_type>::max();

    auto const map =
        projection->template make_map<data_type>(*image,
                                                 samples,
                                                 lines,
                                                 minimum,
                                                 maximum);

    return true;
}

bool test_make_grid()
{
    constexpr auto lat_interval = 10;
    constexpr auto lon_interval = 10;

    auto const grid =
        projection->make_grid(samples, lines, lat_interval, lon_interval);

    return false;
}

bool test_distortion()
{
    constexpr auto latg = 45 * C::degree;

    auto const distortion = projection->distortion(latg);

    return false;
}

int main()
{
    return
        test_projection_name()
        && test_make_map()
        && test_make_grid()
        && test_distortion()
        ? 0 : -1;
}
