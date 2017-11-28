//   -*- C++ -*-
/**
 * @file root_find.h
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

#ifndef MARC_ROOT_FIND_H
#define MARC_ROOT_FIND_H

#include <cmath>


namespace MaRC
{

    /**
     * @brief Find root of the given @a equation.
     *
     * @todo Document this function.
     */
    template <class MAP_FACTORY>
    double
    root_find(double ordinate,
              double ordinate_guess,
              double abscissa_guess,
              double (MAP_FACTORY::* equation)(double) const,
              MAP_FACTORY const * map)
    {
        constexpr double tolerance = 1e-4;
        constexpr double delta = 1e-3;
        constexpr unsigned int max_iterations = 10;
        double const abscissa_guess_orig = abscissa_guess;

        double abscissa;

        for (unsigned int i = 0; i < max_iterations; ++i) {
            // Center divided difference numerical method of computing
            // the first derivative (VERY LOW ERROR)
            double const deriv =
                (-(map->*equation)(abscissa_guess + 2 * delta)
                 + 8 * (map->*equation)(abscissa_guess + delta)
                 - 8 * (map->*equation)(abscissa_guess - delta)
                 + (map->*equation)(abscissa_guess - 2 * delta))
                / 12 / delta;

            abscissa =
                abscissa_guess + (ordinate - ordinate_guess) / deriv;

            if (std::abs(abscissa - abscissa_guess) < tolerance)
                return abscissa;

            abscissa_guess = abscissa;
            ordinate_guess = (map->*equation)(abscissa_guess);
        }

        // Root not found.

        double const begin =
            abscissa_guess_orig - abscissa_guess_orig * 2;
        double const end =
            abscissa_guess_orig + abscissa_guess_orig * 2;

        // Try an incremental search for a good initial guess since
        // original guess was not successful.
        abscissa_guess = begin;
        for (unsigned int count = 0; count < max_iterations; ++count) {
            double const temp = abscissa_guess;

            ordinate_guess = (map->*equation)(abscissa_guess);

            for (unsigned int i = 0; i < max_iterations; ++i) {
                // Center divided difference numerical method of
                // computing the first derivative (VERY LOW ERROR).
                double const deriv =
                    (-(map->*equation)(abscissa_guess + 2 * delta)
                     + 8 * (map->*equation)(abscissa_guess + delta)
                     - 8 * (map->*equation)(abscissa_guess - delta)
                     + (map->*equation)(abscissa_guess - 2 * delta))
                    / 12 / delta;

                abscissa =
                    abscissa_guess + (ordinate - ordinate_guess) / deriv;

                if (std::abs(abscissa - abscissa_guess) < tolerance)
                    return abscissa;

                abscissa_guess = abscissa;
                ordinate_guess = (map->*equation)(abscissa_guess);
            }

            abscissa_guess = temp;
            abscissa_guess += delta;

            if (abscissa_guess >= end)
                break;
        }

        throw
            std::runtime_error ("INTERNAL ERROR: Root finding process "
                                "seems to be diverging.");

        return abscissa;
    }

}


#endif  /* MARC_ROOT_FIND_H */
