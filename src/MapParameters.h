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

#include <marc/plot_info.h>

#include <list>
#include <string>
#include <memory>


namespace MaRC
{
    /**
     * Type used to store a %FITS @c BLANK integer value.
     *
     * @todo Use @c std::optional once %MaRC requires C++17.
     */
    using blank_type = MaRC::plot_info::blank_type;

    /**
     * @class MapParameters
     *
     * @brief Map configuration parameters.
     *
     * The map parameters in this class may be supplied by the user or
     * populated automatically from relevant values in the source
     * images being mapped.
     */
    class MapParameters
    {
    public:

        /// Constructor
        MapParameters();

        /// Destructor.
        ~MapParameters() = default;

        // Disallow copying.
        MapParameters(MapParameters const &) = delete;
        void operator=(MapParameters const &) = delete;

        /// Set map author.
        void author(std::string a);

        /// Get map author.
        std::string const & author() const { return this->author_; }

        /**
         * @brief Set map bits per pixel.
         *
         * @li @c  8  8 bit unsigned integer data.
         * @li @c  16 16 bit signed   integer data.
         * @li @c  32 32 bit signed   integer data.
         * @li @c  64 64 bit signed   integer data.
         * @li @c -32 32 bit floating point   data.
         * @li @c -64 64 bit floating point   data.
         *
         * @param[in] n Bits per pixel value as defined by the %FITS
         *              standard.
         */
        void bitpix(int n);

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
        int bitpix() const;

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

        /// Get the value for the map %FITS @c BZERO keyword.
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

        /// Get the value for the map %FITS @c BSCALE keyword.
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

        /// Get the value for the map %FITS @c BLANK keyword.
        blank_type blank() const { return this->blank_; }

        /// Set the %FITS @c DATAMAX value.
        void datamax(double max);

        /// Get the value for the map %FITS @c DATAMAX keyword.
        double datamax() const { return this->datamax_; }

        /// Set the %FITS @c DATAMIN value.
        void datamin(double min);

        /// Get the value for the map %FITS @c DATAMIN keyword.
        double datamin() const { return this->datamin_; }

        /// Set the %FITS @c EQUINOX value.
        void equinox(double e);

        /// Get the value for the map %FITS @c EQUINOX keyword.
        double equinox() const { return this->equinox_; }

        /// Set name of object being mapped.
        void object(std::string o);

        /// Get name of object being mapped.
        std::string const & object() const { return this->object_; }

        /**
         * @brief Set organization or institution responsible for
         *        creating the map.
         */
        void origin(std::string o);

        /**
         * @brief Get organization or institution responsible for
         *        creating the map.
         *
         * @todo We may not want to automatically pull this value from
         *       the source %FITS file since that "origin" may not be
         *       the same as the one creating the map.
         */
        std::string const & origin() const { return this->origin_; }

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

        /**
         * @brief Coefficient of linear term in the scaling equation.
         *
         * @note This value corresponds to the %FITS "BSCALE" keyword.
         */
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
         *
         * @note This value corresponds to the %FITS "BUNIT" keyword.
         */
        std::string bunit_;

        /**
         * @brief Physical value corresponding to an array value of
         *        zero.
         *
         * @note This value corresponds to the %FITS "BZERO" keyword.
         */
        double bzero_;

        /**
         * @brief Maximum valid physical value.
         *
         * @note This value corresponds to the %FITS "DATAMAX"
         *       keyword.
         *
         * @bug On platforms that implement the IEEE 754 floating
         *      point standard, the use of @c double as the underlying
         *      @c DATAMAX type will cause loss of precision if the
         *      %FITS @c DATAMAX values is an integer that requires
         *      more than 53 bits since the significand of a 64 bit
         *      IEEE 754 floating point value is only 53 bits wide.
         *      Special handling will be need to be implemented to
         *      handle such a case.
         */
        double datamax_;

        /**
         * @brief Minimum valid physical value.
         *
         * @note This value corresponds to the %FITS "DATAMIN"
         *       keyword.
         *
         * @bug On platforms that implement the IEEE 754 floating
         *      point standard, the use of @c double as the underlying
         *      @c DATAMIN type will cause loss of precision if the
         *      %FITS @c DATAMIN values is an integer that requires
         *      more than 53 bits since the significand of a 64 bit
         *      IEEE 754 floating point value is only 53 bits wide.
         *      Special handling will be need to be implemented to
         *      handle such a case.
         */
        double datamin_;

        /**
         * @brief Epoch of the mean equator and equinox in years.
         *
         * @note This value must be non-negative.
         * @note This value corresponds to the %FITS "EQUINOX"
         *       keyword.
         */
        double equinox_;

        /**
         * @brief Instrument used to acquire the data.
         *
         * @note This value corresponds to the %FITS "INSTRUME"
         *       keyword.
         */
        std::string instrument_;

        /**
         * @brief Name of observed object.
         *
         * @note This value corresponds to the %FITS "OBJECT"
         *       keyword.
         */
        std::string object_;

        /**
         * @brief Name of person who acquired the source data.
         *
         * @note This value corresponds to the %FITS "OBSERVER"
         *       keyword.
         */
        std::string observer_;

        /**
         * @brief Organization or institution responsible for creating
         *        the map %FITS file.
         *
         * @note This value corresponds to the %FITS "ORIGIN" keyword.
         */
        std::string origin_;

        /**
         * @brief Reference to where the source data was published.
         *
         * @note The %FITS standard recommends that this be the
         *       19-digit bibliographic indentifier used in the
         *       Astrophysics Data System bibliographic databases or
         *       the Digital Object Identifier.
         *
         * @note This value corresponds to the %FITS "REFERENC"
         *       keyword.
         */
        std::string reference_;

        /**
         * @brief Telescope used to acquire the source data.
         *
         * @note This value corresponds to the %FITS "TELESCOP"
         *       keyword.
         */
        std::string telescope_;

    };
}


#endif  /* MARC_MAP_PARAMETERS_H */
