/**
 * MosaicImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "MosaicImage.h"
#include "PhotoImage.h"
#include "Mathematics.h"


MaRC::MosaicImage::MosaicImage(list_type && images,
                               average_type type)
    : images_(std::move(images))
    , average_type_(images_.size() < 2 ? AVG_NONE : type)
{
    // Note that we override and disable averaging if only one image
    // exists in the image set.  There is no point in attempting
    // averaging in that case.
}

bool
MaRC::MosaicImage::read_data(double lat,
                             double lon,
                             double & data) const
{
    bool found_data = false;

    // Weighted sum of data from potentially multiple images at given
    // latitude and longitude.
    long double weighted_data_sum = 0;

    // Sum of shortest distances.
    unsigned long long weight_sum = 0;

    for (auto const & i : this->images_) {
        /**
         * @todo Find a more elegant way to perform averaging than
         *       this switch statement.
         */
        switch (this->average_type_) {
        case AVG_WEIGHTED:  // Weighted averaging.
            {
                // Shortest distance to an edge of the source image or
                // a blank value in the source image.  This is used as
                // a weight when using weighted averaging.
                std::size_t weight = 1;

                // Scan for data weight.
                static constexpr bool scan = true;

                if (i->read_data(lat,
                                 lon,
                                 data,
                                 weight,
                                 scan)) {
                    weighted_data_sum += weight * data;
                    weight_sum += weight;

                    found_data = true;
                }
            }

            break;

        case AVG_UNWEIGHTED:  // Unweighted averaging.
            if (i->read_data(lat, lon, data)) {
                weighted_data_sum += data;
                ++weight_sum;

                found_data = true;
            }

            break;

        default:      // No averaging
            if (i->read_data(lat, lon, data)) {
                found_data = true;

                // No point in continuing.  Just return the first
                // found datum.
                return found_data;
            }

          break;
        }
    }

    // Perform the average.
    if (weight_sum > 0)
        data = static_cast<double>(weighted_data_sum / weight_sum);

    return found_data;
}
