/**
 * @file LongitudeImage.cpp
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

#include "LongitudeImage.h"
#include "Constants.h"


MaRC::LongitudeImage::LongitudeImage(double scale, double offset)
    : VirtualImage(scale, offset)
{
}

bool
MaRC::LongitudeImage::read_data_i(double /* lat */,
                                  double lon,
                                  double & data) const
{
    data = lon / C::degree;  // Convert radians to degrees.

    if (data < 0)
        data += 360;
    else if (data >= 360)
        data -= 360;

    return true;
}
