// -*- C++ -*-
/**
 * @file Mu0ImageFactory.h
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

#ifndef MARC_MU0_IMAGE_FACTORY_H
#define MARC_MU0_IMAGE_FACTORY_H

#include "ImageFactory.h"


namespace MaRC
{
    class BodyData;

    /**
     * @class Mu0ImageFactory
     *
     * @brief Factory class that creates Mu0Image objects.
     *
     * This class creates Mu0Image objects.
     */
    class Mu0ImageFactory final : public ImageFactory
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param[in] body          Body being mapped.
         * @param[in] sub_solar_lat Planetocentric sub-solar latitude
         *                          in degrees.
         * @param[in] sub_solar_lon Sub-solar longitude in degrees.
         */
        Mu0ImageFactory(std::shared_ptr<BodyData> body,
                        double sub_solar_lat,
                        double sub_solar_lon);

        /// Destructor.
        virtual ~Mu0ImageFactory() = default;

        /// Create a @c Mu0Image.
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Planetocentric sub-solar latitude (degrees).
        double sub_solar_lat_;

        /// Sub-solar longitude (degrees).
        double sub_solar_lon_;

    };

}


#endif  /* MARC_MU0_IMAGE_FACTORY_H */
