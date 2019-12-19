/**
 * @file BilinearInterpolation.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "BilinearInterpolation.h"

#include <cmath>


MaRC::BilinearInterpolation::BilinearInterpolation(
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

bool
MaRC::BilinearInterpolation::interpolate(double const * data,
                                         double x,
                                         double z,
                                         double & datum) const
{
    // Bilinear interpolation over 2x2 area of pixels.

    auto const l = static_cast<std::size_t>(x); // floor(x)
    auto const r = l + 1;                       // ceil (x);
    auto const b = static_cast<std::size_t>(z); // floor(z)
    auto const t = b + 1;                       // ceil (z);

    // Offsets
    auto const ob = b * this->samples_;  // Bottom line
    auto const ot = t * this->samples_;  // Top line

    // Note that we assume the image is inverted from top to bottom.

    // e.g., l > 0 && r < samples && b > 0 && < t < lines

    if (   l < this->left_ || r >= this->right_
        || b < this->top_  || t >= this->bottom_)
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
