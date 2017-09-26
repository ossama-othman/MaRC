// -*- C++ -*-
/**
 * @file NullInterpolationStrategy.h
 *
 * Copyright (C) 2004-2005, 2017  Ossama Othman
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

#ifndef MARC_NULL_INTERPOLATION_STRATEGY_H
#define MARC_NULL_INTERPOLATION_STRATEGY_H

#include "InterpolationStrategy.h"


namespace MaRC
{

    /**
     * @class NullInterpolationStrategy
     *
     * @brief Null (no-op) interpolation strategy.
     *
     * This interpolation strategy is a no-op.  It performs no
     * interpolation.
     */
    class NullInterpolationStrategy : public InterpolationStrategy
    {
    public:

        /// Destructor.
        virtual ~NullInterpolationStrategy();

        /// Performs no interpolation.
        virtual bool interpolate(double const *,
                                 std::size_t,
                                 std::size_t,
                                 double,
                                 double,
                                 double &) const;

    };

}


#endif  /* MARC_NULL_INTERPOLATION_STRATEGY_H */
