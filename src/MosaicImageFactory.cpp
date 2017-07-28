/**
 * @file MosaicImageFactory.cpp
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

#include "MosaicImageFactory.h"


MaRC::MosaicImageFactory::MosaicImageFactory (
    list_type factories,
    MosaicImage::average_type type)
    : factories_(std::move(factories))
    , average_type_(type)
{
}

std::unique_ptr<MaRC::SourceImage>
MaRC::MosaicImageFactory::make()
{
    MosaicImage::list_type photos;

    for (auto & factory : this->factories_)
        photos.push_back(factory->make());

    return
        std::make_unique<MosaicImage>(std::move(photos),
                                      this->average_type_);
}
