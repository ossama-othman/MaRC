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
// #include "FITS_header.h"

#include <array>
#include <vector>


namespace MaRC
{
    namespace FITS
    {
        class header
        {
        public:
            header(fitsfile * fptr);
            ~header() = default;

        private:

            fitsfile * const fptr_;

        };

        class data
        {
        public:
            data(fitsfile * fptr);
            ~data() = default;

            std::size_t samples() const
            {
                    return static_cast<std::size_t>(this->naxes_[0]);
            }

            std::size_t lines() const
            {
                    return static_cast<std::size_t>(this->naxes_[1]);
            }

            void read(std::vector<double> & img) const;

        private:

            fitsfile * const fptr_;

            using naxes_array_type = std::array<long, 2>;
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

            FITS::data const & data() const { return this->data_; }

        private:

            /// Underlying CFITSIO @c fitsfile object.
            file_unique_ptr fptr_;

            header header_;

            FITS::data data_;

        };

    }  // FITS
}  // MaRC


#endif  /* MARC_FITS_FILE_H */
