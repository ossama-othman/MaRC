//   -*- C++ -*-
/**
 * @file PhotoImage.h
 *
 * Copyright (C) 1999, 2003-2005, 2017  Ossama Othman
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

#ifndef MARC_PHOTO_IMAGE_H
#define MARC_PHOTO_IMAGE_H

#include <MaRC/SourceImage.h>

#include <memory>
#include <vector>


namespace MaRC
{
    class OblateSpheroid;
    class GeometricCorrection;
    class PhotometricCorrection;
    class InterpolationStrategy;

    /**
     * @class PhotoImage
     *
     * @brief Concrete SourceImage strategy for mapping bodies in
     *        photos.
     *
     * PhotoImages have viewing geometries that may differ from other
     * photos of the same body being mapped.  For example, photos from
     * telescope observations fit into this category.
     */
    class PhotoImage : public SourceImage
    {
    public:

        /// Constructor
        /**
         * @param[in]     body    Pointer to OblateSpheroid object
         *                        representing body being mapped.
         * @param[in,out] image   Array containing the image data.
         *                        Ownership is transferred through a
         *                        move operation.
         * @param[in]     samples Number of samples in the image.
         * @param[in]     lines   Number of lines   in the image.
         * @param[in,out] gc      Geometric correction strategy.
         *                        (@c PhotoImage assumes ownership).
         */
        PhotoImage(std::shared_ptr<OblateSpheroid> body,
                   std::vector<double> && image, // moved, not copied!
                   std::size_t samples,
                   std::size_t lines,
                   std::unique_ptr<GeometricCorrection> gc);

        /// Destructor.
        virtual ~PhotoImage();

        /// Set sky removal variable
        /**
         * Enabling sky removal prevents data believed (i.e. computed)
         * to be in the sky rather than on the body from being
         * mapped.
         *
         * @param[in] remove @c true  == create sky removal mask,
         *                   @c false == do not create sky removal
         *                               mask.
         *
         * @note The source image array will not be modified.
         */
        void remove_sky(bool remove);

        /// Create sky removal mask.
        void remove_sky();

        /// Set the geometric correction strategy used during lat/lon
        /// to pixel conversion, and vice-versa.
        bool geometric_correction(
            std::unique_ptr<GeometricCorrection> strategy);

        /// Set the photometric correction strategy.
        bool photometric_correction(
            std::unique_ptr<PhotometricCorrection> strategy);

        /// Set all nibble values to @a n.
        /**
         * @param[in] n Nibble value for all image sides.
         */
        void nibble(std::size_t n);

        /// Set left nibble value to @a n.
        /**
         * @param[in] n Nibble value for left image side.
         */
        void nibble_left(std::size_t n);

        /// Set right nibble value to @a n.
        /**
         * @param[in] n Nibble value for right image side.
         */
        void nibble_right(std::size_t n);

        /// Set top nibble value to @a n.
        /**
         * @param[in] n Nibble value for top image side.
         */
        void nibble_top(std::size_t n);

        /// Set bottom nibble value to @a n.
        /**
         * @param[in] n Nibble value for bottom image side.
         */
        void nibble_bottom(std::size_t n);

        /// Return left nibble value.
        std::size_t nibble_left() const   { return this->nibble_left_;   }

        /// Return right nibble value.
        std::size_t nibble_right() const  { return this->nibble_right_;  }

        /// Return top nibble value.
        std::size_t nibble_top() const    { return this->nibble_top_;    }

        /// Return bottom nibble value.
        std::size_t nibble_bottom() const { return this->nibble_bottom_; }

        /// Enable/disable pixel interpolation when reading data.
        void interpolate(bool enable);

        /// Retrieve data from source image.
        /**
         * Retrieve data from source image. The configured data
         * interpolation strategy will be applied.
         *
         * @see MaRC::SourceImage::read_data().
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const;

        /// Retrieve data and weight from source image.
        /**
         * Retrieve data and weight from source image.  The configured
         * data interpolation strategy will be applied.
         *
         * @param[in]     lat    Bodycentric (e.g. planetocentric)
         *                       latitude in radians.
         * @param[in]     lon    Longitude in radians.
         * @param[out]    data   Data retrieved from image.
         * @param[in,out] weight Distance from pixel to closest edge
         *                       or blank pixel.
         * @param[in]     scan   Flag that determines if a data weight
         *                       scan is performed.  It is generally
         *                       only used by the version of
         *                       @c read_data() that does not return a
         *                       weight.
         *
         * @retval true  Data retrieved.
         * @retval false No data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data,
                               std::size_t & weight,
                               bool scan = true) const;

    private:

        /// Pointer to the image array.
        std::vector<double> const image_;

        /// Geometric/optical correction strategy used during
        /// latitude/longitude to pixel conversion, and vice versa.
        std::unique_ptr<GeometricCorrection> geometric_correction_;

        /// Pointer to the photometric correction strategy.
        std::unique_ptr<PhotometricCorrection> photometric_correction_;

        /// Pointer to the photometric correction strategy.
        std::unique_ptr<InterpolationStrategy> interpolation_strategy_;

        /// Mask used when "removing" sky from source image.
        /**
         * A mask is used to mark which pixels in the photo are in the
         * body and which are in the sky without actually modifying
         * the original source photo.
         *
         * @note The sky mask is generally only useful when performing
         *       weighted averaging in a mosaiced image.
         *
         * @see MosaicImage
         */
        std::vector<bool> sky_mask_;

        /// Amount of pixels to ignore from left side of input image
        /// (photo).
        std::size_t nibble_left_;

        /// Amount of pixels to ignore from right side of input image
        /// (photo).
        std::size_t nibble_right_;

        /// Amount of pixels to ignore from top side of input image
        /// (photo).
        std::size_t nibble_top_;

        /// Amount of pixels to ignore from bottom side of input image
        /// (photo).
        std::size_t nibble_bottom_;

  };

}


#endif  /* MARC_PHOTO_IMAGE_H */
