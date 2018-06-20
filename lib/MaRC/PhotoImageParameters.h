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

#include "MaRC/PhotometricCorrection.h"
#include "MaRC/InterpolationStrategy.h"
#include "MaRC/Export.h"

#include <memory>


namespace MaRC
{

    /**
     * @class PhotoImageParameters
     *
     * @brief Configuration parameters specific to @c PhotoImage.
     */
    class MARC_API PhotoImageParameters
    {
    public:

        /// Constructor
        PhotoImageParameters();

        /// Destructor.
        ~PhotoImageParameters() = default;

        // Disallow copying.
        PhotoImageParameters(PhotoImageParameters const &) = delete;
        void operator=(PhotoImageParameters const &) = delete;

        /// Set the photometric correction strategy.
        /**
         * @throw std::invalid_argument @a strategy is @c nullptr.
         */
        void photometric_correction(
            std::unique_ptr<PhotometricCorrection> strategy);

        /// Get the photometric correction strategy.
        auto photometric_correction() const
        {
            return this->photometric_correction_.get();
        }

        /// Set the interpolation strategy used when reading data.
        /**
         * @throw std::invalid_argument @a strategy is @c nullptr.
         */
        void interpolation_strategy(
            std::unique_ptr<InterpolationStrategy> strategy);

        /// Get the interpolation strategy used when reading data.
        auto interpolation_strategy() const
        {
            return this->interpolation_strategy_.get();
        }

        /**
         * @brief Set all nibble values to @a n.
         *
         * @param[in] n Nibble value for all image sides.
         */
        void nibble(std::size_t n);

        /**
         * @brief Set left nibble value to @a n.
         *
         * @param[in] n Nibble value for left image side.
         */
        void nibble_left(std::size_t n);

        /**
         * @brief Set right nibble value to @a n.
         *
         * @param[in] n Nibble value for right image side.
         */
        void nibble_right(std::size_t n);

        /**
         * @brief Set top nibble value to @a n.
         *
         * @param[in] n Nibble value for top image side.
         */
        void nibble_top(std::size_t n);

        /**
         * @brief Set bottom nibble value to @a n.
         *
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

        /**
         * @brief Set the unit for the physical data in the image.
         *
         * @param[in] u Unit for the physical data in the image.
         * @param[in] c Comment for the physical data unit.
         *
         * @see SourceImage::unit()
         */
        void unit(char const * u, char const * c);

        /// Get the unit for the physical data in the image.
        std::string const & unit() const { return this->unit_; }

        /// Get the comment for the physical data unit.
        std::string const & unit_comment() const
        {
            /**
             * @todo The unit comment is currently unused.  Do
             *       something with it, or remove it.
             */
            return this->unit_comment_;
        }

        /**
         * @brief Set sky removal variable
         *
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

        /// Should the sky removal mask be generated.
        bool remove_sky() const { return this->remove_sky_; }

        /**
         * @brief Validate current @c PhotoImage parameters.
         *
         * @param[in] samples  Number of samples in the image.
         * @param[in] lines    Number of lines   in the image.
         */
        void validate_parameters(std::size_t samples,
                                 std::size_t lines) const;

    private:

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

        /// Pointer to the photometric correction strategy.
        std::unique_ptr<PhotometricCorrection> photometric_correction_;

        /// Pointer to the interpolation strategy.
        std::unique_ptr<InterpolationStrategy> interpolation_strategy_;

        /// Unit of physical data in the image.
        std::string unit_;

        /// Physical data unit comment.
        std::string unit_comment_;

        /// Should the sky removal mask be generated.
        bool remove_sky_;

    };

}


#endif  /* MARC_PHOTO_IMAGE_PARAMETERS_H */
