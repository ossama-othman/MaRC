/**
 * @file Math_Test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/Mathematics.h>

#include <catch2/catch.hpp>

#include <cstdint>


namespace
{
    /**
     * @brief Calculate and verify quadratic roots.
     *
     * @param[in] a              "a" coefficient of a quadratic
     *                           equation.
     * @param[in] b              "b" coefficient of a quadratic
     *                           equation.
     * @param[in] c              "c" coefficient of a quadratic
     *                           equation.
     * @param[in] expected_roots Quadratic roots that should be
     *                           returned from
     *                           MaRC::quadratic_roots().
     *
     * @return @c true on successful quadratic root validation, and
     *         @c false otherwise
     */
    bool check_roots(double a,
                     double b,
                     double c,
                     std::pair<double, double> expected_roots)
    {
        std::pair<double, double> roots;

        /**
         * @todo Verify this ULP value isn't too small.
         *
         * @see The blog post "Comparing Floating Point Numbers, 2012
         *      Edition" for an additional discussion on floating
         *      point comparison:
         *      https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
         */
        static constexpr int ulp = 1;  // Units in the last place.

        return
            MaRC::quadratic_roots(a, b, c, roots) // found real roots
            && ((MaRC::almost_equal(roots.first,
                                    expected_roots.first,
                                    ulp)
                 && MaRC::almost_equal(roots.second,
                                       expected_roots.second,
                                       ulp))
                || (MaRC::almost_equal(roots.first,
                                       expected_roots.second,
                                       ulp)
                    && MaRC::almost_equal(roots.second,
                                          expected_roots.first,
                                          ulp)));
    }
}

/**
 * @test Test the MaRC::almost_equal() function.
 */
TEST_CASE("MaRC::almost_equal()", "[math]")
{
    using namespace std;

    static_assert(sizeof(float) == sizeof(int32_t),
                  "Cannot run almost_equal() with different size types.");

    /**
     * @todo Verify claims about ULPs below.
     */

    // Start out with an arbitrary floating point value.
    float x = 12345.6789;

    // Interpret its memory as memory belonging to an integer of the
    // same size.  We are casting the pointer to float to a pointer to
    // int32_t.  We are NOT casting the float value to an int32_t.
    auto const xi = reinterpret_cast<int32_t *>(&x);

    // Add 4 Units in the Last Place (ULPs) to the integer
    // representation.  This is NOT the same as adding 4 to the
    // floating point value x.
    auto yi = *xi + 4;

    // Interpret the memory of the new integer as memory belonging to
    // a float of the same size.  We are casting the pointer to
    // int32_t to a pointer to float.  We are NOT casting the int32_t
    // value to a float.
    auto const y  = reinterpret_cast<float *>(&yi);

    // The floating values (x) and (*y) should now be 4 ULPs
    // apart. Test that they are still almost equal to each other.
    REQUIRE(MaRC::almost_equal(x, *y, 2));

    // Increase precision to make the "almost equal" check more
    // strict.
    REQUIRE(!MaRC::almost_equal(x, *y, 1));

    // Check for inequality with relatively tight precision.
    REQUIRE(!MaRC::almost_equal(3.1234567891234, 3.12345678912341, 2));
}

/**
 * @test Test the MaRC::almost_zero() function.
 */
TEST_CASE("MaRC::almost_zero()", "[math]")
{
    constexpr float  w = 0;
    constexpr double x = std::numeric_limits<double>::min();
    constexpr float  y = -1e-5;
    constexpr float  z = 1;

    REQUIRE(MaRC::almost_zero(w, 1));
    REQUIRE(MaRC::almost_zero(x, 1));
    REQUIRE(MaRC::almost_zero(y, 100));
    REQUIRE(!MaRC::almost_zero(z, 100000));
}

/**
 * @test Test the MaRC::signum() function.
 */
TEST_CASE("MaRC::signum()", "[math]")
{
    // Signed intger
    REQUIRE(MaRC::signum(-3L) == -1);
    REQUIRE(MaRC::signum( 0L) ==  0);
    REQUIRE(MaRC::signum( 5L) ==  1);

    // Unsigned integer
    REQUIRE(MaRC::signum(0U) ==  0);
    REQUIRE(MaRC::signum(2U) ==  1);

    // Floating point number
    REQUIRE(MaRC::signum(-2.0) == -1);
    REQUIRE(MaRC::signum( 0.0) ==  0);
    REQUIRE(MaRC::signum( 7.0) ==  1);
}

/**
 * @test Test the MaRC::sgn() function.
 */
TEST_CASE("MaRC::sgn()", "[math]")
{
    // Signed intger
    REQUIRE(MaRC::sgn(-3L) == -1);
    REQUIRE(MaRC::sgn( 0L) ==  1);
    REQUIRE(MaRC::sgn( 5L) ==  1);

    // Unsigned integer
    REQUIRE(MaRC::sgn(0U) ==  1);
    REQUIRE(MaRC::sgn(2U) ==  1);

    // Floating point number
    REQUIRE(MaRC::sgn(-2.0) == -1);
    REQUIRE(MaRC::sgn( 0.0) ==  1);
    REQUIRE(MaRC::sgn( 7.0) ==  1);
}

/**
 * @test Test the MaRC::quadratic_roots() function.
 */
TEST_CASE("MaRC::quadratic_roots()", "[math]")
{
    /*
      For a quadratic equation with roots of (-3, 2) we have the
      following:

          (x + 3)(x - 2) = 0

      which in its polynomial form is:

            2
           x  + x - 6 = 0

      where its polynomial coefficients are:

           a =  1
           b =  1
           c = -6

      Similarly, for a quadratic equation with roots (-2.5, 2.5) we
      could have the following:

          (2x + 5)(2x - 5) = 0

      which in its polynomial form is:

              2
            4x  - 25 = 0

      where its polynomial coefficients are:

           a =   4
           b =   0
           c = -25

      Solve both equations using MaRC::quadratic_roots(), and confirm
      we get the expected roots.
    */
    static constexpr int const a1 =  1;
    static constexpr int const b1 =  1;
    static constexpr int const c1 = -6;
    static constexpr auto expected_roots1 = std::make_pair(-3.0, 2.0);

    static constexpr int const a2 =  4;
    static constexpr int const b2 =  0;
    static constexpr int const c2 = -25;
    static constexpr auto expected_roots2 = std::make_pair(-2.5, 2.5);

    REQUIRE(check_roots(a1, b1, c1, expected_roots1));
    REQUIRE(check_roots(a2, b2, c2, expected_roots2));
}
