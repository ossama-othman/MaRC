/**
 * @file LongitudeImageFactory.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "LongitudeImageFactory.h"

#include "marc/LongitudeImage.h"
#include "marc/DefaultConfiguration.h"

#include <stdexcept>
#include <cmath>


MaRC::LongitudeImageFactory::LongitudeImageFactory()
    : SourceImageFactory()
{
}

std::unique_ptr<MaRC::SourceImage>
MaRC::LongitudeImageFactory::make(scale_offset_functor calc_so)
{
    using namespace MaRC::default_configuration;

    double scale;
    double offset;

    /**
     * @todo Do we really want to throw an exception here?  Why not
     *       just return a default constructed
     *       @c std::unique_ptr<MaRC::SourceImage> instead?  The same
     *       goes for the other @c VirtualImage factory
     *       implementations.
     */
    if (!calc_so(longitude_low, longitude_high, scale, offset)) {
        throw std::range_error("Cannot store longitudes in map of "
                               "chosen data type.");
    }

    // Scale the default minimum and maximum to match the physical
    // data scaling.
    if (std::isnan(this->minimum()))
        this->minimum(longitude_low  * scale + offset);

    if (std::isnan(this->maximum()))
        this->maximum(longitude_high * scale + offset);

    return std::make_unique<MaRC::LongitudeImage>(scale, offset);
}
