// -*- C++ -*-
/**
 * @file Map_traits.h
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

#ifndef MARC_MAP_TRAITS_H
#define MARC_MAP_TRAITS_H

#include <limits>


/**
 * @namespace MaRC
 *
 * @brief Code specific to the MaRC library and program.
 */
namespace MaRC
{

    /**
     * @struct Map_traits
     *
     * @brief @c Map traits template structure.
     *
     * The @c Map template class delegates some operations to this
     * template traits structure.
     */
    template <typename T>
    struct Map_traits
    {
        /// Value used to initialize an empty map.
        /**
         * The initial/empty map data value for integer typed maps
         * will be zero, but the value for floating point typed maps
         * in MaRC is the Not-a-Number constant, not zero.  This
         * allows for easy disambiguation between actual data and
         * areas of the map that contain no data.
         */
        static constexpr T empty_value()
        {
            return std::numeric_limits<T>::quiet_NaN();
        }

        /// Make sure given minimum value falls within map data type
        /// range.
        /**
         * The idea behind this trait is to prevent data that is
         * actually outside the valid data range of the map data type
         * from being mapped.  In particular, the minimum value is
         * "clipped" if necessary.
         * @par
         * If an invalid minimum is used, data may be cast to a value
         * that is significantly different than its original value.
         * For example, setting the minimum to -65000 for a signed
         * short integer (16 bit) map would end up causing data values
         * less than or equal to that minimum to be "wrapped around"
         * on many implementations to a positive value (e.g. a short
         * integer set to -65000 results in a short integer value of
         * 536).
         *
         * @param[in] min User configured minimum.
         *
         * @return Minimum value that is greater than or equal to the
         *         minimum valid value for the given map data type.
         */
        static T minimum(double min)
        {
            static constexpr T const absolute_min =
                std::numeric_limits<T>::lowest();

            return (min < absolute_min
                    ? absolute_min : static_cast<T>(min));
        }

        /// Make sure given maximum value falls within map data type
        /// range.
        /**
         * The idea behind this trait is to prevent data that is actually
         * outside the valid data range of the map data type from being
         * mapped.  In particular, the maximum value is "clipped" if
         * necessary.
         *
         * @see The discussion for the @c minimum trait for additional
         * details.
         *
         * @param max User configured maximum.
         *
         * @return Maximum value that is less than or equal to the
         *         maximum valid value for the given map data type.
         */
        static T maximum(double max)
        {
            static constexpr T const absolute_max =
                std::numeric_limits<T>::max();

            return (max > absolute_max
                    ? absolute_max : static_cast<T>(max));
        }
    };

    // Map_traits specializations.
    template <>
    struct Map_traits<float>
    {
        static constexpr float empty_value()
        {
            return std::numeric_limits<float>::quiet_NaN();
        }

        static float minimum(double min)
        {
            static constexpr float const absolute_min =
                std::numeric_limits<float>::lowest();

            return (min < absolute_min ? absolute_min : min);
        }

        static float maximum(double max)
        {
            static constexpr float const absolute_max =
                std::numeric_limits<float>::max();

            return (max > absolute_max ? absolute_max : max);
        }
    };

    template <>
    struct Map_traits<double>
    {
        static constexpr double empty_value()
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        static double minimum(double min)
        {
            // No clipping is necessary since the data types are the
            // same.
            return min;
        }

        static double maximum(double max)
        {
            // No clipping is necessary since the data types are the
            // same.
            return max;
        }
    };

}

#endif  /* MAP_TRAITS_H */
