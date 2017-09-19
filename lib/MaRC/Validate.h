// -*- C++ -*-
/**
 * @file Validate.h
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

#ifndef MARC_VALIDATE_H
#define MARC_VALIDATE_H


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
     */
    double validate_latitude(double lat)
    {
        if (lat < -90 || lat > 90)
            throw std::range_error("Invalid latitude.");

        return lat * C::degree;
    }

    /**
     * @brief Validate and return longitude in radians.
     *
     * Make sure the given longitude @a lon falls within the range of
     * valid longitudes, i.e. [-360, 360].
     *
     * @param[in] lat Latitude in degrees.
     *
     * @return Latitude in radians.
     */
    double validate_longitude(double lon)
    {
        if (lon < -360 || lon > 360)
            throw std::range_error("Invalid longitude.");

        return lon * C::degree;
    }

} // End MaRC namespace


#endif  /* MARC_VALIDATE_H */
