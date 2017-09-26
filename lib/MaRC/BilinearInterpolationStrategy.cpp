/**
 * @file BilinearInterpolationStrategy.cpp
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

#include "BilinearInterpolationStrategy.h"

#include <cmath>


MaRC::BilinearInterpolationStrategy::BilinearInterpolationStrategy(
    std::size_t nibble_left,
    std::size_t nibble_right,
    std::size_t nibble_top,
    std::size_t nibble_bottom)
    : InterpolationStrategy()
    , left_(nibble_left)
    , right_(nibble_right)
    , top_(nibble_top)
    , bottom_(nibble_bottom)
{
}

MaRC::BilinearInterpolationStrategy::~BilinearInterpolationStrategy()
{
}

bool
MaRC::BilinearInterpolationStrategy::interpolate(double const * data,
                                                 std::size_t samples,
                                                 std::size_t lines,
                                                 double x,
                                                 double z,
                                                 double & datum) const
{
    // Bilinear interpolation over 2x2 area of pixels.

    std::size_t const l = static_cast<std::size_t>(x); // floor(x)
    std::size_t const r = l + 1;                       // ceil (x);
    std::size_t const b = static_cast<std::size_t>(z); // floor(z)
    std::size_t const t = b + 1;                       // ceil (z);

    // Offsets
    std::size_t const ob = b * samples;
    std::size_t const ot = t * samples;

    // Note that we assume the image is inverted from top to bottom.

    // e.g., l > 0 && r < samples && b > 0 && < t < lines

    if (   l < this->left_ || r >= samples - this->right_
        || b < this->top_  || t >= lines - this->bottom_)
        return false;

    int count = 0;
    double tmp = 0;

    if (!std::isnan(data[ob + r]) && !std::isnan(data[ob + l])) {
        // [0][0]
        tmp += (data[ob + r] - data[ob + l]) * (x - l) + data[ob + l];

        // [1][1] =
        tmp += (data[ob + r] - data[ob + l]) * (z - b) + data[ob + r];

        count += 2;
    }

    if (!std::isnan(data[ot + r]) && !std::isnan(data[ot + l])) {
        // [0][1]
        tmp += (data[ot + r] - data[ot + l]) * (x - l) + data[ot + l];

        ++count;
    }

    if (!std::isnan(data[ot + l]) && !std::isnan(data[ob + l])) {
        // [1][0]
        tmp += (data[ot + l] - data[ob + l]) * (z - b) + data[ob + l];

        ++count;
    }

    if (count > 0) {
        datum = tmp / count;

        return true;
    }

  return false;
}
