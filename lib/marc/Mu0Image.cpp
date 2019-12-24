/**
 * @file Mu0Image.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "Mu0Image.h"
#include "BodyData.h"
#include "Constants.h"
#include "config.h"  // For NDEBUG.

#include <cassert>


MaRC::Mu0Image::Mu0Image(std::shared_ptr<BodyData> body,
                         double sub_solar_lat,
                         double sub_solar_lon,
                         double scale,
                         double offset)
    : VirtualImage(scale, offset)
    , body_(std::move(body))
    , sub_solar_lat_(sub_solar_lat * C::degree)  // Radians
    , sub_solar_lon_(sub_solar_lon * C::degree)  // Radians
{
}

bool
MaRC::Mu0Image::read_data_i(double lat, double lon, double & data) const
{
    /*
      An illuminated point at the given latitude and longitude will
      have an incidence angle in the interval [-90, 90], i.e. mu0 >= 0
      where mu0 is the cosine of the incidence angle.  Points that are
      not illuminated will have a value of mu0 < 0.
    */
    data = this->body_->mu0(this->sub_solar_lat_,
                            this->sub_solar_lon_,
                            lat,
                            lon);

    assert(data >= -1 && data <= 1);

    return true;
}
