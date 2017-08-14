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
    /**
     * @brief Compare two floating numbers for equality.
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
    typename std::enable_if<std::is_floating_point<T>::value, bool>::type
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
     * @brief Check if floating number is almost zero.
     *
     * Determine if a floating point number is essentially zero by
     * comparing it against a small multiple of @c epsilon
     * (e.g. @c FLT_EPSILON for @c float and DBL_EPSILON for @c double).
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

     * @see The blog post "Comparing Floating Point Numbers, 2012
     *      Edition" for an additional discussion on floating point
     *      comparison:
     *      https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
     */
    template<typename T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
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
     * Solve for the roots of a quadratic equation of the form
     *       2
     *     ax  + bx + c = 0
     * using a numerically stable form of the Quadratic Formula that
     * avoids catastrophic cancellation (loss of significant digits
     * due to subtraction of two nearly equal numbers).
     *
     * @see Section 5.6: "Quadratic and Cubic Equations" in "Numerical
     *      Recipes in C", 1992, by Press, Teukolsky, Veterrling
     *      and Flannery for a discussion on how this approach works.
     *
     * @param[in]     a     Coefficient of the quadratic term.
     * @param[in]     b     Coefficient of the linear term.
     * @param[in]     c     Coefficient of the constant term.
     * @param[in,out] roots The roots of the quadratic equation are
     *                      returned through this variable.
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
            return false;  // Roots are not real.

        double const q = -(b + signum(b) * std::sqrt(discriminant)) / 2;

        roots.first  = q / a;
        roots.second = c / q;

        return true;
    }

    /**
     * @name Equality Functors and Functions
     *
     * @brief Check for equality using type appropriate methods.
     *
     * @internal This is not part of the public MaRC library API.
     */
    //@{
    /**
     * @brief Integer equality functor.
     *
     * This equality functor is meant for use when comparing integer
     * typed values.  It behaves the same way as @c std::equal_to(),
     * and exists merely to avoid specializing @c std::equal_to() for
     * floating point types.
     *
     * @internal This is not part of the public MaRC library API.
     *
     * @todo This header probably isn't the best place to put these
     *       equality functors and function.  Consider moving them to
     *       a separate header.
     */
    template <typename T>
    struct equal_to_impl
    {
        constexpr bool operator()(T lhs, T rhs) const
        {
            return lhs == rhs;
        }
    };

    /**
     * @brief @c float equality functor.
     *
     * This equality functor is meant for use when comparing @c float
     * typed values.  It differs from @c std::equal_to() in that it
     * checks if floating point values are almost equal rather than
     * strictly equal since strict comparison of equality of floating
     * point values is not reliable.
     *
     * @internal This is not part of the public MaRC library API.
     */
    template <>
    struct equal_to_impl<float>
    {
        bool operator()(float lhs, float rhs) const
        {
            constexpr int ulp = 20;

            return MaRC::almost_equal(lhs, rhs, ulp);
        }
    };

    /**
     * @brief @c double equality functor.
     *
     * This equality functor is meant for use when comparing @c double
     * typed values.  It differs from @c std::equal_to() in that it
     * checks if floating point values are almost equal rather than
     * strictly equal since strict comparison of equality of floating
     * point values is not reliable.
     *
     * @internal This is not part of the public MaRC library API.
     */
    template <>
    struct equal_to_impl<double>
    {
        bool operator()(double lhs, double rhs) const
        {
            constexpr int ulp = 20;

            return MaRC::almost_equal(lhs, rhs, ulp);
        }
    };

    /**
     * @brief @c Check for equality of two values.
     *
     * This equality comparison function check for equality of two
     * values, but takes into account if the two values are floating
     * point.  Integer values end up being compared using the basic
     * operator==(), but floating point values are compared using
     * @c MaRC::almost_equal() since strict equality comparisons of
     * floating point values are not reliable.
     *
     * This function may be used as a binary predicate argument to the
     * standard C++ algorithms, such as @c std::equal().
     *
     * @retval true  @a lhs and @a rhs are considered equal to each
     *               other.
     * @retval false @a lhs and @a rhs are considered not equal to
     *               each other.
     *
     * @internal This is not part of the public MaRC library API.
     */
    template <typename T>
    bool equal_to(T lhs, T rhs)
    {
        return MaRC::equal_to_impl<T>()(lhs, rhs);
    }
    //@}

} // MaRC


#endif  /* MARC_MATHEMATICS_H */
