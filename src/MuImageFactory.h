// -*- C++ -*-
/**
 * @file MuImageFactory.h
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MU_IMAGE_FACTORY_H
#define MARC_MU_IMAGE_FACTORY_H

#include "SourceImageFactory.h"


namespace MaRC
{
    class BodyData;

    /**
     * @class MuImageFactory
     *
     * @brief Factory class that creates MuImage objects.
     *
     * This class creates @c MuImage objects.
     */
    class MuImageFactory final : public SourceImageFactory
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param[in] body           Body being mapped.
         * @param[in] sub_observ_lat Planetocentric sub-observer
         *                           latitude in degrees.
         * @param[in] sub_observ_lon Sub-observer longitude in
         *                           degrees.
         * @param[in] range          Observer to target center
         *                           distance.
         */
        MuImageFactory(std::shared_ptr<BodyData> body,
                       double sub_observ_lat,
                       double sub_observ_lon,
                       double range);

        /// Destructor.
        ~MuImageFactory() override = default;

        /// Populate map parameters.
        bool populate_parameters(
            map_parameters & parameters) const override;

        /// Create a @c MuImage.
        std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Sub-Observer Latitude -- PlanetoCENTRIC (degrees).
        double sub_observ_lat_;

        /// Sub-Observer Longitude -- Central Meridian (degrees).
        double sub_observ_lon_;

        /// Center of body distance to observer (Kilometers)
        double range_;

    };

}


#endif  /* MARC_MU_IMAGE_FACTORY_H */
