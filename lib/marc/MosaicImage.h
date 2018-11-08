// -*- C++ -*-
/**
 * @file MosaicImage.h
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#ifndef MARC_MOSAIC_IMAGE_H
#define MARC_MOSAIC_IMAGE_H

#include <marc/SourceImage.h>
#include <marc/Export.h>

#include <memory>
#include <vector>


namespace MaRC
{
    /**
     * @class MosaicImage
     *
     * @brief Source image comprised of multiple @c PhotoImages.
     *
     * Mosaics may be comprised of multiple photographs, each taken at
     * different viewing geometries.
     */
    class MARC_API MosaicImage final : public SourceImage
    {
    public:

        using list_type = std::vector<std::unique_ptr<SourceImage>>;

        /**
         * @enum AverageType
         *
         * The type of averaging to be performed on physical data
         * retrieved from multiple images that contain data at the
         * given latitude and longitude.
         */
        enum average_type { AVG_NONE, AVG_UNWEIGHTED, AVG_WEIGHTED };

        /// Constructor.
        /**
         * @param[in,out] images The list of images to be mosaiced.
         *                       Ownership of the list is transferred
         *                       to the @c MosaicImage by peforming a
         *                       C++11 move.
         * @param[in]     type   The type of averaging to be performed.
         */
        MosaicImage(list_type && images, average_type type);

        /// Retrieve physical data from mosaic images.
        /**
         * Retrieve physical data from all mosaic images that have
         * data at the given latitude and longitude.  The configured
         * data averaging strategy will be applied in cases where
         * multiple images have data at the given longitude and
         * latitude.
         *
         * @param[in]  lat  Planetocentric latitude in radians.
         * @param[in]  lon  Longitude in radians.
         * @param[out] data Physical data retrieved from image.
         *
         * @retval true  Physical data retrieved,
         * @retval false No physical data retrieved.
         *
         * @see @c MaRC::SourceImage::read_data();
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const;

    private:

        /// Set of images
        list_type const images_;

        /// The type of averaging to perform on data retrieved from
        /// multiple images.
        average_type const average_type_;

    };

} // End MaRC namespace


#endif  /* MARC_MOSAIC_IMAGE_H */