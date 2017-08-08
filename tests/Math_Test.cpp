/**
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

#include <MaRC/Mathematics.h>

#include <iostream>


bool test_signum()
{
    return
        // Signed intger
        MaRC::signum   ( -3L) == -1
        && MaRC::signum(  0L) ==  0
        && MaRC::signum(  5L) ==  1

        // Unsigned integer
        && MaRC::signum(  0U) ==  0
        && MaRC::signum(  2U) ==  1

        // Floating point number
        && MaRC::signum(-2.0) == -1
        && MaRC::signum( 0.0) ==  0
        && MaRC::signum( 7.0) ==  1;
}

bool test_quadratic_roots()
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

      Solve using MaRC::quadratic_roots() and confirm we get the
      expected roots.
    */
    static constexpr int const a =  1;
    static constexpr int const b =  1;
    static constexpr int const c = -6;
    static constexpr auto expected_roots = std::make_pair(-3.0, 2.0);

    std::pair<double, double> roots;

    /**
     * @todo Verify this ULP value isn't too small.
     *
     * @see The blog post "Comparing Floating Point Numbers, 2012
     *      Edition" for an additional discussion on floating point
     *      comparison:
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

int main()
{
    return
        test_signum()
        && test_quadratic_roots() ? 0 : -1;
}
