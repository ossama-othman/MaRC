/**
 * @file CosPhaseImageFactory.cpp
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

#include "CosPhaseImageFactory.h"

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

    // Scale the default minimum and maximum to match the physical
    // data scaling.
    if (std::isnan(this->minimum()))
        this->minimum(cos_phase_low  * scale + offset);

    if (std::isnan(this->maximum()))
        this->maximum(cos_phase_high * scale + offset);

    return std::make_unique<MaRC::CosPhaseImage>(this->body_,
                                                 this->sub_observ_lat_,
                                                 this->sub_observ_lon_,
                                                 this->sub_solar_lat_,
                                                 this->sub_solar_lon_,
                                                 this->range_,
                                                 scale,
                                                 offset);
}
