// -*- C++ -*-
/**
 * @file Mu0ImageFactory.h
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MU0_IMAGE_FACTORY_H
#define MARC_MU0_IMAGE_FACTORY_H

#include "SourceImageFactory.h"


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
    class Mu0ImageFactory final : public SourceImageFactory
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
        ~Mu0ImageFactory() override = default;

        /// Populate map parameters.
        bool populate_parameters(
            map_parameters & parameters) const override;

        /// Create a @c Mu0Image.
        std::unique_ptr<SourceImage> make(
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
