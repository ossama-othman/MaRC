// -*- C++ -*-
/**
 * @file unweighted_average.h
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_UNWEIGHTED_AVERAGE_H
#define MARC_UNWEIGHTED_AVERAGE_H

#include "marc/compositing_strategy.h"
#include "marc/Export.h"


namespace MaRC
{

    /**
     * @class unweighted_average unweighted_average.h <marc/unweighted_average.h>
     *
     * @brief Unweighted average compositing strategy.
     *
     * Composite data through an unweighted average.
     */
    class MARC_API unweighted_average final : public compositing_strategy
    {
    public:

        /// Destructor.
        ~unweighted_average() override = default;

        /**
         * @brief Average data at given latitude and longitude.
         *
         * @see @c compositing_strategy for parameter details.
         */
        bool composite(list_type const & images,
                       double lat,
                       double lon,
                       double & data) const override;

    };

}


#endif  /* MARC_UNWEIGHTED_AVERAGE_H */