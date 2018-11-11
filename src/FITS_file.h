// -*- C++ -*-
/**
 * @file FITS_file.h
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

#ifndef MARC_FITS_FILE_H
#define MARC_FITS_FILE_H

#include "FITS_memory.h"
#include "FITS_traits.h"
// #include "FITS_header.h"

#include <marc/optional.h>

#include <array>
#include <vector>
#include <string>


namespace MaRC
{
    namespace FITS
    {
        /**
         * @class header
         *
         * @brief Class that encapsulates standard FITS header
         *        keywords.
         *
         * This class is a proxy that provides access to the FITS
         * header.
         *
         * @todo FITS keyword values are not cached during the first
         *       access.  Should they?
         */
        class header
        {
        public:

            /// Constructor
            header(fitsfile * fptr);

            /// Destructor
            ~header() = default;

            // Prevent copying.
            header(header const &) = delete;
            void operator=(header const &) = delete;

            /**
             * @name Standard FITS Keyword Accessors
             *
             * @brief Retrieve values of standard FITS keywords.
             *
             * These methods have corresponding standard FITS
             * keywords, e.g. @c author() corresponds to the standard
             * FITS @c AUTHOR keyword.  Accessors for mandatory
             * keywords will always return a value.  Otherwise, a
             * keyword doesn't exist in the FITS file will return an
             * empty value, such as an empty @c std::string or a
             * @c MaRC::optional<> with no value.
             */
            //@{
            std::string author() const;
            int bitpix() const;
            MaRC::optional<longlong_type> blank() const;
            MaRC::optional<double> bscale() const;
            std::string bunit() const;
            MaRC::optional<double> bzero() const;
            MaRC::optional<double> datamax() const;
            MaRC::optional<double> datamin() const;
            MaRC::optional<double> equinox() const;
            std::string instrument() const;
            int naxis() const;
            std::array<long, 3> naxes() const;
            std::string object() const;
            std::string observer() const;
            std::string origin() const;
            std::string reference() const;
            std::string telescope() const;
            //@}

        private:

            /// Underlying CFITSIO @c fitsfile object.
            fitsfile * const fptr_;

        };

        /**
         * @class data
         *
         * @brief Encapsulate FITS image read into memory.
         *
         * This class is a proxy that provides access to the FITS
         * data.
         */
        class data
        {
        public:

            /// Constructor
            data(fitsfile * fptr);

            /// Destructor
            ~data() = default;

            // Prevent copying.
            data(data const &) = delete;
            void operator=(data const &) = delete;

            /// The number of columns in the FITS image.
            std::size_t samples() const
            {
                return static_cast<std::size_t>(this->naxes_[0]);
            }

            /// The number of rows in the FITS image.
            std::size_t lines() const
            {
                return static_cast<std::size_t>(this->naxes_[1]);
            }

            /**
             * @brief Read the FITS image into the given @c vector.
             *
             * @param[in,out] img Vector that will contain the read
             *                    FITS image data.
             */
            void read(std::vector<double> & img) const;

        private:

            /// Underlying CFITSIO @c fitsfile object.
            fitsfile * const fptr_;

            // Only two-dimensional FITS images are currently
            // supported.
            using naxes_array_type = std::array<long, 2>;

            /// Array containing FITS image dimensions.
            naxes_array_type naxes_;

        };

        /**
         * @brief Encapsulate a FITS file containing an image.
         *
         * This class encapsulates operations performed on a FITS file
         * containing an image, such as opening, closing, image
         * parameter retrieval, etc.
         */
        class file
        {
        public:

            /**
             * @brief Constructor
             *
             * @param[in] filename Name of FITS file to opened.
             */
            file(char const * filename);

            /// Destructor
            ~file() = default;

            // Prevent copying.
            file(file const &) = delete;
            void operator=(file const &) = delete;

            /// Get proxy that provides access to the FITS header.
            FITS::header const & header() const { return this->header_; }

            /// Get proxy that provides access to the FITS data.
            FITS::data const & data() const { return this->data_; }

        private:

            /// Underlying CFITSIO @c fitsfile object.
            file_unique_ptr fptr_;

            /// FITS header proxy.
            FITS::header header_;

            /// FITS data proxy.
            FITS::data data_;

        };

    }  // FITS
}  // MaRC


#endif  /* MARC_FITS_FILE_H */
