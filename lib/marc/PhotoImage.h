//   -*- C++ -*-
/**
 * @file PhotoImage.h
 *
 * Copyright (C) 1999, 2003-2005, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PHOTO_IMAGE_H
#define MARC_PHOTO_IMAGE_H

#include <marc/SourceImage.h>
#include <marc/Export.h>

#include <memory>
#include <vector>


namespace MaRC
{
    class PhotoImageParameters;
    class ViewingGeometry;

    /**
     * @class PhotoImage PhotoImage.h <marc/PhotoImage.h>
     *
     * @brief Concrete SourceImage strategy for mapping bodies in
     *        photos.
     *
     * PhotoImages have viewing geometries that may differ from other
     * photos of the same body being mapped.  For example, photos from
     * telescope observations fit into this category.
     */
    class MARC_API PhotoImage final : public SourceImage
    {
    public:

        /// Constructor
        /**
         * @param[in,out] image    Array containing the image data.
         *                         Ownership is transferred to the
         *                         @c PhotoImage.
         * @param[in]     samples  Number of samples in the image.
         * @param[in]     lines    Number of lines   in the image.
         * @param[in,out] config   Configuration parameters specific
         *                         to a @c PhotoImage.  Ownership is
         *                         transferred to the @c PhotoImage.
         * @param[in,out] geometry Viewing geometry for the photo
         *                         image data encapsulated by this
         *                         @c PhotoImage object.  Ownership is
         *                         transferred to the @c PhotoImage.
         */
        PhotoImage(std::vector<double> && image, // moved, not copied!
                   std::size_t samples,
                   std::size_t lines,
                   std::unique_ptr<PhotoImageParameters> config,
                   std::unique_ptr<ViewingGeometry> geometry);

        // Disallow copying.
        PhotoImage(PhotoImage const &) = delete;
        PhotoImage & operator=(PhotoImage const &) = delete;

        // Disallow moving.
        PhotoImage(PhotoImage &&) = delete;
        PhotoImage & operator=(PhotoImage &&) = delete;

        /// Destructor.
        ~PhotoImage() override = default;

        /// Retrieve physical data from source image.
        /**
         * Retrieve physical data from source image. The configured
         * data interpolation strategy will be applied.
         *
         * @see MaRC::SourceImage::read_data().
         */
        bool read_data(double lat,
                       double lon,
                       double & data) const override;

        /// Retrieve physical data and weight from source image.
        /**
         * Retrieve physical data and weight from source image.  The
         * configured data interpolation strategy will be applied.
         *
         * @param[in]     lat    Planetocentric latitude in radians.
         * @param[in]     lon    Longitude in radians.
         * @param[out]    data   Physical data retrieved from image.
         * @param[in,out] weight Distance from pixel to closest edge
         *                       or blank pixel.
         * @param[in]     scan   Flag that determines if a data weight
         *                       scan is performed.  It is generally
         *                       only used by the version of
         *                       @c read_data() that does not return a
         *                       weight.
         *
         * @retval true  Physical data retrieved.
         * @retval false No physical data retrieved.
         */
        bool read_data(double lat,
                       double lon,
                       double & data,
                       std::size_t & weight,
                       bool scan = true) const override;

        /**
         * @brief Get unit of physical data in the photo.
         *
         * @return Unit name, or empty string (@c "") if no unit is
         *         available.
         */
        char const * unit() const override;

    private:

        /**
         * @brief Obtain data weight for given image pixel.
         *
         * Obtain the data weight based on how close the pixel at
         * sample @a i and line @a k is to the edge of the image or
         * the sky if sky removal is enabled.  For example, less
         * weight is given to pixels close to an edge of the image.
         *
         * @param[in]  i      Image pixel sample.
         * @param[in]  k      Image pixel line.
         * @param[out] weight Data weight at the given image pixel
         *                    coordinate.
         */
        void data_weight(std::size_t i,
                         std::size_t k,
                         std::size_t & weight) const;

    private:

        /// Pointer to the image array.
        std::vector<double> const image_;

        /// Number of samples in the image.
        std::size_t const samples_;

        /// Number of lines in the image.
        std::size_t const lines_;

        /// @c PhotoImage configuration parameters.
        std::unique_ptr<PhotoImageParameters const> const config_;

        /// @c PhotoImage viewing geometry.
        std::unique_ptr<ViewingGeometry const> const geometry_;

        /// Mask used when "removing" sky from source image.
        /**
         * A mask is used to mark which pixels in the photo are in the
         * body and which are in the sky without actually modifying
         * the original source photo.
         *
         * @note The body mask is generally only useful when
         *       performing weighted averaging in a mosaiced image.
         *
         * @see MosaicImage
         */
        std::vector<bool> body_mask_;

    };

}


#endif  /* MARC_PHOTO_IMAGE_H */
