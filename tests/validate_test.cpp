/**
 * @file validate_test.cpp
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

#include <marc/Validate.h>
#include <marc/Mathematics.h>

#include <functional>


bool test_validate(std::function<double(double deg)> validate,
                   double good,
                   double bad)
{
    double const expected  = good * C::degree;  // radians
    double const validated = validate(good);

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

    constexpr int ulps = 2;

    return MaRC::almost_equal(expected, validated, ulps);
}

bool test_validate_latitude()
{
    // -90 <= good latitude (degrees) <= 90
    constexpr double good = 45;   // degrees
    constexpr double bad  = -100;

    return test_validate(MaRC::validate_latitude, good, bad);
}

bool test_validate_longitude()
{
    // -360 <= good longitude (degrees) <= 360
    constexpr double good = -120;   // degrees
    constexpr double bad  = 540;

    return test_validate(MaRC::validate_longitude, good, bad);
}

bool test_validate_position_angle()
{
    // -360 <= good position angle (degrees) <= 360
    constexpr double good = 270;   // degrees
    constexpr double bad  = -365;

    return test_validate(MaRC::validate_position_angle, good, bad);
}

int main()
{
    return
        test_validate_latitude()
        && test_validate_longitude()
        && test_validate_position_angle()
        ? 0 : -1;
}
