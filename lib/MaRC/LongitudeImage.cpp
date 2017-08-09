/**
 * @file LongitudeImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 * Copyright (C) 2005, 2006, 2017  Nathanael Nerode
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
 * @author Nathanael Nerode
 */

#include "LongitudeImage.h"
#include "Constants.h"

#include <cmath>


MaRC::LongitudeImage::LongitudeImage(double scale, double offset)
    : VirtualImage(scale, offset)
{
}

bool
MaRC::LongitudeImage::read_data_i(double /* lat */,
                                  double lon,
                                  double & data) const
{
    static constexpr int maxlon = 360;  // 0 to 360 degree range.

    data = lon / C::degree;  // Convert radians to degrees.

    // Make sure the longitude is in the +/-360 degree range.
    data = std::fmod(data, maxlon);

    // Data is now in the +/- 360 range but we need it to be
    // positive.  fmod() retains the sign of its first argument so
    // shift the negative longitude to its positive equivalent (not
    // the same as absolute value!).
    if (data < 0)
        data += maxlon;

    return true;
}
