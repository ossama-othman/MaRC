/**
 * @file LatitudeImageFactory.cpp
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

#include "LatitudeImageFactory.h"

#include "marc/LatitudeImage.h"
#include "marc/DefaultConfiguration.h"

#include <stdexcept>
#include <cmath>


MaRC::LatitudeImageFactory::LatitudeImageFactory(
    std::shared_ptr<BodyData> body,
    bool graphic_latitudes)
    : SourceImageFactory()
    , body_(std::move(body))
    , graphic_latitudes_(graphic_latitudes)
{
}

std::unique_ptr<MaRC::SourceImage>
MaRC::LatitudeImageFactory::make(scale_offset_functor calc_so)
{
    using namespace MaRC::default_configuration;

    double scale;
    double offset;

    if (!calc_so(latitude_low, latitude_high, scale, offset)) {
        throw std::range_error("Cannot store latitudes in map of "
                               "chosen data type.");
    }

    // Scale the default minimum and maximum to match the physical
    // data scaling.
    if (std::isnan(this->minimum()))
        this->minimum(latitude_low  * scale + offset);

    if (std::isnan(this->maximum()))
        this->maximum(latitude_high * scale + offset);

    return
        std::make_unique<MaRC::LatitudeImage>(this->body_,
                                              this->graphic_latitudes_,
                                              scale,
                                              offset);
}
