/**
 * @file root_find_test.cpp
 *
 * Copyright (C) 2017 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
