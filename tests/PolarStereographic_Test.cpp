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

    /**
     * @brief Perform linear extrapolation.
     *
     * Perform a linear extrapolation given two data points (x1, y1)
     * and (x2, y2), as well as the "x" corresponding to the "y" being
     * extrapolated.
     *
     * @return The linearly extrapolated value.
     */
    double extrapolate(double x1,
                       double x2,
                       double y1,
                       double y2,
                       double x)
    {
        return y1 + (x - x1) / (x2 - x1) * (y2 - y1);
    }

    template <typename IMAGE, typename MAP>
    double extrapolate_data(std::size_t i1,
                            std::size_t i2,
                            double x1,
                            double x2,
                            double x,
                            IMAGE const & image,
                            MAP const & map)
    {
        double const y1 = map[i1] * image.scale() + image.offset();
        double const y2 = map[i2] * image.scale() + image.offset();

        return extrapolate(x1, x2, y1, y2, x);
    }

    double percent_difference(double l, double l0)
    {
        // l0 better not be zero!
        return (l - l0) / l0 * 100;
    }
}

bool test_projection_name()
{
    static char const name[] = "Polar Stereographic";

    return std::strcmp(projection->projection_name(), name) == 0;
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

    /**
     * @todo Verify that the longitudes are correct by taking into
     *       account the body rotation and the pole at the center of
     *       the projection.
     */

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

      Approximate that latitude by linearly extrapolating from the two
      pixels closest to each of the edges along the smaller map
      dimension.  We could also reproduce the pixel conversion
      algorithm in the projection but this approach gives us a more
      independent way of confirming the integrity of the generated
      map.
    */
    double max_lat_data[2];

    if (samples < lines) {
        /*
          Maximum latitude will be on the left and right side on the
          center line (as well as points on the circle with that
          diameter).
        */
        constexpr std::size_t line = (lines / 2) * samples;

        // Left-most pair of pixels.
        constexpr std::size_t l1 = line;
        constexpr std::size_t l2 = line + 1;
        constexpr std::size_t xl = 0;  // Left edge of the projection.
        constexpr double xl1 = xl + 0.5;
        constexpr double xl2 = xl + 1.5;

        // Extrapolate the latitude at the left edge of the
        // projection.
        max_lat_data[0] =
            extrapolate_data(l1, l2, xl1, xl2, xl, *image, map);

        // Right-most pair of pixels.
        constexpr std::size_t r1 = line + samples - 1;
        constexpr std::size_t r2 = line + samples - 2;
        constexpr std::size_t xr = samples;  // Right edge of the
                                             // projection.
        constexpr double xr1 = xr - 0.5;
        constexpr double xr2 = xr - 1.5;

        // Extrapolate the latitude at the right edge of the
        // projection.
        max_lat_data[1] =
            extrapolate_data(r1, r2, xr1, xr2, xr, *image, map);

    } else {
        /*
          Maximum latitude will be on the top and bottom side on the
          center sample (as well as points on the circle with that
          diameter).
        */
        constexpr std::size_t sample = samples / 2;

        // Top-most pair of pixels
        constexpr std::size_t t1 = sample;             // Line 0
        constexpr std::size_t t2 = samples + sample;   // Line 1
        constexpr std::size_t xt = 0;  // Top edge of the projection.
        constexpr double xt1 = xt + 0.5;
        constexpr double xt2 = xt + 1.5;

        // Extrapolate the latitude at the top edge of the
        // projection.
        max_lat_data[0] =
            extrapolate_data(t1, t2, xt1, xt2, xt, *image, map);

        // Bottom-most pair of pixels.
        constexpr std::size_t b1 = (lines - 1) * samples + sample;
        constexpr std::size_t b2 = (lines - 2) * samples + sample;
        constexpr std::size_t xb = lines;  // Bottom edge of the
                                           // projection.
        constexpr double xb1 = xb - 0.5;
        constexpr double xb2 = xb - 1.5;

        // Extrapolate the latitude at the right edge of the
        // projection.
        max_lat_data[1] =
            extrapolate_data(b1, b2, xb1, xb2, xb, *image, map);
    }

    constexpr int ulps = 2;
    constexpr double pdmax = 0.08; // Maximum allowed difference in
                                   // percent.

    return
        !map.empty()
        && MaRC::almost_equal(center_data, expected_center_data, ulps)

        /*
          The linearly extrapolated maximum latitudes won't be close
          enough to max_lat to get a good result from
          MaRC::almost_equal() unless we greatly increase the ulps
          argument.  Just check if the difference between the two
          values falls within a small percentage instead.
        */
        && (percent_difference(max_lat_data[0], max_lat) < pdmax)
        && (percent_difference(max_lat_data[1], max_lat) < pdmax);
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
    constexpr double pole    = (north_pole ? 90 : -90) * C::degree;
    constexpr double equator = 0;

    // Scale distortion at the center of the Polar Stereogrpahic
    // projection should be 1.
    constexpr double map_center_distortion = 1;

    constexpr int ulps = 2;

    auto const distortion = projection->distortion(pole);

    return
        MaRC::almost_equal(map_center_distortion, distortion, ulps)

        // Distortion away from the pole at the center of the
        // projection should always be greater than one.
        && projection->distortion(equator) > map_center_distortion;
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
