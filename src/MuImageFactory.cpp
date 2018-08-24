/**
 * @file MuImageFactory.cpp
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

#include "MuImageFactory.h"

#include "marc/MuImage.h"
#include "marc/DefaultConfiguration.h"

#include <stdexcept>


MaRC::MuImageFactory::MuImageFactory(std::shared_ptr<BodyData> body,
                                     double sub_observ_lat,
                                     double sub_observ_lon,
                                     double range)
    : ImageFactory()
    , body_(body)
    , sub_observ_lat_(sub_observ_lat)
    , sub_observ_lon_(sub_observ_lon)
    , range_(range)
{
    using namespace MaRC::default_configuration;
    this->minimum(mu_low);
    this->maximum(mu_high);
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

    return std::make_unique<MaRC::MuImage>(this->body_,
                                           this->sub_observ_lat_,
                                           this->sub_observ_lon_,
                                           this->range_,
                                           scale,
                                           offset);
}
