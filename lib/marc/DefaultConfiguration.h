// -*- C++ -*-
/**
 * @file DefaultConfiguration.h
 *
 * Copyright (C) 2017  Ossama Othman
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

#ifndef MARC_DEFAULT_CONFIGURATION_H
#define MARC_DEFAULT_CONFIGURATION_H


namespace MaRC
{

    /**
     * @namespace default_configuration
     *
     * @brief Default %MaRC configuration constants.
     *
     * These constants are used throughout %MaRC when a corresponding
     * user-supplied value is not available.
     */
    namespace default_configuration
    {

        /**
         * @name Map Latitude Constants
         */
        //@{
        /**
         * @brief Lowest latitude in degrees to be potentially mapped.
         *
         * @note This value should never be less than -90.
         */
        constexpr double latitude_low   = -90;

        /**
         * @brief Highest latitude in degrees to be potentially mapped.
         *
         * @note This value should never be greater than 90.
         */
        constexpr double latitude_high  =  90;

        /**
         * @brief Maximum latitude range in degrees to be potentially
         *        mapped.
         *
         * @note This value should never be greater than 180.
         */
        constexpr double latitude_range =  latitude_high - latitude_low;
        //@}

        /**
         * @name Map Longitude Constants
         */
        //@{
        /**
         * @brief Lowest longitude in degrees to be potentially mapped.
         *
         * @note This value should never be less than -180.
         */
        constexpr double longitude_low   = 0;

        /**
         * @brief Highest longitude in degrees to be potentially mapped.
         *
         * @note This value should never be greater than 360.
         */
        constexpr double longitude_high  = longitude_low + 360;

        /**
         * @brief Maximum longitude range in degrees to be potentially
         *        mapped.
         *
         * @note This value should never be greater than 360.
         */
        constexpr double longitude_range = longitude_high - longitude_low;
        //@}

        /**
         * @name Map Cosine Virtual Image Constants
         *
         * @note As these values are cosines, the allowable range is
         *       [-1, 1].
         */
        //@{
        constexpr double mu_low  = -1;
        constexpr double mu_high =  1;

        constexpr double mu0_low  = -1;
        constexpr double mu0_high =  1;

        constexpr double cos_phase_low  = -1;
        constexpr double cos_phase_high =  1;
        //@}

    }

} // End MaRC namespace


#endif  /* MARC_DEFAULT_CONFIGURATION_H */
