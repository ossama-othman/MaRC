/**
 * @file Scale_Offset_Test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/scale_and_offset.h>
#include <marc/DefaultConfiguration.h>
#include <marc/config.h>  // For NDEBUG.

#include <catch2/catch.hpp>

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
 */
TEMPLATE_TEST_CASE("Extreme value scaling",
                   "[scale and offset]",
                   uint8_t,
                   int16_t,
                   int32_t,
                   int64_t,
                   float,
                   double)
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

    if (std::is_same<TestType,
                     std::remove_const<decltype(minimum)>::type>()) {
        // TestType == double
        REQUIRE(test_scaling<TestType>(minimum, maximum));
    } else {
        // TestType != double
        REQUIRE(!test_scaling<TestType>(minimum, maximum));
    }
}

/**
 * @test Test scaling of cosine values.
 */
TEMPLATE_TEST_CASE("Cosine scaling",
                   "[scale and offset]",
                   uint8_t,
                   int16_t,
                   int32_t,
                   int64_t,
                   float,
                   double)
{
    // Cosine range is [-1, 1].
    constexpr double minimum = -1;
    constexpr double maximum =  1;

    REQUIRE(test_scaling<TestType>(minimum, maximum));
}

/**
 * @test Test scaling of latitude values.
 */
TEMPLATE_TEST_CASE("Latitude scaling",
                   "[scale and offset]",
                   uint8_t,
                   int16_t,
                   int32_t,
                   int64_t,
                   float,
                   double)
{
    using namespace MaRC::default_configuration;

    // Latitude range is [-90, 90] by default.

    REQUIRE(test_scaling<TestType>(latitude_low, latitude_high));
}

/**
 * @test Test scaling of longitude values.
 */
TEMPLATE_TEST_CASE("Longitude scaling",
                   "[scale and offset]",
                   uint8_t,
                   int16_t,
                   int32_t,
                   int64_t,
                   float,
                   double)
{
    using namespace MaRC::default_configuration;

    // Longitude range is [0, 360] by default.

    if (sizeof(TestType) == 1) {
        /*
          Expect the longitude scale and offset computation to fail
          for 8 bit integer types since it isn't possible to fit the
          entire 360 degree longitude range into the 8 bit integer
          range (e.g. [0, 360] will not fit into [0, 255]).

          NOTE: We assume that 1 byte integer types have 8 bits,
                e.g. UCHAR_WIDTH = 8.
        */
        REQUIRE(!test_scaling<TestType>(0, 360)); // Force full
                                                  // longitude range.
    } else {
        // Integer types larger than 8 bits.
        REQUIRE(test_scaling<TestType>(longitude_low, longitude_high));
    }
}
