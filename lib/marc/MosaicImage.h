// -*- C++ -*-
/**
 * @file MosaicImage.h
 *
 * Copyright (C) 2003-2004, 2017-2018, 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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
     * @class MosaicImage MosaicImage.h <marc/MosaicImage.h>
     *
     * @brief Source image comprised of multiple @c PhotoImages.
     *
     * Mosaics may be comprised of multiple photographs, each taken at
     * different viewing geometries.
     */
    class MARC_API MosaicImage final : public SourceImage
    {
    public:

        /**
         * Type of list containing source images that comprise the
         * mosaic.
         */
        using list_type = std::vector<std::unique_ptr<SourceImage>>;

        /**
         * @enum average_type
         *
         * The type of averaging to be performed on physical data
         * retrieved from multiple images that contain data at the
         * given latitude and longitude.
         */
        enum average_type { AVG_NONE, AVG_UNWEIGHTED, AVG_WEIGHTED };

        typedef bool (*avg_func_type)(list_type const & images,
                                      double lat,
                                      double lon,
                                      double & data);

        /// Constructor.
        /**
         * @param[in,out] images The list of images to be mosaiced.
         *                       Ownership of the list is transferred
         *                       to the @c MosaicImage by peforming a
         *                       C++11 move.
         * @param[in]     type   The type of averaging to be performed.
         */
        MosaicImage(list_type && images, average_type type);

        // Disallow copying and moving.
        MosaicImage(MosaicImage const &) = delete;
        MosaicImage(MosaicImage &&) = delete;
        MosaicImage & operator=(MosaicImage const &) = delete;
        MosaicImage & operator=(MosaicImage &&) = delete;

        /// Destructor.
        ~MosaicImage() override = default;

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
        bool read_data(double lat,
                       double lon,
                       double & data) const override;

    private:

        /// Set of images
        list_type const images_;

        /**
         * @brief Data averaging strategy.
         *
         * This function performs an average on data retrieved from
         * multiple images.  The type of average (e.g. none,
         * unweighted, or weighted) corresponds to the one specified
         * by the user when instantiating this class.
         *
         * @todo Currently this is a simple pointer to function.
         *       An alternative approach would be to implement a class
         *       hiearchy, similar to what is done for interpolation
         *       in MaRC (see @c MaRC::InterpolationStrategy).
         */
        avg_func_type const average_;

    };

} // End MaRC namespace


#endif  /* MARC_MOSAIC_IMAGE_H */
