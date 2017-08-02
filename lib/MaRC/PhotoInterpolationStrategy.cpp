/**
 * @file PhotoInterpolationStrategy.cpp
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

#include "PhotoInterpolationStrategy.h"

#include <cmath>
#include <iostream>


MaRC::PhotoInterpolationStrategy::PhotoInterpolationStrategy(
    std::size_t samples,
    std::size_t lines,
    std::size_t nibble_left,
    std::size_t nibble_right,
    std::size_t nibble_top,
    std::size_t nibble_bottom)
    : InterpolationStrategy()
    , samples_(samples)
    , left_(nibble_left)
    , right_(samples - nibble_right)
    , top_(nibble_top)
    , bottom_(lines - nibble_bottom)
{
}

MaRC::PhotoInterpolationStrategy::~PhotoInterpolationStrategy()
{
}

bool
MaRC::PhotoInterpolationStrategy::interpolate(double const * image,
                                              double x,
                                              double z,
                                              double & data) const
{
    // Bilinear interpolation over 2x2 area of pixels.

    std::size_t const l = static_cast<std::size_t>(x); // floor(x)
    std::size_t const r = l + 1;                       // ceil  (x);
    std::size_t const b = static_cast<std::size_t>(z); // floor(z)
    std::size_t const t = b + 1;                       // ceil  (z);

    // Offsets
    std::size_t const ob = b * this->samples_;
    std::size_t const ot = t * this->samples_;

    // Note that we assume the image is inverted from top to bottom.

    // e.g., l > 0 && r < samples && b > 0 && < t < lines

    if (l < this->left_ || r >= this->right_
        || b < this->top_  || t >= this->bottom_)
        return false;

    int count = 0;
    double tmp = 0;

    if (!std::isnan(image[ob + r]) && !std::isnan(image[ob + l])) {
        // [0][0]
        tmp += (image[ob + r] - image[ob + l]) * (x - l) + image[ob + l];

        // [1][1] =
        tmp += (image[ob + r] - image[ob + l]) * (z - b) + image[ob + r];

        count += 2;
    }

    if (!std::isnan(image[ot + r]) && !std::isnan(image[ot + l])) {
        // [0][1]
        tmp += (image[ot + r] - image[ot + l]) * (x - l) + image[ot + l];

        ++count;
    }

    if (!std::isnan(image[ot + l]) && !std::isnan(image[ob + l])) {
        // [1][0]
        tmp += (image[ot + l] - image[ob + l]) * (z - b) + image[ob + l];

        ++count;
    }

    if (count > 0) {
        data = tmp / count;

        return true;
    }

  return false;
}
