/**
 * @file LatitudeImageFactory.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
