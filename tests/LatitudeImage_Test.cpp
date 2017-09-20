/**
 * @file LatitudeImage_Test.cpp
 *
 * Copyright (C) 2017 Ossama Othman
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

#include <MaRC/LatitudeImage.h>
#include <MaRC/OblateSpheroid.h>
#include <MaRC/DefaultConfiguration.h>
#include <MaRC/Mathematics.h>
#include <MaRC/Constants.h>

#include <memory>
#include <cstdint>
#include <cstring>


bool test_read_data(
    std::unique_ptr<MaRC::VirtualImage> const & latitude_image,
    double expected_lat,  // degrees
    double test_lat)      // radians
{
    constexpr double longitude = -42 * C::degree;  // arbitrary
    constexpr int    ulps      = 2;

    double data;  // Data will be in degrees.

    // Scale and offset used to convert map data to physical data
    // (latitudes)
    double const data_scale  = latitude_image->scale();
    double const data_offset = latitude_image->offset();

    return
        latitude_image->read_data(test_lat, longitude, data)
        && MaRC::almost_equal(expected_lat,
                              data * data_scale + data_offset,
                              ulps);
}



template <typename T>
bool test_latitude_image()
{
    using namespace MaRC::default_configuration;

    // Scale and offset used to make physical data (latitudes) fit in
    // map data type.
    double map_scale;
    double map_offset;

    if (!MaRC::scale_and_offset<T>(latitude_low,
                                   latitude_high,
                                   map_scale,
                                   map_offset))
        return false;

    constexpr bool   prograde   = false;
    constexpr double eq_rad     = 1234567;
    constexpr double pol_rad    = eq_rad / 2;
    constexpr double flattening = -1;  // unused

    std::shared_ptr<MaRC::BodyData> body =
        std::make_shared<MaRC::OblateSpheroid>(prograde,
                                               eq_rad,
                                               pol_rad,
                                               flattening);

    /**
     * @todo Test case when bodygraphic latitudes is requested.
     */
    constexpr bool graphic_latitudes = false;

    std::unique_ptr<MaRC::VirtualImage> const latitude_image =
        std::make_unique<MaRC::LatitudeImage>(body,
                                              graphic_latitudes,
                                              map_scale,
                                              map_offset);

    // Lower and upper latitude bounds in degrees.
    // Latitude test values in radians.
    constexpr double lo_lat  = latitude_low  * C::degree;
    constexpr double hi_lat  = latitude_high * C::degree;
    constexpr double mid_lat = (hi_lat - lo_lat) / 2;

    constexpr double oob_lo_lat = -C::pi_2 - 1;  // Out-of-bounds
    constexpr double oob_hi_lat =  C::pi_2 + 1;

    // Expected unit string.
    constexpr char const unit[] = "deg"; // Per FITS recommendation.

    return
        latitude_image

        && test_read_data(latitude_image, latitude_low, lo_lat)
        && test_read_data(latitude_image, latitude_high, hi_lat)
        && test_read_data(latitude_image, mid_lat / C::degree, mid_lat)

        && !test_read_data(latitude_image,
                           oob_lo_lat / C::degree,
                           oob_lo_lat)
        && !test_read_data(latitude_image,
                           oob_hi_lat / C::degree,
                           oob_hi_lat)

        && std::strcmp(latitude_image->unit(), unit) == 0;
}

int main()
{
    return
        test_latitude_image<int16_t>()
        && test_latitude_image<uint32_t>()
        && test_latitude_image<float>()
        && test_latitude_image<double>()
        ? 0 : -1;
}
