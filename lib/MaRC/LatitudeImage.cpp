/**
 * @file LatitudeImage.cpp
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

#include "LatitudeImage.h"
#include "BodyData.h"
#include "Constants.h"


MaRC::LatitudeImage::LatitudeImage(BodyData const & body,
                                   bool graphic_latitudes)
  : body_(body)
  , graphic_latitudes_(graphic_latitudes)
{
}

bool
MaRC::LatitudeImage::read_data_i(double lat,
                                 double /* lon */,
                                 double & data) const
{
    if (this->graphic_latitudes_)
        data = this->body_.graphic_latitude(lat);
    else
        data = lat;

    data /=  C::degree;  // Convert radians to degrees.

    return true;
}
