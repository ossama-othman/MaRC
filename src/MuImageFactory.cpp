/**
 * @file MuImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "MuImageFactory.h"
#include "MapParameters.h"

#include "marc/MuImage.h"
#include "marc/DefaultConfiguration.h"

#include <stdexcept>
#include <cmath>


MaRC::MuImageFactory::MuImageFactory(std::shared_ptr<BodyData> body,
                                     double sub_observ_lat,
                                     double sub_observ_lon,
                                     double range)
    : SourceImageFactory()
    , body_(std::move(body))
    , sub_observ_lat_(sub_observ_lat)
    , sub_observ_lon_(sub_observ_lon)
    , range_(range)
{
}

bool
MaRC::MuImageFactory::populate_parameters(MaRC::MapParameters &p) const
{
    using namespace MaRC::default_configuration;

    p.datamax(mu_high);
    p.datamin(mu_low);

    return true;
}


std::unique_ptr<MaRC::SourceImage>
MaRC::MuImageFactory::make(scale_offset_functor calc_so)
{
    using namespace MaRC::default_configuration;

    double scale;
    double offset;

    if (!calc_so(mu_low, mu_high, scale, offset)) {
        throw std::range_error("Cannot store mu (cosines) in map of "
                               "chosen data type.");
    }

    // Scale the default minimum and maximum to match the physical
    // data scaling.
    if (std::isnan(this->minimum()))
        this->minimum(mu_low  * scale + offset);

    if (std::isnan(this->maximum()))
        this->maximum(mu_high * scale + offset);

    return std::make_unique<MaRC::MuImage>(this->body_,
                                           this->sub_observ_lat_,
                                           this->sub_observ_lon_,
                                           this->range_,
                                           scale,
                                           offset);
}
