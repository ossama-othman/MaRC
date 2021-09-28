/**
 * @file SourceImage.cpp
 *
 * Copyright (C) 1999, 2003-2004, 2017, 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "SourceImage.h"


bool
MaRC::SourceImage::read_data(double lat,
                             double lon,
                             double & data,
                             double & /* weight */,
                             bool /* scan */) const
{
    return this->read_data(lat, lon, data);
}
