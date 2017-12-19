// -*- C++ -*-
/**
 * @file NullInterpolation.h
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

#ifndef MARC_NULL_INTERPOLATION_H
#define MARC_NULL_INTERPOLATION_H

#include "MaRC/InterpolationStrategy.h"
#include "MaRC/Export.h"


namespace MaRC
{

    /**
     * @class NullInterpolation
     *
     * @brief Null (no-op) interpolation strategy.
     *
     * This interpolation strategy is a no-op.  It performs no
     * interpolation.
     */
    class MARC_API NullInterpolation : public InterpolationStrategy
    {
    public:

        /// Destructor.
        virtual ~NullInterpolation();

        /// Performs no interpolation.
        virtual bool interpolate(double const *,
                                 double,
                                 double,
                                 double &) const;

    };

}


#endif  /* MARC_NULL_INTERPOLATION_H */
