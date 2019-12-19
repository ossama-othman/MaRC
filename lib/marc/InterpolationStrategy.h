// -*- C++ -*-
/**
 * @file InterpolationStrategy.h
 *
 * Copyright (C) 2004-2005, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_INTERPOLATION_STRATEGY_H
#define MARC_INTERPOLATION_STRATEGY_H

#include <marc/Export.h>


namespace MaRC
{

    /**
     * @class InterpolationStrategy InterpolationStrategy.h <marc/InterpolationStrategy.h>
     *
     * @brief Abstract base class for all interpolation strategies.
     *
     * Concrete interpolation classes must implement the interface
     * required by this abstract base class.
     */
    class MARC_API InterpolationStrategy
    {
    public:

        InterpolationStrategy() = default;

        // Disallow copying.
        InterpolationStrategy(InterpolationStrategy const &) = delete;
        InterpolationStrategy & operator=(
            InterpolationStrategy const &) = delete;

        /// Destructor.
        virtual ~InterpolationStrategy() = default;

        /// Perform interpolation on given pixel.
        /**
         * The interpolation technique will not include invalid data
         * (e.g. @c NaN) when computing interpolated values.
         *
         * @param[in]  data    The array containing the data to be
         *                     interpolated.
         * @param[in]  x       Floating point sample in image.
         * @param[in]  z       Floating point line in image.
         * @param[out] datum Interpolated datum.
         *
         * @return @c true if interpolation succeeded.
         */
        virtual bool interpolate(double const * data,
                                 double x,
                                 double z,
                                 double & datum) const = 0;

    };

}

#endif  /* MARC_INTERPOLATION_STRATEGY_H */
