/**
 * @file fixed_value_image.cpp
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "fixed_value_image.h"

#include <marc/Log.h>


bool
MaRC::fixed_value_image::read_data(double lat,
                                   double lon,
                                   double & data) const
{
    if (lon < this->lower_lon_)
        lon += C::_2pi;
    else if (lon > this->upper_lon_)
        lon -= C::_2pi;

    bool const visible =
        lat >= this->lower_lat_ && lat <= this->upper_lat_
        && lon >= this->lower_lon_ && lon <= this->upper_lon_;

    if (visible)
        data = this->value_;

    return visible;
}

bool
MaRC::fixed_value_image::read_data(double lat,
                                   double lon,
                                   double & data,
                                   double & weight,
                                   bool /* scan */) const
{
    bool const visible = this->read_data(lat, lon, data);

    if (visible) {
        /**
         * @todo Refactor this duplicate code in both @c read_data()
         *       methods.
         */
        if (lon < this->lower_lon_)
            lon += C::_2pi;
        else if (lon > this->upper_lon_)
            lon -= C::_2pi;

        // Give less weight to pixels close to an edge of the image.
        weight = std::min(this->upper_lat_ - lat,
                          std::min(lat - this->lower_lat_,
                                   std::min(this->upper_lon_ - lon,
                                            lon - this->lower_lon_)));
    }

    return visible;
}
