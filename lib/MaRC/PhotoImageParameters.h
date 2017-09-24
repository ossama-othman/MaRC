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

#ifndef MARC_PHOTO_IMAGE_PARAMETERS_H
#define MARC_PHOTO_IMAGE_PARAMETERS_H

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
    class PhotoImageParameters
    {
    public:

        /// Constructor
        PhotoImageParameters() = default;

        /// Destructor.
        ~PhotoImage();

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

    private:

        /// Geometric/optical correction strategy used during
        /// latitude/longitude to pixel conversion, and vice versa.
        std::unique_ptr<GeometricCorrection> geometric_correction_;

        /// Pointer to the photometric correction strategy.
        std::unique_ptr<PhotometricCorrection> photometric_correction_;

        /// Pointer to the photometric correction strategy.
        std::unique_ptr<InterpolationStrategy> interpolation_strategy_;

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
