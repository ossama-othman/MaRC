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


int
MaRC::weighted_average::composite(list_type const & images,
                                  double lat,
                                  double lon,
                                  double & data) const
{
    // Weighted sum of data from potentially multiple images at
    // given latitude and longitude.
    long double weighted_data_sum = 0;

    long double weight_sum = 0;

    // Datum count.
    int count = 0;

    for (auto const & i : images) {
        // Physical data weight.
        double weight = 1;

        // Scan for data weight.
        static constexpr bool scan = true;

        if (i->read_data(lat, lon, data, weight, scan)) {
            weighted_data_sum += weight * data;
            weight_sum += weight;
            ++count;
        }
    }

    /*
      Perform the weighted average only if more than one image
      contributed to avoid introducing floating point error, such as
      a datum of 200 for one image vs. 199.999999999996 obtained from
      the weighted average calculation
    */
    if (count > 1 && weight_sum > 0)
        data = static_cast<double>(weighted_data_sum / weight_sum);

    return count;
}
