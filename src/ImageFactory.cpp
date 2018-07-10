/**
 * @file ImageFactory.cpp
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

#include "ImageFactory.h"

#include <limits>
#include <cmath>
#include <stdexcept>


MaRC::ImageFactory::ImageFactory()
    : minimum_(std::numeric_limits<double>::lowest())
    , maximum_(std::numeric_limits<double>::max())
{
}

void
MaRC::ImageFactory::minimum(double m)
{
    if (std::isnan(m) || this->maximum_ < m)
        throw std::invalid_argument("invalid source image minimum");

    this->minimum_ = m;
}

void
MaRC::ImageFactory::maximum(double m)
{
    if (std::isnan(m) || this->minimum_ > m)
        throw std::invalid_argument("invalid source image maximum");

    this->maximum_ = m;
}
