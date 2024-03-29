/**
 * @file unweighted_average.cpp
 *
 * Copyright (C) 2003-2004, 2017, 2020-2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "unweighted_average.h"


int
MaRC::unweighted_average::composite(list_type const & images,
                                    double lat,
                                    double lon,
                                    double & data) const
{
    /**
     * @todo We use a @c long @c double variable to store the data sum
     *       to avoid a potential overflow during the addition.
     *       Alternatively, calculating the mean iteratively could
     *       instead be used to avoid the potential overflow like so:
     * @code
     * double average = 0;
     * int count = 0;
     * for (auto const & i : images) {
     *     if (i->read_data(lat, lon, data)) {
     *         average += (data - average) / count;
     *         ++count;
     *     }
     * }
     *
     * data = average;
     * @endcode
     * @see http://www.heikohoffmann.de/htmlthesis/node134.html
     */

    // Sum of data from potentially multiple images at given
    // latitude and longitude.
    long double sum = 0;

    // Datum count.
    int count = 0;

    for (auto const & i : images) {
        if (i->read_data(lat, lon, data)) {
            sum += data;
            ++count;
        }
    }

    /*
      Perform the average only if more than one image contributed to
      avoid introducing floating point error, such as a datum of 200
      for one image vs. 199.999999999996 obtained from the average
      calculation.
    */
    if (count > 1) {
        // Calculate the average.
        data = static_cast<double>(sum / count);
    }

    return count;
}
