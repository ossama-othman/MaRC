/**
 * @file Mu0Image.cpp
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

#include "Mu0Image.h"
#include "BodyData.h"
#include "Constants.h"

#include <cmath>


MaRC::Mu0Image::Mu0Image(std::shared_ptr<BodyData> body,
                         double sub_solar_lat,
                         double sub_solar_lon,
                         double scale,
                         double offset)
    : VirtualImage(scale, offset)
    , body_(body)
    , sub_solar_lat_(sub_solar_lat * C::degree)  // Radians
    , sub_solar_lon_(sub_solar_lon * C::degree)  // Radians
{
}

bool
MaRC::Mu0Image::read_data_i(double lat, double lon, double & data) const
{
    data = this->body_->mu0(this->sub_solar_lat_,
                            this->sub_solar_lon_,
                            lat,
                            lon);

    return true;
}

bool
MaRC::Mu0Image::is_visible(double lat, double lon) const
{
    return this->body_->mu0(this->sub_solar_lat_,
                            this->sub_solar_lon_,
                            lat,
                            lon) >= 0;
}
