// -*- C++ -*-
/**
 * @file Validate.h
 *
 * Copyright (C) 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 *
 * @todo Rename this file and the functions inside it to something
 *       less confusing.
 */

#ifndef MARC_VALIDATE_H
#define MARC_VALIDATE_H

#include "marc/Constants.h"

#include <stdexcept>
#include <cmath>


namespace MaRC
{

    /**
     * @brief Validate and return latitude in radians.
     *
     * Make sure the given latitude @a lat falls within the range of
     * valid latitudes, i.e. [-90, 90].
     *
     * @param[in] lat Latitude in degrees.
     *
     * @return Latitude in radians.
     *
     * @throw std::invalid_argument Invalid @a lat.
     */
    inline double validate_latitude(double lat)
    {
        if (std::isnan(lat) || lat < -90 || lat > 90)
            throw std::invalid_argument("invalid latitude");

        return lat * C::degree;
    }

    /**
     * @brief Validate and return longitude in radians.
     *
     * Make sure the given longitude @a lon falls within the range of
     * valid longitudes, i.e. [-360, 360].
     *
     * @param[in] lon Longitude in degrees.
     *
     * @return Longitude in radians.
     *
     * @throw std::invalid_argument Invalid @a lon.
     */
    inline double validate_longitude(double lon)
    {
        if (std::isnan(lon) || lon < -360 || lon > 360)
            throw std::invalid_argument("invalid longitude");

        // We don't shift negative longitudes to the equivalent
        // positive longitude (i.e. [0, 360]) here since there are
        // cases where a negative longitude is desired, such as when
        // mapping from [-180, 180] instead of [0, 360].

        return lon * C::degree;
    }

    /**
     * @brief Validate and return position angle in radians.
     *
     * Make sure the given position angle @a north falls within the
     * range of valid position angles, i.e. [-360, 360].
     *
     * @param[in] north Position angle in degrees.
     *
     * @return Position angle in radians.
     *
     * @throw std::invalid_argument Invalid @a north.
     */
    inline double validate_position_angle(double north)
    {
        if (std::isnan(north) || north < -360 || north > 360)
            throw std::invalid_argument("invalid position angle");

        return north * C::degree;
    }

} // End MaRC namespace


#endif  /* MARC_VALIDATE_H */
