/**
 * @file validate_test.cpp
 *
 * Copyright (C) 2018, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/Validate.h>
#include <marc/Mathematics.h>

#include <catch2/catch.hpp>

#include <functional>


/**
 * @brief Validate @a good and @a bad values using the given
 *        @a validate function.
 *
 * @param[in] validate Validation function.
 * @param[in] good     Good value in degrees to validate.
 * @param[in] bad      Bad  value in degrees to that should fail
 *                     validation.
 *
 * @retval true  Test succeeded.
 * @retval false Test failed.
 */
bool test_validate(std::function<double(double deg)> const & validate,
                   double good,
                   double bad)
{
    try {
        // This should throw std::invalid_argument.
        (void) validate(bad);

        return false;
    } catch (std::invalid_argument const &) {
        // Expected.  Ignore.
    } catch (...) {
        // Unexpected exception.
        return false;
    }

    double const expected  = good * C::degree;  // radians
    double const validated = validate(good);
    constexpr int ulps = 2;

    return MaRC::almost_equal(expected, validated, ulps);
}

/**
 * @test Verify that the MaRC::validate_latitude() function returns
 *       the provided good latitude in radians, and fails to validate
 *       the provided bad latitude.
 */
TEST_CASE("Latitude validation", "[validate]")
{
    // -90 <= good latitude (degrees) <= 90
    constexpr double good = 45;   // degrees
    constexpr double bad  = -100;

    REQUIRE(test_validate(MaRC::validate_latitude, good, bad));
}

/**
 * @test Verify that the MaRC::validate_longitude() function returns
 *       the provided good longitude in radians, and fails to validate
 *       the provided bad longitude.
 */
TEST_CASE("Longitude validation", "[validate]")
{
    // -360 <= good longitude (degrees) <= 360
    constexpr double good = -120;   // degrees
    constexpr double bad  = 540;

    REQUIRE(test_validate(MaRC::validate_longitude, good, bad));
}

/**
 * @test Verify that the MaRC::validate_position_angle() function
 *       returns the provided good position angle in radians, and
 *       fails to validate the provided bad position angle.
 */
TEST_CASE("Position angle validation", "[validate]")
{
    // -360 <= good position angle (degrees) <= 360
    constexpr double good = 270;   // degrees
    constexpr double bad  = -365;

    REQUIRE(test_validate(MaRC::validate_position_angle, good, bad));
}
