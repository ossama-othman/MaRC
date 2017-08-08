//  -*- C++ -*-
/**
 * @file math.h
 *
 * Copyright (C) 1996-1999, 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#ifndef MARC_MATH_H
#define MARC_MATH_H

#include <limits>
#include <type_traits>
#include <utility>
#include <cmath>


namespace MaRC
{
    /**
     * @brief Compare two floating numbers for equality.
     *
     * Determine if two floating types are almost equal using the ULP
     * based comparison approach.
     *
     * @note This function generally isn't suitable for comparison to
     *       zero.  For that it is probably better to compare against
     *       a multiple of @c std::numeric_limits<T>::epsilon().
     *
     * @param[in] x   First  of two floating operands being compared.
     * @param[in] y   Second of two floating operands being compared.
     * @param[in] ulp Units in the last place.
     *
     * @see The @c std::numeric_limits<>::epsilon() discussion on
     *      cppreference.com for the original implementation of this
     *      function:
     *      http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
     *
     * @see The blog post "Comparing Floating Point Numbers, 2012
     *      Edition" for an additional discussion on floating point
     *      comparison:
     *      https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
     */
    template<typename T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp)
    {
        /*
          The machine epsilon has to be scaled to the magnitude of the
          values used and multiplied by the desired precision in ULPs
          (units in the last place).
        */
        return
            std::abs(x-y) < std::numeric_limits<T>::epsilon()
                            * std::abs(x+y) * ulp
            // Unless the result is subnormal
            || std::abs(x-y) < std::numeric_limits<T>::min();
    }

    /**
     * Return signum of a real number.
     *
     * @retval -1 if @a x < 0
     * @retval  0 if @a x = 0
     * @retval  1 if @a x > 0
     */
    template <typename T>
    int
    signum(T x)
    {
        /**
         * Iverson bracket notation of the signum function.
         */
        return (x > T(0)) - (x < T(0));
    }

    /**
     * Solve the quadratic formula in a numerically stable manner.
     *
     * Solve the quadratic formula for a quadratic equation with the
     * the given coefficients, @a a, @a b and @a c in a way that
     * avoids catastrophic cancellation (loss of significant digits
     * due to subtraction of two nearly equal numbers).
     *
     * @see Section 5.6: "Quadratic and Cubic Equations" in "Numerical
     *      Recipes in C", 1992, by Press, Teukolsky, Veterrling
     *      and Flannery for a discussion on how this approach works.
     *
     * @returns @c true if real roots were found, @c false otherwise.
     */
    inline
    bool
    quadratic_roots(double a,
                    double b,
                    double c,
                    std::pair<double, double> & roots)
    {
        double const discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return false;  // One or both roots are not real.

        double const q = -(b + signum(b) * std::sqrt(discriminant)) / 2;

        roots.first  = q / a;
        roots.second = c / q;

        return true;
    }

} // MaRC


#endif  /* MARC_MATH_H */
