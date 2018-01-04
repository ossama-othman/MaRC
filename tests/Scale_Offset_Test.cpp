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
#include <type_traits>
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
bool test_extreme_value_scaling()
{
    /*
      These minimum and maximum values scaling will only work when
      T = double.  Technically the test will succeed when T is a
      floating point type with the number of bits greater than or
      equal to the size of double (e.g. long double) but MaRC
      currently doesn't support such floating point types.
    */
    constexpr auto minimum = std::numeric_limits<double>::lowest();
    constexpr auto maximum = std::numeric_limits<double>::max();

    return
        // T = double
        (std::is_same<T, std::remove_const<decltype(minimum)>::type>()
         && test_scaling<T>(minimum, maximum))

        // T = type other than double
        || !test_scaling<T>(minimum, maximum);
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
         && !test_scaling<T>(0, 360)) // Force full longitude range.

        // Integer types larger than 8 bits.
        || test_scaling<T>(longitude_low, longitude_high);
}


int main()
{
    // The fixed width integer types used here correspond to the
    // integer data types specified in the FITS standard.

    return
        test_extreme_value_scaling<uint8_t>()
        && test_extreme_value_scaling<int16_t>()
        && test_extreme_value_scaling<int32_t>()
        && test_extreme_value_scaling<int64_t>()
        && test_extreme_value_scaling<float>()
        && test_extreme_value_scaling<double>()
        
        && test_cosine_scaling<uint8_t>()
        && test_cosine_scaling<int16_t>()
        && test_cosine_scaling<int32_t>()
        && test_cosine_scaling<int64_t>()
        && test_cosine_scaling<float>()
        && test_cosine_scaling<double>()

        && test_latitude_scaling<uint8_t>()
        && test_latitude_scaling<int16_t>()
        && test_latitude_scaling<int32_t>()
        && test_latitude_scaling<int64_t>()
        && test_latitude_scaling<float>()
        && test_latitude_scaling<double>()

        && test_longitude_scaling<uint8_t>()
        && test_longitude_scaling<int16_t>()
        && test_longitude_scaling<int32_t>()
        && test_longitude_scaling<int64_t>()
        && test_longitude_scaling<float>()
        && test_longitude_scaling<double>()

        ? 0 : -1;
}
