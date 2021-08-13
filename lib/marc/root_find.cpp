/**
 * @file root_find.cpp
 *
 * %MaRC root finding related functions
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "root_find.h"
#include "Mathematics.h"

#ifndef NDEBUG
// # include "Log.h"
#endif

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
    inline double
    first_derivative(double x, std::function<double(double)> const & f)
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
        constexpr int epsilons = 2;
        constexpr auto e =
            epsilons * std::numeric_limits<double>::epsilon();
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
        constexpr int epsilons = 2;

        return
            MaRC::almost_equal(lhs, rhs, ulps)
            || (MaRC::almost_zero(lhs, epsilons)
                && MaRC::almost_zero(rhs, epsilons));
    }

    double
    newton_raphson(double y,
                   double x0,
                   std::function<double(double)> const & f)
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
MaRC::root_find(double y,
                double x0,
                std::function<double(double)> const & f)
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
                std::function<double(double)> const & f)
{
    /*
      This implementation is based on the rtsafe() function found in
      Section 9.4 - Newton-Raphson Method Using Derivative of the book
      "Numerical Recipes in C" by Press, Teukolsky, Vetterling and
      Flannery.
    */
    double const yl = f(xl);
    double const yh = f(xh);

    // MaRC::debug ("(xl, xh, yl, yh, y) = "
    //              "({}, {}, {}, {}, {})",
    //                xl, xh, yl, yh, y);

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

    double x0 = (xl + xh) / 2;
    double y0 = f(x0);

    constexpr int max_iterations = 100;

    for (int i = 0; i < max_iterations; ++i) {
        double const df = first_derivative(x0, f);

        // Bisect if Newtown-Raphson is out of range or not
        // decreasing fast enough.
        if (((x0 - xh) * df - y0 + y) * ((x0 - xl) * df - y0 + y) > 0
            || std::abs(2 * (y0 - y)) > std::abs(dxold * df)){
            dxold = dx;
            dx = (xh - xl) / 2;
            x0 = xl + dx;

            // MaRC::debug("{:>8}: (dx, xl, x0) = ({}, {}, {})",
            //             i, dx, xl, x0);
        } else {
            // Perform the Newtown-Raphson iteration.
            dxold = dx;
            dx = (y0 - y) / df;
            x0 -= dx;

            // MaRC::debug("{:>8}: (dx, df, x0, y0, y) = "
            //             "({}, {}, {}, {}, {})",
            //             i, dx, df, x0, y0, y);
        }

        // Convergence criterion.
        constexpr int epsilons = 2;
        if (MaRC::almost_zero(dx, epsilons))
            return x0;

        y0 = f(x0);

        if (y0 < y)
            xl = x0;
        else
            xh = x0;
    }

    throw
        std::runtime_error("Root finding process is diverging.");

    return std::numeric_limits<double>::signaling_NaN();
}
