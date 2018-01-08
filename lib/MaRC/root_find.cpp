/**
 * @file root_find.cpp
 *
 * MaRC root finding related functions
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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

#include "MaRC/root_find.h"
#include "MaRC/Mathematics.h"

#include <cmath>
#include <limits>
#include <stdexcept>


namespace
{
    /**
     * @brief Compute the first derivative @a f'(x).
     *
     * Compute the first derivative of the function @a f at @a x using
     * the center divided difference numerical method.
     *
     * @param[in] x Value at which the first derivative @c f'(x) will
     *              be computed.
     * @param[in] f Function whose first derivative @c f'(x) will be
     *              computed.
     *
     * @return First derivative @c f'(x).
     */
    inline double first_derivative(double x,
                                   std::function<double(double)> f)
    {
        /**
         * @todo Automate selection of the @c h step value.  See
         *       Section 5.7 in "Numerical Recipes in C".
         */
        constexpr double h = 1e-3;

        // Center divided difference numerical method of computing
        // the first derivative (VERY LOW ERROR).
        return (f(x - 2 * h)
                - 8 * f(x - h) + 8 * f(x + h)
                - f(x + 2 * h))
            / (12 * h);
    }

    double newton_raphson(double y,
                          double x0,
                          std::function<double(double)> f)
    {
        constexpr int ulps = 8;
        constexpr int max_iterations = 10;

        /**
         * @bug Division by zero if first derivative is zero.
         */
        for (int i = 0; i < max_iterations; ++i) {
            double const x = x0 + (y - f(x0)) / first_derivative(x0, f);

            if (MaRC::almost_equal(x, x0, ulps))
                return x;

            x0 = x;
        }

        return std::numeric_limits<double>::signaling_NaN();
    }
}

double
MaRC::root_find(double y,
                double x0,
                std::function<double(double)> f)
{
    double x = newton_raphson(y, x0, f);

    if (!std::isnan(x))
        return x;

    /**
     * @todo Figure out if we can come up with a better way of picking
     *       an initial guess so that we can avoid the below search
     *       altogether.
     */
    // Root not found.
    // Try an incremental search for a good initial guess since
    // original guess was not successful.
    constexpr int max_iterations = 10;

    /**
     * @todo Automate selection of the @c h step value.  See
     *       Section 5.7 in "Numerical Recipes in C".
     */
    constexpr double h = 1e-3;

    /**
     * @todo These look like bad bracket values.  They are equivalent
     *       to (-x0, 3 * x0).  That seems like an awfully large
     *       range.
     */
    double const begin = x0 - x0 * 2;
    double const end =   x0 + x0 * 2;

    x0 = begin;

    for (int count = 0; count < max_iterations; ++count) {
        x = newton_raphson(y, x0, f);

        if (!std::isnan(x))
            return x;

        x0 += h;

        if (x0 >= end)
            break;
    }

    throw
        std::runtime_error("INTERNAL ERROR: Root finding process "
                           "seems to be diverging.");

    return x;
}
