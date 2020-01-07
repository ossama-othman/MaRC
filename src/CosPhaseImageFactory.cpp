/**
 * @file CosPhaseImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2019-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "CosPhaseImageFactory.h"
#include "MapParameters.h"

#include "marc/CosPhaseImage.h"
#include "marc/DefaultConfiguration.h"

#include <stdexcept>
#include <cmath>


MaRC::CosPhaseImageFactory::CosPhaseImageFactory(
    std::shared_ptr<BodyData> body,
    double sub_observ_lat,
    double sub_observ_lon,
    double sub_solar_lat,
    double sub_solar_lon,
    double range)
    : SourceImageFactory()
    , body_(std::move(body))
    , sub_observ_lat_(sub_observ_lat)
    , sub_observ_lon_(sub_observ_lon)
    , sub_solar_lat_(sub_solar_lat)
    , sub_solar_lon_(sub_solar_lon)
    , range_(range)
{
}

bool
MaRC::CosPhaseImageFactory::populate_parameters(
    MaRC::MapParameters &) const
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
    // using namespace MaRC::default_configurati
    // p.datamax(cos_phase_high);
    // p.datamin(cos_phase_low);

    return true;
}

std::unique_ptr<MaRC::SourceImage>
MaRC::CosPhaseImageFactory::make(scale_offset_functor calc_so)
{
    using namespace MaRC::default_configuration;

    double scale;
    double offset;

    if (!calc_so(cos_phase_low, cos_phase_high, scale, offset)) {
        throw std::range_error("Cannot store cosine of phase angles in "
                               "map of chosen datatype.");
    }

    // Set physical data extrema if not previously set.
    if (!this->extrema_.is_valid()) {
        // Scale the default minimum and maximum to match the physical
        // data scaling.
        this->extrema_.update(cos_phase_low  * scale + offset);
        this->extrema_.update(cos_phase_high * scale + offset);
    }

    return std::make_unique<MaRC::CosPhaseImage>(this->body_,
                                                 this->sub_observ_lat_,
                                                 this->sub_observ_lon_,
                                                 this->sub_solar_lat_,
                                                 this->sub_solar_lon_,
                                                 this->range_,
                                                 scale,
                                                 offset);
}
