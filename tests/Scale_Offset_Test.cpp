/**
 * @file Scale_Offset_Test.cpp
 *
 * Copyright (C) 2017 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/scale_and_offset.h>
#include <marc/DefaultConfiguration.h>
#include <marc/config.h>  // For NDEBUG.

#include <limits>
#include <type_traits>
#include <cstdint>
#include <cassert>


/**
 * Verify that the given @a minimum and @a maximum fit within the data
 * type @a T.
 *
 * @tparam    T       Destination data type.
 * @param[in] minimum Minimum value to be potentially scaled and
 *                    offset.
 * @param[in] maximum Maximum value to be potentially scaled and
 *                    offset.
 *
 * @retval true  Test succeeded.
 * @retval false Test failed.
 */
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

/**
 * @test Test scaling of values that can't possibly fit into integer
 *       types or 32 bit floating point types without complete loss of
 *       precision.
 *
 * @tparam T Destination data type.
 */
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

/**
 * @test Test scaling of cosine values.
 *
 * @tparam T Destination data type.
 */
template <typename T>
bool test_cosine_scaling()
{
    // Cosine range is [-1, 1].
    constexpr double minimum = -1;
    constexpr double maximum =  1;

    return test_scaling<T>(minimum, maximum);
}

/**
 * @test Test scaling of latitude values.
 *
 * @tparam T Destination data type.
 */
template <typename T>
bool test_latitude_scaling()
{
    using namespace MaRC::default_configuration;

    // Latitude range is [-90, 90] by default.

    return test_scaling<T>(latitude_low, latitude_high);
}

/**
 * @test Test scaling of longitude values.
 *
 * @tparam T Destination data type.
 */
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

/// The canonical main entry point.
int main()
{
    // The fixed width integer types used here correspond to the
    // integer data types specified in the FITS standard.

    using namespace std;

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
