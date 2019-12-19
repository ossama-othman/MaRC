/**
 * @file LatitudeImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "LatitudeImage.h"
#include "BodyData.h"
#include "Constants.h"


MaRC::LatitudeImage::LatitudeImage(std::shared_ptr<BodyData> body,
                                   bool graphic_latitudes,
                                   double scale,
                                   double offset)
    : VirtualImage(scale, offset)
    , body_(std::move(body))
    , graphic_latitudes_(graphic_latitudes)
{
}

bool
MaRC::LatitudeImage::read_data_i(double lat,
                                 double /* lon */,
                                 double & data) const
{
    // Valid latitude range is [-90, 90] degrees.
    if (lat < -C::pi_2 || lat > C::pi_2)
        return false;

    if (this->graphic_latitudes_)
        data = this->body_->graphic_latitude(lat);
    else
        data = lat;

    data /= C::degree;  // Convert radians to degrees.

    /**
     * @todo Should we instead return the following?
     *       @code
     *          data >= latitude_low && data <= latitude_high
     *       @endcode
     */
    return true;
}
