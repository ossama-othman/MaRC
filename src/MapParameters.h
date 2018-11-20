//   -*- C++ -*-
/**
 * @file MapParameters.h
 *
 * Copyright (C) 2018  Ossama Othman
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

#ifndef MARC_MAP_PARAMETERS_H
#define MARC_MAP_PARAMETERS_H

#include "FITS_traits.h"

#include <marc/optional.h>

#include <list>
#include <string>
#include <memory>


namespace MaRC
{
    class ImageFactory;

    /**
     * Type used to store a %FITS @c BLANK integer value.
     *
     * @todo Use @c std::optional<> once %MaRC requires C++17.
     */
    using blank_type = MaRC::optional<FITS::longlong_type>;

    /**
     * @class MapParameters
     *
     * @brief Map configuration parameters.
     *
     * The map parameters in this class may be supplied by the user or
     * populated automatically from relevant values in the source
     * images being mapped.
     *
     * @todo If possible, populate the following (%FITS)
     *       parameters from source image factories if they exist:
     * @li AUTHOR
     * @li BITPIX
     * @li BLANK
     * @li BSCALE
     * @li BUNIT
     * @li BZERO
     * @li DATAMAX
     * @li DATAMIN
     * @li EQUINOX
     * @li INSTRUME
     * @li NAXES
     * @li OBJECT
     * @li OBSERVER
     * @li ORIGIN
     * @li REFERENC
     * @li TELESCOP
     */
    class MapParameters
    {
    public:

        /// Source image factories type.
        using image_factories_type =
            std::list<std::unique_ptr<MaRC::ImageFactory>>;

        /// Constructor
        MapParameters();

        /// Destructor.
        ~MapParameters() = default;

        // Disallow copying.
        MapParameters(MapParameters const &) = delete;
        void operator=(MapParameters const &) = delete;

        /*
         * @brief Populate map parameters from the give source.
         *
         * @param[in] source
         *
         * Populate optional parameters automatically, such as from
         * @c SourceImage parameters, where possible.
         */
        bool populate_from();

        /// Set map author.
        void author(std::string author);

        std::string author() const { return this->author_; }

        /// Set map bits per pixel code.
        void bitpix(int bitpix);

        /**
         * @brief Get map %FITS bits per pixel code.
         *
         * Obtain the bits per pixel "BITPIX" in the map %FITS file,
         * as defined by the %FITS standard.  This value may either be
         * supplied by the user or determined at run-time based on
         * source image data being mapped.
         *
         * @retval   8  8 bit unsigned integer data.
         * @retval  16 16 bit signed   integer data.
         * @retval  32 32 bit signed   integer data.
         * @retval  64 64 bit signed   integer data.
         * @retval -32 32 bit floating point   data.
         * @retval -64 64 bit floating point   data.
         *
         * @throw std::runtime_error Unable to determine bitpix
         *                           value.
         */
        int bitpix();

        /**
         * @brief Set the value for the map %FITS @c BZERO keyword.
         *
         * The default value of the %FITS @c BZERO keyword is zero.
         *
         * @param[in] zero @c BZERO keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       %FITS file to be transformed according the equation:
         * @code{.cpp}
         *     (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void bzero(double zero);

        double bzero() const { return this->bzero_; }

        /**
         * @brief Set the value for the map %FITS @c BSCALE keyword.
         *
         * The default value of the %FITS @c BSCALE keyword is one.
         *
         * @param[in] scale @c BSCALE keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       %FITS file to be transformed according the equation:
         * @code{.cpp}
         *     (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void bscale(double scale);

        double bscale() const { return this->bscale_; }

        /**
         * @brief Set the value for the map %FITS @c BLANK keyword.
         *
         * The %FITS @c BLANK keyword only applies to %FITS images
         * containing integer types.  The corresponding "blank" value
         * for floating point %FITS images is the IEEE "not-a-number
         * constant.
         *
         * The @c BLANK keyword merely documents which physical (not
         * %FITS) values in the image array are undefined.
         *
         * @param[in] blank %FITS @c BLANK keyword value.
         */
        void blank(blank_type blank);


        blank_type blank() const { return this->blank_; }

        void object(std::string object);

        std::string object() const { return this->object_; }

        /// Set organization or institution responsible for creating
        /// map.
        void origin(std::string origin);

        std::string origin() const { return this->origin_; }

    private:

        /**
         * @brief Source data author.
         *
         * The person that compiled the data in the map.
         *
         * @note This value corresponds to the %FITS "AUTHOR" keyword
         */
        std::string author_;

        /**
         * @brief Bits per pixel.
         *
         * %FITS bits per pixel (i.e. 8, 16, 32, 64, -32, or -64).
         *
         * @note This value corresponds to the %FITS "BITPIX"
         *       keyword.
         */
        int bitpix_;

        /**
         * @brief  Value of pixels with undefined physical value.
         *
         * @note This value is only valid for integer typed maps.
         * @note This value corresponds to the %FITS "BLANK" keyword.
         */
        blank_type blank_;

        /// Coefficient of linear term in the scaling equation.
        double bscale_;

        /**
         * @brief Unit of physical data.
         *
         * Get the units of the physical data, i.e. image data that
         * has been scaled and offset according to the following
         * equation:
         *
         * @code{.cpp}
         *     physical_data = image_data * scale + offset;
         * @endcode
         *
         * The unit should conform to IAU Style Manual
         * recommendations.
         *
         * @see https://www.iau.org/publications/proceedings_rules/units/
         *
         * @return Unit of physical data in the source image.
         */
        std::string bunit_;

        /// Physical value corresponding to an array value of zero.
        double bzero_;


        double data_max_;
        double data_min_;
        double equinox_;
        std::string instrument_;

        /// Name of object to be mapped.
        std::string object_;

        std::string observer_;

        /// Organization or institution responsible for creating this
        /// %FITS file.
        std::string origin_;

        std::string reference_;
        std::string telescope_;

    };

}


#endif  /* MARC_MAP_PARAMETERS_H */
