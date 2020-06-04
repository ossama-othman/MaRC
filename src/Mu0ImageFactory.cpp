/**
 * @file Mu0ImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2019-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "Mu0ImageFactory.h"
#include "map_parameters.h"

#include "marc/Mu0Image.h"
#include "marc/DefaultConfiguration.h"

#include <stdexcept>
#include <cmath>


MaRC::Mu0ImageFactory::Mu0ImageFactory(
    std::shared_ptr<BodyData> body,
    double sub_solar_lat,
    double sub_solar_lon)
    : SourceImageFactory()
    , body_(std::move(body))
    , sub_solar_lat_(sub_solar_lat)
    , sub_solar_lon_(sub_solar_lon)
{
}

bool
MaRC::Mu0ImageFactory::populate_parameters(MaRC::map_parameters &) const
{
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
    // p.datamax(mu0_high);
    // p.datamin(mu0_low);

    return true;
}

std::unique_ptr<MaRC::SourceImage>
MaRC::Mu0ImageFactory::make(scale_offset_functor calc_so)
{
    using namespace MaRC::default_configuration;

    double scale;
    double offset;

    if (!calc_so(mu0_low, mu0_high, scale, offset)) {
        throw std::range_error("Cannot store mu0 (cosines) in map of "
                               "chosen data type.");
    }

    /*
      Set physical data extrema if not previously set.

      Scale the default minimum and maximum to match the physical data
      scaling.
    */
    this->minimum(mu0_low  * scale + offset);
    this->maximum(mu0_high * scale + offset);

    return std::make_unique<MaRC::Mu0Image>(this->body_,
                                            this->sub_solar_lat_,
                                            this->sub_solar_lon_,
                                            scale,
                                            offset);
}
