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

#include <functional>


namespace MaRC
{
    /**
     * @brief Find root of a given equation .
     *
     * Given function f(x), find the root "x" using the Newton-Raphson
     * method.
     *
     * @param[in] y  Known result of @a f(x).
     * @param[in] x0 Initial guess of the root @c x.
     * @param[in] f  Function @a f(x) for which @c x will be
     *               computed.
     */
    double root_find(double y,
                     double x0,
                     std::function<double(double)> f);
}


#endif  /* MARC_ROOT_FIND_H */
