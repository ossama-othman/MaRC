// -*- C++ -*-
/**
 * @file first_read.h
 *
 * Copyright (C) 2021, 2022  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_FIRST_READ_H
#define MARC_FIRST_READ_H

#include "marc/compositing_strategy.h"
#include "marc/Export.h"


namespace MaRC
{

    /**
     * @class first_read first_read.h <marc/first_read.h>
     *
     * @brief First read datum compositing strategy.
     *
     * Compositing strategy that simply returns the first read datum
     * from the image set.
     */
    class MARC_API first_read final : public compositing_strategy
    {
    public:

        /// Constructor.
        first_read() = default;

        // Disallow copying.
        first_read(first_read const &) = delete;
        first_read & operator=(first_read const &) = delete;

        // Disallow moving.
        first_read(first_read &&) = delete;
        first_read & operator=(first_read &&) = delete;

        /// Destructor.
        ~first_read() override = default;

        /**
         * @brief Return first read datum at given latitude and longitude.
         *
         * @see @c compositing_strategy for parameter details.
         */
        int composite(list_type const & images,
                      double lat,
                      double lon,
                      double & data) const override;

    };

}


#endif  /* MARC_FIRST_READ_H */
