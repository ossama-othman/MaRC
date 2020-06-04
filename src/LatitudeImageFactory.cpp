/**
 * @file LatitudeImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2019-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "LatitudeImageFactory.h"
#include "map_parameters.h"

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

bool
MaRC::LatitudeImageFactory::populate_parameters(
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
    // p.datamax(latitude_high);
    // p.datamin(latitude_low);

    return true;
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

    /*
      Set physical data extrema if not previously set.

      Scale the default minimum and maximum to match the physical data
      scaling.
    */
    this->minimum(latitude_low  * scale + offset);
    this->maximum(latitude_high * scale + offset);

    return
        std::make_unique<MaRC::LatitudeImage>(this->body_,
                                              this->graphic_latitudes_,
                                              scale,
                                              offset);
}
