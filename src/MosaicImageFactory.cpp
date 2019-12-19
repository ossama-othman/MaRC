/**
 * @file MosaicImageFactory.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "MosaicImageFactory.h"

#include <stdexcept>


MaRC::MosaicImageFactory::MosaicImageFactory (
    list_type && factories,
    MosaicImage::average_type type)
    : SourceImageFactory()
    , factories_(std::move(factories))
    , average_type_(type)
{
    if (this->factories_.empty())
        throw std::invalid_argument("Empty PhotoImageFactory list.");
}

std::unique_ptr<MaRC::SourceImage>
MaRC::MosaicImageFactory::make(scale_offset_functor calc_so)
{
    MosaicImage::list_type photos;

    for (auto & factory : this->factories_)
        photos.push_back(factory->make(calc_so));

    return
        std::make_unique<MosaicImage>(std::move(photos),
                                      this->average_type_);
}
