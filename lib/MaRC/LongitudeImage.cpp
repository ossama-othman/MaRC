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
#include "DefaultConfiguration.h"


MaRC::LongitudeImage::LongitudeImage(double scale, double offset)
    : VirtualImage(scale, offset)
{
}

bool
MaRC::LongitudeImage::read_data_i(double /* lat */,
                                  double lon,
                                  double & data) const
{
    using namespace MaRC::default_configuration;

    data = lon / C::degree;  // Convert radians to degrees.

    // Make sure the longitude is in the +/-360 degree range.  Note
    // that fmod() retains the sign of its first argument.
    data = std::fmod(data, longitude_range);

    // Data is now in the +/- 360 range but we need it to be between
    // [0, 360] or [-180, 180], depending on the configuration.  Shift
    // the longitude to the equivalent within the configured longitude
    // range.
    if (data < longitude_low)
        data += longitude_range;
    else if (data > longitude_high)
        data -= longitude_range;

    return true;
}
