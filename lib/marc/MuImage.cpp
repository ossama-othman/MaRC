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

#include <cassert>


MaRC::MuImage::MuImage(std::shared_ptr<BodyData> body,
                       double sub_observ_lat,
                       double sub_observ_lon,
                       double range,
                       double scale,
                       double offset)
    : VirtualImage(scale, offset)
    , body_(std::move(body))
    , sub_observ_lat_(sub_observ_lat * C::degree) // Radians
    , sub_observ_lon_(sub_observ_lon * C::degree) // Radians
    , range_(range)
{
}

bool
MaRC::MuImage::read_data_i(double lat, double lon, double & data) const
{
    /*
      A visible point at the given latitude and longitude will have an
      emission angle in the interval [-90, 90], i.e. mu >= 0 where mu
      is the cosine of the emission angle.  Points that are not
      visible to the observer will have a value of mu < 0.
    */
    data = this->body_->mu(this->sub_observ_lat_,
                           this->sub_observ_lon_,
                           lat,
                           lon,
                           this->range_);

    assert(data >= -1 && data <= 1);

    return true;
}
