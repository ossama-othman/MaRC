/**
 * @file root_find_test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/root_find.h>
#include <marc/Mathematics.h>

#include <catch2/catch.hpp>


/**
 * @test Test the MaRC::root_find() function.
 */
namespace
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
}

/**
 * @test Test the bracketing MaRC::root_find() function.
 */
TEST_CASE("Roots are computed - bracketed", "[root find]") {
    REQUIRE(MaRC::almost_zero(MaRC::root_find(y, xl, xh, f), ulps));
}

/**
 * @test Test the non-bracketing MaRC::root_find() function.
 */
TEST_CASE("Roots are computed - non-bracketed", "[root find]") {
    REQUIRE(MaRC::almost_zero(MaRC::root_find(y, x0, f), ulps));
}
