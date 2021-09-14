/**
 * @file MosaicImage.cpp
 *
 * Copyright (C) 2003-2004, 2017, 2020-2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "MosaicImage.h"
#include "PhotoImage.h"


MaRC::MosaicImage::MosaicImage(
    list_type && images,
    std::unique_ptr<compositing_strategy> compositor)
    : images_(std::move(images))
    , compositor_(std::move(compositor))
{
}

bool
MaRC::MosaicImage::read_data(double lat,
                             double lon,
                             double & data) const
{
    return this->compositor_->composite(this->images_, lat, lon, data);
}
