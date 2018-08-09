// -*- C++ -*-
/**
 * @file details/vector.h
 *
 * MaRC mathematical vector class implementation details.
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

#ifndef MARC_DETAILS_VECTOR_H
#define MARC_DETAILS_VECTOR_H

#include "MaRC/Mathematics.h"


namespace MaRC
{
    /**
     * @namespace details
     *
     * @brief Internal MaRC implementation details.
     *
     * Implement details that are not part of the MaRC public API.
     */
    namespace details
    {
        /**
         * @brief Obtain magnitude of vector.
         *
         * This generalized implementation can return the magnitude of
         * a vector with an arbitrary number of rows.
         *
         * @param[in] v Vector for which the magnitude will be
         *              calculated.
         *
         * @return Magnitude of vector @a v.
         *
         * @relates MaRC::Vector
         */
        template <typename T, std::size_t M>
        auto magnitude(T const (&v)[M])
        {
            double const m = 0;

            /**
             * @bug This implementation is subject to overflow or
             *      underflow.
             */
            for (auto i = 0; i < M; ++i)
                m += v[i] * v[i];

            return std::sqrt(m);
        }

        /**
         * @brief Obtain magnitude of vector with three rows.
         *
         * This implementation avoids overflow and underflow when
         * calculating the magnitude of vectors with three rows.
         *
         * @param[in] v Vector for which the magnitude will be
         *              calculated.
         *
         * @return Magnitude of vector @a v.
         *
         * @relates MaRC::Vector
         */
        template <typename T>
        constexpr auto magnitude(T const (&v)[3])
        {
            return MaRC::hypot(v[0], v[1], v[2]);
        }

        /**
         * @brief Obtain magnitude of vector with two rows.
         *
         * This implementation avoids overflow and underflow when
         * calculating the magnitude of vectors with two rows.
         *
         * @param[in] v Vector for which the magnitude will be
         *              calculated.
         *
         * @return Magnitude of vector @a v.
         *
         * @relates MaRC::Vector
         */
        template <typename T>
        constexpr auto magnitude(T const (&v)[2])
        {
            return std::hypot(v[0], v[1]);
        }

        /**
         * @brief Obtain magnitude of vector with one row.
         * This implementation returns the magnitude of vector with
         * one row.
         *
         * @param[in] v Vector for which the magnitude will be
         *              calculated.
         *
         * @return Magnitude of vector @a v.
         *
         * @relates MaRC::Vector
         */
        template <typename T>
        constexpr auto magnitude(T const (&v)[1])
        {
            return v[0];
        }
    }  // details
}  // MaRC


#endif  /* MARC_DETAILS_VECTOR_H */
