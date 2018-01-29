// -*- C++ -*-
/**
 * @file CosPhaseImageFactory.h
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

#ifndef MARC_COS_PHASE_IMAGE_FACTORY_H
#define MARC_COS_PHASE_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include <string>


namespace MaRC
{
    class BodyData;

    /**
     * @class CosPhaseImageFactory
     *
     * @brief Factory class that creates CosPhaseImage objects.
     *
     * This class creates CosPhaseImage objects.
     */
    class CosPhaseImageFactory final : public ImageFactory
    {
    public:

        /// Constructor.
        /**
         * @param[in] body           Body being mapped.
         * @param[in] sub_observ_lat Bodycentric sub-observer latitude
         *                           in degrees.
         * @param[in] sub_observ_lon Sub-observer longitude in
         *                           degrees.
         * @param[in] sub_solar_lat  Sub-solar latitude in degrees.
         * @param[in] sub_solar_lon  Sub-solar longitude in degrees.
         * @param[in] range          Observer to target center
         *                           distance.
         */
        CosPhaseImageFactory(std::shared_ptr<BodyData> body,
                             double sub_observ_lat,
                             double sub_observ_lon,
                             double sub_solar_lat,
                             double sub_solar_lon,
                             double range);

        /// Create a @c CosPhaseImage.
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so);

  private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Sub-Observer Latitude -- BodyCENTRIC (degrees).
        double sub_observ_lat_;

        /// Sub-Observer Longitude -- Central Meridian (degrees).
        double sub_observ_lon_;

        /// Sub-Solar Latitude -- BodyCENTRIC (degrees)
        double sub_solar_lat_;

        /// Sub-Solar Longitude (degrees)
        double sub_solar_lon_;

        /// Center of body distance to observer (Kilometers)
        double range_;

    };

}


#endif  /* MARC_COS_PHASE_IMAGE_FACTORY_H */
