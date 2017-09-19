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

    namespace default_configuration
    {

        constexpr double latitude_low   = -90;
        constexpr double latitude_high  =  90;
        constexpr double latitude_range =  latitude_high - latitude_low;

        constexpr double longitude_low   = 0;
        constexpr double longitude_high  = 360;
        constexpr double longitude_range = longitude_high - longitude_low;

        constexpr double mu_low  = -1;
        constexpr double mu_high =  1;

        constexpr double mu0_low  = -1;
        constexpr double mu0_high =  1;

        constexpr double cos_phase_low  = -1;
        constexpr double cos_phase_high =  1;

    }

} // End MaRC namespace


#endif  /* MARC_DEFAULT_CONFIGURATION_H */
