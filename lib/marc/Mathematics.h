//  -*- C++ -*-
/**
 * @file Mathematics.h
 *
 * Copyright (C) 2017  Ossama Othman
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
 *
 * @attention This file cannot be named @c Math.h or @c math.h since
 *            some platforms with case insensitive filesystems could
 *            end up including it instead of <math.h>.
 */

#ifndef MARC_MATHEMATICS_H
#define MARC_MATHEMATICS_H

#include <limits>
#include <type_traits>
#include <utility>
#include <cmath>


namespace MaRC
{
#if __cplusplus < 201703L
    /**
     * @brief Distance of (x,y,z) from the origin.
     *
     * Compute the distance of the point in space (@a x, @a y, @a z)
     * from the origin (0, 0, 0).  This function is implemented in
     * terms of the two-parameter @c std::hypot() to leverage its
     * ability to perform the operation without floating point
     * underflow or overflow, as well as its excellent floating point
     * error characteristics.
     *
     * @param[in] x,y,z Coordinate in space.
     *
     * @return Distance from the origin to the point in space (@a x,
     *         @a y, @a z), i.e. the equivalent of the square root of
     *         the sum of the squares of each coordinate
     *         \f$\sqrt{x^2+y^2+z^2}\f$.
     *
     * @deprecated This implementation of the three-parameter
     *             @c std::hypot() will be dropped once we start
     *             using C++17 features in %MaRC.
     */
    template <typename T>
    auto hypot(T x, T y, T z)
    {
        /*
          Implement the missing three parameter std::hypot() function
          by nesting two std::hypot() calls.  This works since:
          Given:
              std::hypot(x,y)   = sqrt(x*x + y*y)
          and:
              std::hypot(x,y,z) = sqrt(x*x + y*y + z*z)
          We have:
              std::hypot(std::hypot(x, y), z)
                  = sqrt((sqrt(x*x + y*y) * sqrt(x*x + y*y)) + z*z)
                  = sqrt(x*x + y*y + z*z) =
                  = std::hypot(x, y, z)
        */
        return std::hypot(std::hypot(x, y), z);
    }
#endif  // __cplusplus < 201703L

    using std::hypot;

    /**
     * @brief Compare two floating point numbers for equality.
     *
     * Floating point values cannot be reliably compared for equaility
     * using the typical @c operator==().  Determine if two floating
     * point numbers are almost equal using the ULP based comparison
     * approach.
     *
     * @note This function generally isn't suitable for comparison to
     *       zero.  For that it is probably better to compare against
     *       a multiple of @c std::numeric_limits<T>::epsilon().
     *
     * @note This function generally isn't suitable for comparison of
     *       two values close to zero.
     *
     * @param[in] x   First  of two floating operands being compared.
     * @param[in] y   Second of two floating operands being compared.
     * @param[in] ulp Units in the last place.  A measure of the
     *                desired precision of "equality".  The @a ulp
     *                should be chosen according to the context in
     *                which this function is called.  One @a ulp value
     *                may not be suitable for all cases.  This
     *                argument should be greater than zero.
     *
     * @return @c true @a x and @a y are essentially equal.  @c false
     *         otherwise.
     *
     * @see The @c std::numeric_limits::epsilon() discussion on
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
    typename std::enable_if_t<std::is_floating_point<T>::value, bool>
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
     * @brief Check if floating point number is almost zero.
     *
     * Determine if a floating point number is essentially zero by
     * comparing it against a small multiple of @c epsilon
     * (e.g. @c FLT_EPSILON for @c float and @c DBL_EPSILON for
     * @c double).
     *
     * The relative epsilon approach in @c almost_equal() is not
     * suitable for comparing numbers that are close to zero due
     * catastophic cancellation.  This approach compares against
     * an absolute epsilon instead.
     *
     * @param[in] x Number being compared against zero.
     * @param[in] n Multiplication factor of the @c epsilon value to
     *              be used when determining if @a x can be considered
     *              to be zero.  For example, to treat a number @a x
     *              as zero when it is within 2 epsilons, pass in
     *              @c 2 for the argument @a n.  This argument
     *              basically affects the precision of the zero
     *              check.  It should be greater than zero.
     *
     * @return @c true @a x is essentially zero.  @c false otherwise.
     *
     * @see The blog post "Comparing Floating Point Numbers, 2012
     *      Edition" for an additional discussion on floating point
     *      comparison:
     *      https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
     */
    template<typename T>
    typename std::enable_if_t<std::is_floating_point<T>::value, bool>
    almost_zero(T x, int n)
    {
        /**
         * @todo Confirm that this is a suitable approach.  Should the
         *       argument @a x also be used when calculating the
         *       multiple of epsilon, for example?
         */
        return std::abs(x) < std::numeric_limits<T>::epsilon() * n;
    }

    /**
     * @brief Return signum of a real number.
     *
     * @retval -1 if @a x < 0
     * @retval  0 if @a x = 0
     * @retval  1 if @a x > 0
     */
    template <typename T>
    int
    signum(T x)
    {
        // Iverson bracket notation of the signum function.
        return (x > T(0)) - (x < T(0));
    }

    /**
     * @brief Return sign of a real number.
     *
     * This function differs from the @c signum() function in that it
     * interprets the value zero (@a x == 0) as having a positive
     * sign, i.e. @c 1, instead of @c 0.
     *
     * @retval -1 if @a x <  0
     * @retval  1 if @a x >= 0
     */
    template <typename T>
    int
    sgn(T x)
    {
        // Iverson bracket notation of the sgn function.
        return (x >= T(0)) - (x < T(0));
    }

    /**
     * @brief Solve the quadratic formula in a numerically stable
     *        manner.
     *
     * Solve for the roots of a quadratic equation of the form \f$ax^2
     * + bx + c = 0\f$ using a numerically stable form of the
     * Quadratic Formula that avoids catastrophic cancellation (loss
     * of significant digits due to subtraction of two nearly equal
     * numbers).
     *
     * @see Section 5.6: "Quadratic and Cubic Equations" in "Numerical
     *      Recipes in C", 1992, by Press, Teukolsky, Veterrling
     *      and Flannery for a discussion on how this approach works.
     *
     * @param[in]  a     Coefficient of the quadratic term.
     * @param[in]  b     Coefficient of the linear term.
     * @param[in]  c     Coefficient of the constant term.
     * @param[out] roots The roots of the quadratic equation are
     *                   returned through this variable.
     *
     * @returns @c true if real roots were found, @c false otherwise.
     */
    inline bool
    quadratic_roots(double a,
                    double b,
                    double c,
                    std::pair<double, double> & roots)
    {
        double const discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return false;  // Roots are not real.

        /**
         * @note We do not use signum() here since @c signum(b) when
         *       @c b is zero would return @c 0, resulting in
         *       incorrect root results @c (0, &infin;) due to the
         *       square root of the discrimant term being dropped from
         *       the calculation.
         */
        double const q =
            -(b + sgn(b) * std::sqrt(discriminant)) / 2;

        roots.first  = q / a;
        roots.second = c / q;

        return true;
    }

} // MaRC


#endif  /* MARC_MATHEMATICS_H */
