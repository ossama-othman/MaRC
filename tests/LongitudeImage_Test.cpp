/**
 * @file LongitudeImage_Test.cpp
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

#include <MaRC/LongitudeImage.h>
#include <MaRC/DefaultConfiguration.h>
#include <MaRC/Mathematics.h>
#include <MaRC/Constants.h>

#include <memory>
#include <cstdint>
#include <cstring>


bool test_read_data(
    std::unique_ptr<MaRC::VirtualImage> const & longitude_image,
    double expected_lon,  // degrees
    double test_lon)      // radians
{
    constexpr double latitude = 72 * C::degree;  // arbitrary
    constexpr int    ulps     = 2;

    double data;  // Data will be in degrees.

    // Scale and offset used to convert map data to physical data
    // (longitudes)
    double const data_scale  = longitude_image->scale();
    double const data_offset = longitude_image->offset();

    return
        longitude_image->read_data(latitude, test_lon, data)
        && (MaRC::almost_equal(expected_lon,
                               data * data_scale + data_offset,
                               ulps)

            // Case where expected longitude is 360 and longitude
            // returned from LongitudeImage is 0.  Both are
            // equivalent.
            || (MaRC::almost_equal(expected_lon, 360.0, ulps)
                && MaRC::almost_equal(expected_lon,
                                      (data * data_scale + data_offset)
                                      + 360,
                                      ulps)));
}



template <typename T>
bool test_longitude_image()
{
    using namespace MaRC::default_configuration;

    // Scale and offset used to make physical data (longitudes) fit in
    // map data type.
    double map_scale;
    double map_offset;

    if (!MaRC::scale_and_offset<T>(longitude_low,
                                   longitude_high,
                                   map_scale,
                                   map_offset))
        return false;

    std::unique_ptr<MaRC::VirtualImage> const longitude_image =
        std::make_unique<MaRC::LongitudeImage>(map_scale, map_offset);

    // Lower and upper longitude bounds in degrees.
    // Longitude test values in radians.
    constexpr double lo_lon  = longitude_low  * C::degree;
    constexpr double hi_lon  = longitude_high * C::degree;
    constexpr double mid_lon = (hi_lon - lo_lon) / 2;

    // Test wrap-around
    constexpr double shift   = C::pi_4;  // radians
    constexpr double oob_lon = lo_lon - shift;  // out-of-bounds
    constexpr double ib_lon  = hi_lon - shift;  // in-bounds

    // Expected unit string.
    constexpr char const unit[] = "deg"; // Per FITS recommendation.

    return
        longitude_image

        && test_read_data(longitude_image, longitude_low, lo_lon)
        && test_read_data(longitude_image, longitude_high, hi_lon)
        && test_read_data(longitude_image, mid_lon / C::degree, mid_lon)
        && test_read_data(longitude_image, ib_lon  / C::degree, oob_lon)

        && std::strcmp(longitude_image->unit(), unit) == 0;
}

int main()
{
    return
        test_longitude_image<int16_t>()
        && test_longitude_image<uint32_t>()
        && test_longitude_image<float>()
        && test_longitude_image<double>()
        ? 0 : -1;
}
