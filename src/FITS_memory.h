// -*- C++ -*-
/**
 * @file FITS_memory.h
 *
 * Copyright (C) 2017  Ossama Othman
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

#ifndef MARC_FITS_MEMORY_H
#define MARC_FITS_MEMORY_H

#include <fitsio.h>

#include <memory>


namespace MaRC
{
    namespace FITS
    {
        /**
         * @brief CFITSIO file close functor.
         *
         * This functor closes a FITS file opened by CFITSIO
         * functions.  It is meant for use as the @c std::unique_ptr<>
         * @c Deleter template parameter.
         */
        struct closer
        {
            /**
             * @param[in] fptr Pointer to CFITSIO file instance that
             *                 will be closed.
             */
            void operator()(fitsfile * fptr) const
            {
                int status = 0;
                fits_close_file(fptr, &status);

                // At this point we don't really care about the
                // status.  Don't bother checking it.
            }
        };

        /**
         * @typedef file_unique_ptr
         *
         * Type alias of @c std::unique_ptr<> that automatically
         * closes the managed CFITSIO file up on exiting the scope in
         * which an instance of this @c file_unique_ptr resides.
         */
        using file_unique_ptr = std::unique_ptr<fitsfile, closer>;

    }  // FITS
}  // MaRC


#endif  /* MARC_FITS_MEMORY_H */
