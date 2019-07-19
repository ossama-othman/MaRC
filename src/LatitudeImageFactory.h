// -*- C++ -*-
/**
 * @file LatitudeImageFactory.h
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
        ~LatitudeImageFactory() override = default;

        /// Populate map parameters.
        virtual bool populate_parameters(
            MapParameters & parameters) const override;

        /// Create a @c LatitudeImage.
        std::unique_ptr<SourceImage> make(
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
