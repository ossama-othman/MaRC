// -*- C++ -*-
/**
 * @file LatitudeImageFactory.h
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
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

#ifndef MARC_LATITUDE_IMAGE_FACTORY_H
#define MARC_LATITUDE_IMAGE_FACTORY_H

#include "SourceImageFactory.h"


namespace MaRC
{
    class BodyData;

    /**
     * @class LatitudeImageFactory
     *
     * @brief Factory class that creates @c LatitudeImage objects.
     *
     * This class creates @c LatitudeImage objects.
     */
    class LatitudeImageFactory final : public SourceImageFactory
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param[in] body              Pointer to BodyData object
         *                              representing body being
         *                              mapped.
         * @param[in] graphic_latitudes Return planetographic
         *                              latitudes instead of
         *                              planetocentric latitudes.
         */
        LatitudeImageFactory(std::shared_ptr<BodyData> body,
                             bool graphic_latitudes);

        /// Destructor.
        virtual ~LatitudeImageFactory() = default;

        /// Create a @c LatitudeImage.
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Flag that determines if planetographic latitudes are
        /// returned instead of planetocentric latitudes.
        bool const graphic_latitudes_;

    };

}


#endif  /* MARC_LATITUDE_IMAGE_FACTORY_H */
