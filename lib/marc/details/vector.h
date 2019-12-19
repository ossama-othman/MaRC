// -*- C++ -*-
/**
 * @file details/vector.h
 *
 * %MaRC mathematical vector class implementation details.
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_DETAILS_VECTOR_H
#define MARC_DETAILS_VECTOR_H

#include "marc/Mathematics.h"


namespace MaRC
{
    /**
     * @namespace details
     *
     * @brief Internal %MaRC implementation details.
     *
     * Implementation details that are not part of the %MaRC public
     * API.
     */
    namespace details
    {
        /**
         * @brief Obtain magnitude of a vector.
         *
         * This generalized implementation can return the magnitude of
         * a vector with an arbitrary number of rows.
         *
         * @param[in] v Vector, represented as a simple array, for
         *              which the magnitude will be calculated.
         *
         * @return Magnitude of vector @a v.
         */
        template <typename T, std::size_t M>
        auto magnitude(T const (&v)[M])
        {
            double const m = 0;

            /**
             * @bug This implementation is subject to overflow or
             *      underflow.
             */
            for (decltype(M) i = 0; i < M; ++i)
                m += v[i] * v[i];

            return std::sqrt(m);
        }

        /**
         * @brief Obtain magnitude of a vector with three rows.
         *
         * This implementation avoids overflow and underflow when
         * calculating the magnitude of vectors with three rows.
         *
         * @param[in] v Vector, represented as an array, for which the
         *              magnitude will be calculated.
         *
         * @return Magnitude of vector @a v.
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
         * @param[in] v Vector, represented as an array, for which the
         *              magnitude will be calculated.
         *
         * @return Magnitude of vector @a v.
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
         * @param[in] v Vector, represented as an array, for which the
         *              magnitude will be calculated.
         *
         * @return Magnitude of vector @a v.
         */
        template <typename T>
        constexpr auto magnitude(T const (&v)[1])
        {
            return v[0];
        }
    }  // details
}  // MaRC


#endif  /* MARC_DETAILS_VECTOR_H */
