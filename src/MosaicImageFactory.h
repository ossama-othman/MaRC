// -*- C++ -*-
/**
 * @file MosaicImageFactory.h
 *
 * Copyright (C) 2004, 2017, 2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MOSAIC_IMAGE_FACTORY_H
#define MARC_MOSAIC_IMAGE_FACTORY_H

#include "PhotoImageFactory.h"

#include "marc/MosaicImage.h"

#include <list>


namespace MaRC
{

    /**
     * @class MosaicImageFactory
     *
     * @brief Factory class that create MosaicImage objects.
     *
     * This class creates MosaicImage objects.  It is designed to
     * decouple %FITS (for example) file and image operations from the
     * MosaicImage class.  It also exists to decouple the %MaRC parser
     * grammar from the MosaicImage class.  This allows MosaicImage
     * object creation to be delayed until it is time for the data in
     * the MosaicImage to be mapped, which reduces run-time memory
     * requirements.
     */
    class MosaicImageFactory final : public SourceImageFactory
    {
    public:

        /**
         * Type of list containing @c PhotoImageFactory instances that
         * used to create individual mosaic contributors.
         */
        using list_type = std::list<std::unique_ptr<PhotoImageFactory>>;

        /// Constructor.
        MosaicImageFactory(list_type && factories,
                           MosaicImage::average_type type);

        /// Destructor.
        ~MosaicImageFactory() override = default;

        /// Populate map parameters.
        virtual bool populate_parameters(
            MapParameters & parameters) const override;

        /// Create a @c MosaicImage.
        std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;

    private:

        /// List of PhotoImageFactory objects.
        list_type factories_;

        /// The type of averaging to be performed when multiple images
        /// overlap.
        MosaicImage::average_type const average_type_;

  };

}


#endif  /* MARC_MOSAIC_IMAGE_FACTORY_H */
