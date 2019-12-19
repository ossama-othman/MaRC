/**
 * @file LongitudeImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "LongitudeImage.h"
#include "Constants.h"
#include "DefaultConfiguration.h"

#include <cmath>


MaRC::LongitudeImage::LongitudeImage(double scale, double offset)
    : VirtualImage(scale, offset)
{
}

bool
MaRC::LongitudeImage::read_data_i(double /* lat */,
                                  double lon,
                                  double & data) const
{
    using namespace MaRC::default_configuration;

    data = lon / C::degree;  // Convert radians to degrees.

    // Make sure the longitude is in the +/-360 degree range.  Note
    // that fmod() retains the sign of its first argument.
    data = std::fmod(data, 360);

    // Data is now in the +/-360 range but we need it to be between
    // [0, 360] or [-180, 180], for example, depending on the
    // configuration.  Shift the longitude by 360 degrees as needed in
    // an attempt to bring it within the configured range.
    /**
     * @note We would only need to check if @c data less then zero for
     *       the [0,360] case but we also need to support the case
     *       where @c longitude_high is less than 360, such as
     *       [-180,180], which is why we we have the "@c else if"
     *       clause below.
     */
    if (data < longitude_low)
        data += 360;
    else if (data > longitude_high)
        data -= 360;

    return data >= longitude_low && data <= longitude_high;
}
