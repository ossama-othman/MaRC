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
#include <algorithm>


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

    /*
      Choose odd values for the map dimensions so that the center of
      the map falls on the center of a pixel.  This allows us to
      accurately locate the body's pole in the Polar Stereographic
      projection since MaRC maps data at the center of map pixel.  For
      example, the pixel (line, sample) = (0, 0) MaRC maps data at
      pixel coordinate (0.5, 0.5), i.e. the center of the pixel.
     */
    constexpr std::size_t samples = 51;
    constexpr std::size_t lines   = 61;

    template <typename T>
    constexpr bool is_odd(T n)
    {
        return n % 2;
    }

    static_assert(is_odd(samples) && is_odd(lines),
                  "Map dimensions should be odd for this test.");
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

    if (map.empty())
        return false;

    constexpr auto center_sample = samples / 2;
    constexpr auto center_line   = lines / 2;
    constexpr auto center_offset = center_line * samples + center_sample;
    constexpr double expected_center_data = (north_pole ? 90 : -90);

    auto const center_data =
        map[center_offset] * image->scale() + image->offset();

    /*
      Since a "latitude image" map was created, the data at the
      smaller of the map dimensions should be equal to the maximum
      configured latitude.
    */
    double max_lat_data[2];

    if (samples < lines) {
        /*
          Maximum latitude will be on the left and right side on the
          center line (as well as points on the circle with that
          diameter).
        */
        constexpr auto line = (lines / 2) * samples;
        constexpr std::size_t left  = line;
        constexpr std::size_t right = line + samples - 1;

        max_lat_data[0] = map[left]  * image->scale() + image->offset();
        max_lat_data[1] = map[right] * image->scale() + image->offset();

        // ...

    } else {
        /*
          Maximum latitude will be on the top and bottom side on the
          center sample (as well as points on the circle with that
          diameter).
        */
        constexpr auto sample = samples / 2;
        constexpr std::size_t top    = sample;
        constexpr std::size_t bottom = (lines - 1) * samples + sample;

        max_lat_data[0] = map[top]    * image->scale() + image->offset();
        max_lat_data[1] = map[bottom] * image->scale() + image->offset();

        // ...
    }

    constexpr int ulps = 2;

    return
        !map.empty()
        && MaRC::almost_equal(center_data, expected_center_data, ulps);

}

bool test_make_grid()
{
    constexpr auto lat_interval = 10;
    constexpr auto lon_interval = 10;

    auto const grid =
        projection->make_grid(samples, lines, lat_interval, lon_interval);

    auto const minmax =
        std::minmax_element(grid.begin(), grid.end());

    static constexpr auto black =
        std::numeric_limits<decltype(grid)::value_type>::lowest();

    static constexpr auto white =
        std::numeric_limits<decltype(grid)::value_type>::max();

    return
        minmax.first != minmax.second
        && *minmax.first  == black
        && *minmax.second == white;
}

bool test_distortion()
{
    // Latitude at the center of the map.
    constexpr auto latg = -90 * C::degree;

    // Scale distortion at the center of the Polar Stereogrpahic
    // projection shouble 1.
    constexpr double expected_distortion = 1;

    constexpr int ulps = 2;

    auto const distortion = projection->distortion(latg);

    return MaRC::almost_equal(expected_distortion,
                              distortion,
                              ulps);
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
