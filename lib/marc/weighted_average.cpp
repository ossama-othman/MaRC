/**
 * @file weighted_average.cpp
 *
 * Copyright (C) 2003-2004, 2017, 2020-2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "weighted_average.h"


bool
MaRC::weighted_average::composite(list_type const & images,
                                  double lat,
                                  double lon,
                                  double & data) const
{
    // Weighted sum of data from potentially multiple images at
    // given latitude and longitude.
    long double weighted_data_sum = 0;

    // Sum of shortest distances.
    unsigned long long weight_sum = 0;

    for (auto const & i : images) {
        /*
          Shortest distance to an edge of the source image or a blank
          value in the source image.  This is used as a weight for the
          datum.
        */
        std::size_t weight = 1;

        // Scan for data weight.
        static constexpr bool scan = true;

        /**
         * @todo Move weight calculation code from @c PhotoImage
         *       to @c MosaicImage.
         */
        if (i->read_data(lat, lon, data, weight, scan)) {
            weighted_data_sum += weight * data;
            weight_sum += weight;
        }
    }

    // Perform the average.
    if (weight_sum > 0)  {
        data = static_cast<double>(weighted_data_sum / weight_sum);

        return true;
    }

    return false;
}
