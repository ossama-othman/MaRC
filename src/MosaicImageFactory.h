// -*- C++ -*-
/**
 * @file MosaicImageFactory.h
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
     * decouple FITS (for example) file and image operations from the
     * MosaicImage class.  It also exists to decouple the MaRC parser
     * grammar from the MosaicImage class.  This allows MosaicImage
     * object creation to be delayed until it is time for the data in
     * the MosaicImage to be mapped, which reduces run-time memory
     * requirements.
     */
    class MosaicImageFactory final : public ImageFactory
    {
    public:

        /**
         * Type of list containing @c PhotoImageFactory instants that
         * used to create individual mosaic contributors.
         */
        using list_type = std::list<std::unique_ptr<PhotoImageFactory>>;

        /// Constructor.
        MosaicImageFactory(list_type && factories,
                           MosaicImage::average_type type);

        /// Create a @c MosaicImage.
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so);

    private:

        /// List of PhotoImageFactorys
        list_type factories_;

        /// The type of averaging to be performed when multiple images
        /// overlap.
        MosaicImage::average_type const average_type_;

  };

}


#endif  /* MARC_MOSAIC_IMAGE_FACTORY_H */
