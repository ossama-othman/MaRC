// -*- C++ -*-
/**
 * @file compositing_strategy.h
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_COMPOSITING_STRATEGY_H
#define MARC_COMPOSITING_STRATEGY_H

#include <marc/SourceImage.h>
#include <marc/Export.h>

#include <memory>
#include <vector>

namespace MaRC
{

    /**
     * @class compositing_strategy compositing_strategy.h <marc/compositing_strategy.h>
     *
     * @brief Abstract base class for all compositing strategies.
     *
     * Concrete compositing classes must implement the interface
     * required by this abstract base class.
     */
    class MARC_API compositing_strategy
    {
    public:

        /// Type of list containing source images to be composited.
        using list_type = std::vector<std::unique_ptr<SourceImage>>;

        /// Constructor.
        compositing_strategy() = default;

        // Disallow copying.
        compositing_strategy(compositing_strategy const &) = delete;
        compositing_strategy & operator=(
            compositing_strategy const &) = delete;

        // Disallow moving.
        compositing_strategy(compositing_strategy &&) = delete;
        compositing_strategy & operator=(
            compositing_strategy &&) = delete;

        /// Destructor.
        virtual ~compositing_strategy() = default;

        /**
         * @brief Perform compositing on data at given latitude and
         *        longitude.         *

         * @param[in]  images Set of images to composite.
         * @param[in]  lat    Planetocentric latitude in radians.
         * @param[in]  lon    Longitude in radians.
         * @param[out] datum  Composited datum.
         *
         * @return The number of images that were composited.
         */
        virtual int composite(list_type const & images,
                              double lat,
                              double lon,
                              double & data) const = 0;

    };

}

#endif  /* MARC_COMPOSITING_STRATEGY_H */
