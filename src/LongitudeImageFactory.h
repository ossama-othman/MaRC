// -*- C++ -*-
/**
 *  @file LongitudeImageFactory.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
        ~LongitudeImageFactory() override = default;

        /// Create a @c LongitudeImage.
        std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;};

}


#endif  /* MARC_LONGITUDE_IMAGE_FACTORY_H */
