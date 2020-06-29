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

    // Make sure the longitude is in the +/-360 degree range.  Note
    // that fmod() retains the sign of its first argument.
    data = std::fmod(lon, C::circle) / C::degree;

    // Data is now in the +/-360 range but we need it to be between
    // [0, 360] or [-180, 180], for example, depending on the
    // configuration.  Shift the longitude by 360 degrees as needed in
    // an attempt to bring it within the configured range.
    /**
     * @note We would only need to check if @c data less then zero for
     *       the [0,360] case but we also need to support the case
     *       where @c longitude_high is less than 360, such as
     *       [-180,180], which is why we have the "@c else @c if"
     *       clause below.
     */
    constexpr int circle_degrees = 360;
    if (data < longitude_low)
        data += circle_degrees;
    else if (data > longitude_high)
        data -= circle_degrees;

    return data >= longitude_low && data <= longitude_high;
}
