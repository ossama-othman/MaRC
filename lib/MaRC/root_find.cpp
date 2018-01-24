/**
 * @file root_find.cpp
 *
 * MaRC root finding related functions
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
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

// #include <iostream>
// #include <iomanip>


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
    inline double
    first_derivative(double x, std::function<double(double)> f)
    {
        /*
          Choose a delta "h" that is approximately within the scale of
          "x", being careful not to choose a delta that is less than
          the machine accuracy "epsilon".

          This is inspired by the discussion for selecting a value of
          "h" in Section 5.7 - Numerical Derivates of the book
          "Numerical Recipes in C" by Press, Teukolsky, Vetterling and
          Flannery.
         */
        constexpr int ulps = 2;
        constexpr auto e = ulps * std::numeric_limits<double>::epsilon();
        auto const h = (x < 1 ? e : e * x);

        // Center divided difference numerical method of computing
        // the first derivative.
        return (f(x - 2 * h)
                - 8 * f(x - h) + 8 * f(x + h)
                - f(x + 2 * h))
            / (12 * h);
    }

    inline bool
    is_almost_equal(double lhs, double rhs)
    {
        constexpr int ulps = 2;

        return
            MaRC::almost_equal(lhs, rhs, ulps)
            || (MaRC::almost_zero(lhs, ulps)
                && MaRC::almost_zero(rhs, ulps));
    }

    double
    newton_raphson(double y, double x0, std::function<double(double)> f)
    {
        constexpr int max_iterations = 20;

        /**
         * @bug Division by zero if first derivative is zero.
         */
        for (int i = 0; i < max_iterations; ++i) {
            /*
              Finding a root requires an equation of the form f(x)=0.
              The equation y=f(x) is placed in the necessary form by
              subtracting y, accordingly.  Given:

                  f(x) - y = 0

              and the Newtown-Raphson approximation:

                              f(x )
                                 n
                  x    = x  - -------
                   n+1    n   f'(x )
                                  n

              we end up with:

                              f(x ) - y
                                 n
                  x    = x  - ---------
                   n+1    n    f'(x )
                                   n
             */
            double const x = x0 - (f(x0) - y) / first_derivative(x0, f);

            if (is_almost_equal(x, x0))
                return x;

            x0 = x;
        }

        return std::numeric_limits<double>::signaling_NaN();
    }
}

double
MaRC::root_find(double y, double x0, std::function<double(double)> f)
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
     *
     * @todo Rewrite this loop to take a hybrid Newton-Raphson /
     *       bisection approach as described in Section 9.4 of
     *       "Numerical Recipes in C".
     */
    double const begin = x0 - x0 * 2;
    double const end =   x0 + x0 * 2;

    x0 = begin;

    for (int count = 0; count < max_iterations; ++count) {
        x = newton_raphson(y, x0, f);

        if (!std::isnan(x))
            return x;

        /**
         * @bug This fails miserably for cases where x0 < h.  We need
         *      a better step value.  See note above.
         */
        x0 += h;

        if (x0 >= end)
            break;
    }

    throw
        std::runtime_error("INTERNAL ERROR: Root finding process "
                           "seems to be diverging.");

    return x;
}

double
MaRC::root_find(double y,
                double xl,
                double xh,
                std::function<double(double)> f)
{
    /*
      This implementation is based on the rtsafe() function found in
      Section 9.4 - Newton-Raphson Method Using Derivative of the book
      "Numerical Recipes in C" by Press, Teukolsky, Vetterling and
      Flannery.
    */
    double x0 = (xl + xh) / 2;

    double const yl = f(xl);
    double const yh = f(xh);

    // std::cout << std::setprecision(20)
    //           << "(x0, xl, xh, yl, yh, y) = ("
    //           << x0 << ", "
    //           << xl << ", "
    //           << xh << ", "
    //           << yl << ", "
    //           << yh << ", "
    //           << y << ")\n";

    if ((yl > y && yh > y) || (yl < y && yh < y))
        throw
            std::invalid_argument("Root finding brackets "
                                  "are not suitable.");

    if (is_almost_equal(yl, y))
        return xl;
    else if (is_almost_equal(yh, y))
        return xh;

    // Orient the search so that f(xl) < y.
    //
    // We are looking for the "root" at the given ordinate rather than
    // the x-axis, meaning "y" is not necessarily zero.
    if (yl > y)
        std::swap(xl, xh);

    // The "step size before last".
    double dxold = std::abs(xh - xl);

    // The last step.
    double dx = dxold;

    double y0 = f(x0);
    double df = first_derivative(x0, f);

    constexpr int max_iterations = 100;

    for (int i = 0; i < max_iterations; ++i) {
        // Bisect if Newtown-Raphson is out of range or not
        // decreasing fast enough.
        if (((x0 - xh) * df - y0 + y) * ((x0 - xl) * df - y0 + y) > 0
            || std::abs(2 * (y0 - y)) > std::abs(dxold * df)){
            dxold = dx;
            dx = (xh - xl) / 2;

            x0 = xl + dx;

            // std::cout << i << ":\t(dx, xl, x0) = ("
            //           << dx << ", "
            //           << xl << ", "
            //           << x0 << ")\n";

            if (is_almost_equal(xl, x0)) {
                // Change in root is negligible.  Newton step is
                // acceptable.  Take it.
                return x0;
            }
        } else {
            // Perform the Newtown-Raphson iteration.
            dxold = dx;
            dx = (y0 - y) / df;

            double const temp = x0;
            x0 -= dx;

            // std::cout << i << ":\t(dx, df,  temp, x0, y0, y) = ("
            //           << dx << ", "
            //           << df << ", "
            //           << temp << ", "
            //           << x0 << ", "
            //           << y0 << ", "
            //           << y << ")\n";

            if (is_almost_equal(temp, x0))
                return x0;
        }

        // Convergence criterion.
        constexpr int ulps = 2;
        if (MaRC::almost_zero(dx, ulps))
            return x0;

        y0 = f(x0);
        df = first_derivative(x0, f);

        if (y0 < y)
            xl = x0;
        else
            xh = x0;
    }

    throw
        std::runtime_error("Root finding process is diverging.");

    return std::numeric_limits<double>::signaling_NaN();
}
