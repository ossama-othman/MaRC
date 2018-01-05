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


namespace MaRC
{
    namespace FITS
    {
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

            // Prevent copying.
            file(file const &) = delete;
            void operator=(file const &) = delete;            

        private:

            /// Underlying CFITSIO @c fitsfile object.
            file_unique_ptr fptr_;

        };

    }  // FITS
}  // MaRC


#endif  /* MARC_FITS_FILE_H */
