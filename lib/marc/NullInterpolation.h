// -*- C++ -*-
/**
 * @file NullInterpolation.h
 *
 * Copyright (C) 2004-2005, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_NULL_INTERPOLATION_H
#define MARC_NULL_INTERPOLATION_H

#include "marc/InterpolationStrategy.h"
#include "marc/Export.h"


namespace MaRC
{

    /**
     * @class NullInterpolation NullInterpolation.h <marc/NullInterpolation.h>
     *
     * @brief Null (no-op) interpolation strategy.
     *
     * This interpolation strategy is a no-op.  It performs no
     * interpolation.
     */
    class MARC_API NullInterpolation final : public InterpolationStrategy
    {
    public:

        /// Destructor.
        ~NullInterpolation() override = default;

        /// Performs no interpolation.
        bool interpolate(double const *,
                         double,
                         double,
                         double &) const override;

    };

}


#endif  /* MARC_NULL_INTERPOLATION_H */
