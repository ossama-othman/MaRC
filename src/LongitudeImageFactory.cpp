/**
 * @file LongitudeImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "LongitudeImageFactory.h"
#include "MapParameters.h"

#include "marc/LongitudeImage.h"
#include "marc/DefaultConfiguration.h"

#include <stdexcept>
#include <cmath>


MaRC::LongitudeImageFactory::LongitudeImageFactory()
    : SourceImageFactory()
{
}

bool
MaRC::LongitudeImageFactory::populate_parameters(
    MaRC::MapParameters &p) const
{
    using namespace MaRC::default_configuration;

    /**
     * @note "deg" is used is instead of "degree" per %FITS standard
     *       recommendation for the BUNIT keyword.
     *
     * @see https://heasarc.gsfc.nasa.gov/docs/fcg/standard_dict.html
     */
    p.bunit("deg");
    p.datamax(longitude_high);
    p.datamin(longitude_low);

    return true;
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
