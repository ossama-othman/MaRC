/**
 * MosaicImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#include "MosaicImage.h"
#include "PhotoImage.h"
#include "Mathematics.h"

#include <cmath>


MaRC::MosaicImage::MosaicImage(list_type && images,
                               average_type type)
    : images_(std::move(images))
    , average_type_(images.size() < 2 ? AVG_NONE : type)
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

    // Buffer holding weighted sum for a given pixel.
    long double weighted_data_sum = 0;

    // Sum of shortest distances.
    unsigned long long weight_sum = 0;

    for (auto const & i : this->images_) {
        /**
         * @todo Find a more elegant way to peform averaging than this
         *       switch statement.
         */
        switch (this->average_type_) {
        case AVG_WEIGHTED:  // Weighted averaging.
            {
                // Shortest distance to an edge of the source image or
                // a blank value in the source image.  This is used as
                // a weight when using weighted averaging.
                std::size_t weight = 1;

                // Scan for data weight.
                static bool const scan = true;

                if (i->read_data(lat,
                                 lon,
                                 data,
                                 weight,
                                 scan)
                    && !MaRC::almost_zero(data, 5)) {
                    weighted_data_sum += weight * data;
                    weight_sum += weight;

                    found_data = true;
                }
            }

            break;

        case AVG_UNWEIGHTED:  // Unweighted averaging.
            if (i->read_data(lat, lon, data)
                && !MaRC::almost_zero(data, 5)) {
                weighted_data_sum += data;
                ++weight_sum;

                found_data = true;
            }

            break;

        default:      // No averaging
            if (i->read_data(lat, lon, data)
                && !MaRC::almost_zero(data, 5)) {
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
