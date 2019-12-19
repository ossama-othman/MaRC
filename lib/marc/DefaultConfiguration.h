// -*- C++ -*-
/**
 * @file DefaultConfiguration.h
 *
 * Copyright (C) 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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
