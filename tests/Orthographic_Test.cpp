/**
 * @file Orthographic_Test.cpp
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

#include <marc/Orthographic.h>
#include <marc/OblateSpheroid.h>
#include <marc/LatitudeImage.h>
#include <marc/Constants.h>
#include <marc/DefaultConfiguration.h>
#include <marc/scale_and_offset.h>

#include <marc/Log.h>

#include <memory>
#include <cstring>
#include <algorithm>


namespace
{
    constexpr bool   prograde   = true;
    constexpr double eq_rad     = 1234567;
    constexpr double pol_rad    = eq_rad / 2;

    constexpr double sub_observ_lat = -14;  // degrees
    constexpr double sub_observ_lon = 160;
    constexpr double position_angle = 35;
    constexpr double km_per_pixel   = -1;  // Choose automatically.
    constexpr double lat_at_center  = 20;
    constexpr double lon_at_center  = 130;

    // Place the sub-observation point at the center of the map.
    MaRC::OrthographicCenter const center;

    std::shared_ptr<MaRC::OblateSpheroid> body =
        std::make_shared<MaRC::OblateSpheroid>(prograde, eq_rad, pol_rad);

    auto const projection =
        std::make_unique<MaRC::Orthographic>(body,
                                             sub_observ_lat,
                                             sub_observ_lon,
                                             position_angle,
                                             km_per_pixel,
                                             center);

    /*
      Choose an odd value for the map dimensions so that the center of
      the projection falls on the center of a pixel.  This allows us
      to accurately locate the sub-observation point in the
      Orthographic projection since MaRC maps data at the center of
      map pixel.  For example, for the pixel (line, sample) = (0, 0)
      MaRC maps data at pixel coordinate (0.5, 0.5), i.e. the center
      of the pixel.

      An odd value is also chosen for the map height to potentially
      reproduce a bug that caused the center line in the
      orthgrographic projection to either be blank or contain
      unexpected values.  That problem was not observed with an even
      number of lines.
     */
    constexpr std::size_t samples = 51;
    constexpr std::size_t lines   = 51;

    template <typename T>
    constexpr bool is_odd(T n) noexcept
    {
        return n % 2;
    }

    static_assert(is_odd(lines) && is_odd(samples),
                  "Map dimension should be odd for this test.");
}

/**
 * @test Test the MaRC::Orthographic::projection_name() method.
 */
bool test_projection_name()
{
    static char const name[] = "Orthographic";

    return std::strcmp(projection->projection_name(), name) == 0;
}

/**
 * @test Test the MaRC::Orthographic::make_map() method,
 *       i.e. Orthographic map projection image creation.
 */
bool test_make_map()
{
    using namespace MaRC::default_configuration;

    // Data type should be floating point or a 64 bit (or more)
    // integer.  Otherwise the equality comparison at the end of this
    // function will fail due to lack of precision.
    using data_type = float;

    static_assert(std::is_floating_point<data_type>()
                  || (std::is_integral<data_type>()
                      && sizeof(data_type) > 4),
                  "Use floating point or larger integer type.");

    // Scale and offset used to make physical data (latitudes) fit in
    // map data type.
    double scale;
    double offset;

    if (!MaRC::scale_and_offset<data_type>(latitude_low,
                                           latitude_high,
                                           scale,
                                           offset))
        return false;

    constexpr bool graphic_latitudes = false;

    auto const image =
        std::make_unique<MaRC::LatitudeImage>(body,
                                              graphic_latitudes,
                                              scale,
                                              offset);

    /**
     * @todo Verify that the longitudes are correct by taking into
     *       account the body rotation.
     */

    // Latitudes in degrees, scaled accordingly.
    auto const minimum = -90 * scale + offset;
    auto const maximum =  90 * scale + offset;

    // Ignored in the floating point data type case.
    auto const blank = std::numeric_limits<data_type>::lowest();

    MaRC::plot_info info(*image, minimum, maximum, blank);

    constexpr auto center_sample = samples / 2;
    constexpr auto center_line   = lines / 2;
    constexpr auto center_offset = center_line * samples + center_sample;

    // -----------------------------------------------------------------
    // Case 1: Sub-observeration point at center of image (the default).
    // -----------------------------------------------------------------

    auto map =
        projection->template make_map<data_type>(info, samples, lines);

    if (map.empty())
        return false;

    auto const non_blank =
        std::find_if_not(std::cbegin(map),
                         std::cend(map),
                         [blank](auto v)
                         {
                             return
                                 std::is_integral<data_type>()
                                 ? v != blank
                                 : !std::isnan(v);
                         });

    if (non_blank == std::cend(map))
        return false;  // All blank!

    auto const sub_observation_data =
        map[center_offset] * image->scale() + image->offset();

    // -----------------------------------------------------------------
    // Case 2: Latitude and longitude at center of image provided.
    // -----------------------------------------------------------------

    // Place the chosen latitude and longitue at the center of the
    // map translating the body in the projection, accordingly.
    constexpr MaRC::OrthographicCenter center2(MaRC::LAT_LON_GIVEN,
                                               lat_at_center,
                                               lon_at_center);

    auto const p =
        std::make_unique<MaRC::Orthographic>(body,
                                             sub_observ_lat,
                                             sub_observ_lon,
                                             position_angle,
                                             km_per_pixel,
                                             center2);

    map = p->template make_map<data_type>(info, samples, lines);

    auto const lat_at_center_data =
        map[center_offset] * image->scale() + image->offset();

    constexpr int ulps = 2;

    return
        !map.empty()
        && MaRC::almost_equal(sub_observation_data, sub_observ_lat, ulps)
        && MaRC::almost_equal(lat_at_center_data, lat_at_center, ulps);
}

/**
 * @test Test the MaRC::Orthogographic::make_grid() method,
 *       i.e. Orthographic projection grid image creation.
 */
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


/// The canonical main entry point.
int main()
{
    /**
     * @todo Test calculated kilometers per pixel, sample and line at
     *       the body center, and latitude and longitude at the center
     *       of the projection.
     */
    return
        test_projection_name()
        && test_make_map()
        && test_make_grid()
        ? 0 : -1;
}
