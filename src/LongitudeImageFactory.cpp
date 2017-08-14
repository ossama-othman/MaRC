/**
 * @file LongitudeImageFactory.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#include "LongitudeImageFactory.h"

#include "MaRC/LongitudeImage.h"


std::unique_ptr<MaRC::SourceImage>
MaRC::LongitudeImageFactory::make(scale_offset_functor calc_so)
{
    constexpr double lon_min = 0;
    constexpr double lon_max = 360;
    double scale;
    double offset;

    if (!calc_so(lon_min, lon_max, scale, offset)) {
        throw std::range_error("Cannot store longitudes in map of "
                               "chosen type.");
    }

    return std::make_unique<MaRC::LongitudeImage>(scale, offset);
}
