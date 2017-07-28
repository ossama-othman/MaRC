/**
 * @file CosPhaseImage.cpp
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

#include "CosPhaseImage.h"
#include "MuImage.h"
#include "OblateSpheroid.h"
#include "Constants.h"


MaRC::CosPhaseImage::CosPhaseImage(OblateSpheroid const & body,
                                   double sub_observ_lat,
                                   double sub_observ_lon,
                                   double sub_solar_lat,
                                   double sub_solar_lon,
                                   double range)
    : VirtualImage (10000, 0)
    , body_(body)
    , sub_observ_lat_(sub_observ_lat * C::degree) // Radians
    , sub_observ_lon_(sub_observ_lon * C::degree) // Radians
    , sub_solar_lat_(sub_solar_lat * C::degree)   // Radians
    , sub_solar_lon_(sub_solar_lon * C::degree)   // Radians
    , range_(range)
{
}

bool
MaRC::CosPhaseImage::read_data_i(double lat,
                                 double lon,
                                 double & data) const
{
    data = this->body_.cos_phase(this->sub_observ_lat_,
                                 this->sub_observ_lon_,
                                 this->sub_solar_lat_,
                                 this->sub_solar_lon_,
                                 lat,
                                 lon,
                                 this->range_);

    return true;
}

bool
MaRC::CosPhaseImage::is_visible(double lat, double lon) const
{
    // This implementation is as the same as the one used by the
    // MuImage class.

    return MaRC::MuImage::is_visible_i(this->body_,
                                       this->sub_observ_lat_,
                                       this->sub_observ_lon_,
                                       lat,
                                       lon,
                                       this->range_);
}
