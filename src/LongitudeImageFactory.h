// -*- C++ -*-
/**
 *  @file LongitudeImageFactory.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#ifndef MARC_LONGITUDE_IMAGE_FACTORY_H
#define MARC_LONGITUDE_IMAGE_FACTORY_H

#include "SourceImageFactory.h"


namespace MaRC
{

    /**
     * @class LongitudeImageFactory
     *
     * @brief Factory class that creates LongitudeImage objects.
     *
     * This class creates LongitudeImage objects.
     */
    class LongitudeImageFactory final : public SourceImageFactory
    {
    public:

        /// Constructor.
        LongitudeImageFactory();

        /// Destructor.
        virtual ~LongitudeImageFactory() = default;

        /// Create a @c LongitudeImage.
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;

    };

}


#endif  /* MARC_LONGITUDE_IMAGE_FACTORY_H */
