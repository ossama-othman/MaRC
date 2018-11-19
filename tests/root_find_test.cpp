/**
 * @file root_find_test.cpp
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

#include <marc/root_find.h>
#include <marc/Mathematics.h>


/**
 * @test Test the MaRC::root_find() function.
 */
bool test_root_find()
{
    constexpr int ulps = 2;

    auto f =
        [](double x)
        {
            // Quadratic equation with roots at x = { 0.5, 1 }.
            return 2 * x * x - 3 * x + 1;
        };

    // x at f(x) = 1 should be 0.
    constexpr double y = 1;

    // Root finding brackets
    constexpr double xl = 0.5;
    constexpr double xh = -0.7;

    // Root guess for non-bracketing variant of MaRC::root_find().
    constexpr double x0 = xh;

    return
        // Since the value we're looking for is zero, check for
        // "almost zero" rather than "almost equal to zero", since
        // MaRC::almost_equal() is not suitable for the latter.
        MaRC::almost_zero(MaRC::root_find(y, xl, xh, f), ulps)
        && MaRC::almost_zero(MaRC::root_find(y, x0, f), ulps);
}

/// The canonical main entry point.
int main()
{
    return test_root_find() ? 0 : -1;
}
