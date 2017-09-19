/**
 * @file Scale_Offset_Test.cpp
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

#include <MaRC/VirtualImage.h>
#include <MaRC/DefaultConfiguration.h>

#include <limits>
#include <cstdint>
#include <cassert>


template <typename T>
bool test_scaling(double minimum, double maximum)
{
    assert(minimum < maximum);

    constexpr double T_lowest = std::numeric_limits<T>::lowest();
    constexpr double T_max    = std::numeric_limits<T>::max();
    
    double scale;
    double offset;

    return
        MaRC::scale_and_offset<T>(minimum, maximum, scale, offset)
        && (minimum * scale + offset >= T_lowest)
        && (maximum * scale + offset <= T_max);
}

template <typename T>
bool test_cosine_scaling()
{
    // Cosine range is [-1, 1].
    constexpr double minimum = -1;
    constexpr double maximum =  1;

    return test_scaling<T>(minimum, maximum);
}

template <typename T>
bool test_latitude_scaling()
{
    using namespace MaRC::default_configuration;

    // Latitude range is [-90, 90] by default.

    return test_scaling<T>(latitude_low, latitude_high);
}

template <typename T>
bool test_longitude_scaling()
{
    using namespace MaRC::default_configuration;

    // Longitude range is [0, 360] by default.

    return

        /*
          Expect the longitude scale and offset computation to fail
          for 8 bit integer types since it isn't possible to fit the
          entire 360 degree longitude range into the 8 bit integer
          range (e.g. [0, 360] will not fit into [0, 255]).

          NOTE: We assume that 1 byte integer types have 8 bits,
                e.g. UCHAR_WIDTH = 8.
        */
        (sizeof(T) == 1
         && !test_scaling<T>(longitude_low, longitude_high))

        // Integer types larger than 8 bits.
        || test_scaling<T>(longitude_low, longitude_high);
}


int main()
{
    // The fixed width integer types used here correspond to the
    // integer data types specified in the FITS standard.

    return
        test_cosine_scaling<uint8_t>()
        && test_cosine_scaling<int16_t>()
        && test_cosine_scaling<int32_t>()
        && test_cosine_scaling<int64_t>()

        && test_latitude_scaling<uint8_t>()
        && test_latitude_scaling<int16_t>()
        && test_latitude_scaling<int32_t>()
        && test_latitude_scaling<int64_t>()

        && test_longitude_scaling<uint8_t>()
        && test_longitude_scaling<int16_t>()
        && test_longitude_scaling<int32_t>()
        && test_longitude_scaling<int64_t>()

        ? 0 : -1;
}
