// -*- C++ -*-
/**
 * @file SourceImage.h
 *
 * Copyright (C) 2017-2018  Ossama Othman
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

#ifndef MARC_DETAILS_SCALE_AND_OFFSET_H
#define MARC_DETAILS_SCALE_AND_OFFSET_H

#include <limits>
#include <cmath>


namespace MaRC
{
    namespace details
    {
        /**
         * @brief Integer data scale and offset calculation functor.
         *
         * Determine the best scale and offset parameters applied to
         * floating point data read from source images to retain as
         * many significant digits as possible when storing that data
         * in integer typed maps.
         *
         * @see MaRC::scale_and_offset()
         *
         * @note This functor is not part of the public %MaRC library
         *       API.
         */
        template <typename T>
        struct scale_and_offset
        {
            /**
             * @brief Compute data scaling values.
             *
             * Given the source data range [@a min, @a max], determine
             * the @a scale and @a offset values suitable for
             * maximizing the number of significant digits retained
             * when storing their floating point values in a
             * destination of data type @c T.
             *
             * @param[in]  min    The minimum source value to be
             *                    potentially stored in an integer of
             *                    type @c T.
             * @param[in]  max    The maximum source value to be
             *                    potentially stored in an integer of
             *                    type @c T.
             * @param[out] scale  Factor by which to multiply the
             *                    source value to bring it into
             *                    destination data type @c T range.
             *                    This will always be a power of 10
             *                    for simplicity.
             * @param[out] offset Value added to the source value to
             *                    bring it into the destination data
             *                    type @c T range.
             *
             * @retval true  Suitable @a scale and @a offset values were
             *               found.
             * @retval false Suitable @a scale and @a offset values were not
             *               found, such as when a complete loss of
             *               significant digits would occur when
             *               scaled and offset values are assigned to
             *               an integer due to trunctation (e.g.
             *               @a scale < 1).
             *
             * @see MaRC::scale_and_offset() for further details.
             */
            bool operator()(double min,
                            double max,
                            double & scale,
                            double & offset) const
            {
                constexpr auto T_lowest = std::numeric_limits<T>::lowest();
                constexpr auto T_max    = std::numeric_limits<T>::max();

                static_assert(
                    T_lowest >= std::numeric_limits<double>::lowest() / 2
                    && T_max <= std::numeric_limits<double>::max() / 2,
                    "Integer type is too large for "
                    "scale/offset calculation");

                /*
                  Avoid integer overflow by performing a floating point
                  subtraction.  No overflow will occur since we already
                  know:
                  T_max - T_lowest < std::numeric_limits<double>::max()
                */
                constexpr double type_range =
                    static_cast<double>(T_max) - T_lowest;

                double const data_range = max - min;

                if (!std::isfinite(data_range)
                    || data_range < 0
                    || data_range > type_range) {
                    // data_range is not a finite value
                    // (e.g. overflowed) or min > max or can't fit all
                    // data into desired type T
                    return false;
                }

                int const exponent =
                    static_cast<int>(std::numeric_limits<T>::digits10)
                    - static_cast<int>(std::log10(data_range));

                scale = std::pow(10, exponent);

                if (min * scale < T_lowest)
                    offset = data_range / 2 * scale;
                else if (max * scale > T_max)
                    offset = -data_range / 2 * scale;
                else
                    offset = 0;

                return true;
            }
        };

        /**
         * @brief @c float typed data scale and offset calculation
         *        functor.
         *
         * Automatic source data scaling is not performed when mapping
         * to floating point typed maps.  This implementation is
         * basically a no-op, and returns @a scale and @a offset
         * values that result in the source data remaining unchanged.
         *
         * @see MaRC::scale_and_offset()
         *
         * @note This functor is not part of public %MaRC library
         *       API.
         */
        template <>
        struct scale_and_offset<float>
        {
            /**
             * @brief @c float destination data type specialization.
             *
             * No scaling is needed for floating point types.
             *
             * @param[out] scale  This will always be 1.
             * @param[out] offset This will always be 0.
             *
             * @return This specialization always returns @c true.
             *
             * @see MaRC::details::scale_and_offset
             * @see MaRC::scale_and_offset()
             */
            bool operator()(double /* min */,
                            double /* max */,
                            double & scale,
                            double & offset) const
            {
                // No auto-scaling for floating point map data.
                scale  = 1;
                offset = 0;

                return true;
            }
        };

        /**
         * @brief @c double typed data scale and offset calculation
         *        functor.
         *
         * Automatic source data scaling is not performed when mapping
         * to floating point typed maps.  This implementation is
         * basically a no-op, and returns @a scale and @a offset
         * values that result in the source data remaining unchanged.
         *
         * @see MaRC::scale_and_offset()
         *
         * @note This functor is not part of public %MaRC library
         *       API.
         */
        template <>
        struct scale_and_offset<double>
        {
            /**
             * @brief @c double destination data type specialization.
             *
             * No scaling is needed for floating point types.
             *
             * @param[out] scale  This will always be 1.
             * @param[out] offset This will always be 0.
             *
             * @return This specialization always returns @c true.
             *
             * @see MaRC::details::scale_and_offset
             * @see MaRC::scale_and_offset()
             */
            bool operator()(double /* min */,
                            double /* max */,
                            double & scale,
                            double & offset) const
            {
                // No auto-scaling for floating point map data.
                scale  = 1;
                offset = 0;

                return true;
            }
        };
    }  // details
} // End MaRC namespace


#endif  /* MARC_DETAILS_SCALE_AND_OFFSET_H */
