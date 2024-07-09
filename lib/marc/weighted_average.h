// -*- C++ -*-
/**
 * @file weighted_average.h
 *
 * Copyright (C) 2021, 2022  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_WEIGHTED_AVERAGE_H
#define MARC_WEIGHTED_AVERAGE_H

#include "marc/compositing_strategy.h"
#include "marc/Export.h"


namespace MaRC
{

    /**
     * @class weighted_average weighted_average.h <marc/weighted_average.h>
     *
     * @brief Weighted average compositing strategy.
     *
     * Composite data through a weighted average.
     */
    class MARC_API weighted_average final : public compositing_strategy
    {
    public:

        /// Constructor.
        weighted_average() = default;

        // Disallow copying.
        weighted_average(weighted_average const &) = delete;
        weighted_average & operator=(weighted_average const &) = delete;

        // Disallow moving.
        weighted_average(weighted_average &&) = delete;
        weighted_average & operator=(weighted_average &&) = delete;

        /// Destructor.
        ~weighted_average() override = default;

        /**
         * @brief Average data at given latitude and longitude.
         *
         * @see @c compositing_strategy for parameter details.
         */
        int composite(list_type const & images,
                      double lat,
                      double lon,
                      double & data) const override;

    };

}


#endif  /* MARC_WEIGHTED_AVERAGE_H */
