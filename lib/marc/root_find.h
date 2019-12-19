//   -*- C++ -*-
/**
 * @file root_find.h
 *
 * %MaRC root finding related functions
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_ROOT_FIND_H
#define MARC_ROOT_FIND_H

#include <marc/Export.h>

#include <functional>


namespace MaRC
{
    /**
     * @brief Find root of a given equation with an initial guess.
     *
     * Given a function y=f(x), find the value of "x" at "y" with an
     * initial guess @a x0.
     *
     * @note The current implementation uses the Newton-Raphson method
     *       to find roots.
     *
     * @note Do not place the equation in the form f(x)=0.  This
     *       implementation already does that by subtracting the known
     *       result @a y from f(x), i.e. f(x) - y = 0, when finding
     *       the root.
     *
     * @param[in] y  Known result of @a f(x).
     * @param[in] x0 Initial guess from which to start the root
     *               finding process.
     * @param[in] f  Function @a f(x) for which @c x will be
     *               computed.
     *
     * @throw std::runtime_error Root finding process is diverging.
     *
     * @return The root @c x where @a f(x) is equal to @a y.
     */
    MARC_API double root_find(double y,
                              double x0,
                              std::function<double(double)> const & f);

    /**
     * @brief Find root of a given equation within a search bracket.
     *
     * Given a function y=f(x), find the value of "x" at "y" within a
     * search bracket.
     *
     * @note The current implementation uses a hybrid approach where
     *       bisection is used if Newton-Raphson based root finding is
     *       not converging quickly enough.
     *
     * @note Do not place the equation in the form f(x)=0.  This
     *       implementation already does that by subtracting the known
     *       result @a y from f(x), i.e. f(x) - y = 0, when finding
     *       the root.
     *
     * @param[in] y  Known result of @a f(x).
     * @param[in] xl Lower bound of root finding bracket.
     * @param[in] xh Upper bound of root finding bracket.
     * @param[in] f  Function @a f(x) for which @c x will be
     *               computed.
     *
     * @throw std::invalid_argument Root finding bracket is not
     *                              suitable since @a f(xl) and
     *                              @a f(xh) do not bracket @a y.
     * @throw std::runtime_error    Root finding process is
     *                              diverging.
     *
     * @return The value @c x where @a f(x) is equal to @a y.
     */
    MARC_API double root_find(double y,
                              double xl,
                              double xh,
                              std::function<double(double)> const & f);
}


#endif  /* MARC_ROOT_FIND_H */
