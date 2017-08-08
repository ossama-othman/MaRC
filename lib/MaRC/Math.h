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

#include <utility>
#include <cmath>


namespace MaRC
{
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
     * @returns @c true if real roots were found, @c false otherwise.
     */
    bool
    quadratic_roots(double a,
                    double b,
                    double c,
                    std::pair<double, double> & roots)
    {
        double const discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
            return false;  // One or both roots are not real.

        double const q = -(b + signum(b) * sqrt(discriminant)) / 2;

        roots.first  = q / a;
        roots.second = c / q;

        return true;
    }

} // MaRC


#endif  /* MARC_MATH_H */
