/**
 * @file MuImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
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

#include "MuImage.h"
#include "BodyData.h"
#include "Constants.h"

#include <cmath>


MaRC::MuImage::MuImage(std::shared_ptr<BodyData> body,
                       double sub_observ_lat,
                       double sub_observ_lon,
                       double range,
                       double scale,
                       double offset)
    : VirtualImage(scale, offset)
    , body_(body)
    , sub_observ_lat_(sub_observ_lat * C::degree) // Radians
    , sub_observ_lon_(sub_observ_lon * C::degree) // Radians
    , range_(range)
{
}

bool
MaRC::MuImage::read_data_i(double lat, double lon, double & data) const
{
    data = this->body_->mu(this->sub_observ_lat_,
                           this->sub_observ_lon_,
                           lat,
                           lon,
                           this->range_);

    return true;
}

bool
MaRC::MuImage::is_visible(double lat, double lon) const
{
    return this->body_->mu(this->sub_observ_lat_,
                           this->sub_observ_lon_,
                           lat,
                           lon,
                           this->range_) >= 0;
}
