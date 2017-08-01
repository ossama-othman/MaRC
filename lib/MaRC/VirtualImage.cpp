/**
 * @file VirtualImage.cpp
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

#include "VirtualImage.h"


MaRC::VirtualImage::VirtualImage(double s, double o)
    : scale_(s)
    , offset_(o)
{
}

bool
MaRC::VirtualImage::read_data(double lat, double lon, double & data) const
{
    if (this->is_visible(lat, lon)
        && this->read_data_i(lat, lon, data)) {
        data = (data * this->scale_) + this->offset_;

        return true;
    }

  return false;
}

bool
MaRC::VirtualImage::is_visible(double /* lat */, double /* lon */) const
{
    return true;  // Always visible by default.
}
