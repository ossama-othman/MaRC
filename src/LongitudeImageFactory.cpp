/**
 * @file LongitudeImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2019-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "LongitudeImageFactory.h"
#include "map_parameters.h"

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
    MaRC::map_parameters &p) const
{
    /**
     * @note "deg" is used is instead of "degree" per %FITS standard
     *       recommendation for the BUNIT keyword.
     *
     * @see https://heasarc.gsfc.nasa.gov/docs/fcg/standard_dict.html
     */
    p.bunit("deg");

    /**
     * @note The %FITS @c DATAMIN and @c DATAMAX values are not set in
     *       the map parameters.  Instead they are set in this image
     *       factory so that they may be used when plotting the image
     *       to the map.  The %FITS @c DATAMIN and @c DATAMAX values
     *       corresponding to data that was actually plotted will be
     *       automatically written to map %FITS once mapping is done.
     *
     * @see make()
     */
    // using namespace MaRC::default_configuration;
    // p.datamax(longitude_high);
    // p.datamin(longitude_low);

    return true;
}

std::unique_ptr<MaRC::SourceImage>
MaRC::LongitudeImageFactory::make(scale_offset_functor calc_so)
{
    using namespace MaRC::default_configuration;

    double scale  = 1;
    double offset = 0;

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

    /*
      Set physical data extrema if not previously set.

      Scale the default minimum and maximum to match the physical data
      scaling.
    */
    this->minimum(longitude_low  * scale + offset);
    this->maximum(longitude_high * scale + offset);

    return std::make_unique<MaRC::LongitudeImage>(scale, offset);
}
