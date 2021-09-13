/**
 * @file MosaicImage.cpp
 *
 * Copyright (C) 2003-2004, 2017, 2020-2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "MosaicImage.h"
#include "PhotoImage.h"


namespace
{
    bool
    no_average(MaRC::MosaicImage::list_type const & images,
               double lat,
               double lon,
               double & data)
    {
        // No averaging.  Just return the first found datum.
        for (auto const & i : images)
            if (i->read_data(lat, lon, data))
                return true;

        return false;
    }

    bool
    unweighted_average(MaRC::MosaicImage::list_type const & images,
                       double lat,
                       double lon,
                       double & data)
    {
        /**
         * @todo We use a @c long @c double variable to store the data
         *       sum to avoid a potential overflow during the
         *       addition.  Alternatively, calculating the mean
         *       iteratively could instead be used to avoid the
         *       potential overflow like so:
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

        if (count > 0) {
            // Calculate the average.
            data = static_cast<double>(sum / count);

            return true;
        }

        return false;
    }

    bool
    weighted_average(MaRC::MosaicImage::list_type const & images,
                     double lat,
                     double lon,
                     double & data)
    {
        // Weighted sum of data from potentially multiple images at
        // given latitude and longitude.
        long double weighted_data_sum = 0;

        // Sum of shortest distances.
        unsigned long long weight_sum = 0;

        for (auto const & i : images) {
            // Shortest distance to an edge of the source image or a
            // blank value in the source image.  This is used as a
            // weight for the datum.
            std::size_t weight = 1;

            // Scan for data weight.
            static constexpr bool scan = true;

            /**
             * @todo Move weight calculation code from @c PhotoImage
             *       to @c MosaicImage.
             */
            if (i->read_data(lat,
                             lon,
                             data,
                             weight,
                             scan)) {
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

    MaRC::MosaicImage::avg_func_type
    get_average_function(MaRC::MosaicImage::list_type const & images,
                         MaRC::MosaicImage::average_type type)
    {
        if (images.size() < 2) {
            /*
              Note that we override and disable averaging if only one
              image exists in the image set.  There is no point in
              attempting averaging in that case.
            */
            return no_average;
        }

        switch (type) {
        case MaRC::MosaicImage::AVG_NONE:
        default:
            return no_average;
        case MaRC::MosaicImage::AVG_UNWEIGHTED:
            return unweighted_average;
        case MaRC::MosaicImage::AVG_WEIGHTED:
            return weighted_average;
        }
    }
}

// ---------------------------------------------------------------------

MaRC::MosaicImage::MosaicImage(list_type && images,
                               average_type type)
    : images_(std::move(images))
    , average_(get_average_function(images_, type))
{
}

bool
MaRC::MosaicImage::read_data(double lat,
                             double lon,
                             double & data) const
{
    return this->average_(this->images_, lat, lon, data);
}
