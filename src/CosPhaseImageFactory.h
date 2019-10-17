// -*- C++ -*-
/**
 * @file CosPhaseImageFactory.h
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_COS_PHASE_IMAGE_FACTORY_H
#define MARC_COS_PHASE_IMAGE_FACTORY_H

#include "SourceImageFactory.h"

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
    class CosPhaseImageFactory final : public SourceImageFactory
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param[in] body           Body being mapped.
         * @param[in] sub_observ_lat Planetocentric sub-observer
         *                           latitude  in degrees.
         * @param[in] sub_observ_lon Sub-observer longitude in
         *                           degrees.
         * @param[in] sub_solar_lat  Planetocentric sub-solar latitude
         *                           in degrees.
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

        /// Destructor.
        ~CosPhaseImageFactory() override = default;

        /// Populate map parameters.
        bool populate_parameters(
            MapParameters & parameters) const override;

        /// Create a @c CosPhaseImage.
        std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;

  private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Sub-Observer Latitude -- PlanetoCENTRIC (degrees).
        double sub_observ_lat_;

        /// Sub-Observer Longitude -- Central Meridian (degrees).
        double sub_observ_lon_;

        /// Sub-Solar Latitude -- PlanetoCENTRIC (degrees)
        double sub_solar_lat_;

        /// Sub-Solar Longitude (degrees)
        double sub_solar_lon_;

        /// Center of body distance to observer (Kilometers)
        double range_;

    };

}


#endif  /* MARC_COS_PHASE_IMAGE_FACTORY_H */
