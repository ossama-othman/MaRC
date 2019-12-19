/**
 * @file VirtualImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "VirtualImage.h"


MaRC::VirtualImage::VirtualImage(double s, double o)
    : scale_(s)
    , offset_(o)
{
}

bool
MaRC::VirtualImage::read_data(double lat, double lon, double & data) const
{
    /**
     * @todo Validate @a lat and @a lon.
     */

    bool const visible = this->read_data_i(lat, lon, data);

    if (visible)
        data = (data * this->scale_) + this->offset_;

    return visible;
}
