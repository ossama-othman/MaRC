/**
 * @file FITS_file.cpp
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

#include "FITS_file.h"

#include <MaRC/Log.h>

#include <stdexcept>


namespace
{
    /**
     * @brief Open a FITS file for reading.
     *
     * Open FITS file in read-only mode.  This function exists solely
     * to make it easy to return a CFITSIO @c fitsfile object so that the
     * return value may be used as an rvalue.
     *
     * @param[in] filename Name of FITS file to opened.
     *
     * @return CFITSIO @c fitsfile object corresponding to the opened
     *         file.
     *
     * @throw std::runtime_error FITS file could not be opened.

     */
    fitsfile *
    open_fits_file(char const * filename)
    {
        static constexpr int mode = READONLY;  // CFITSIO mode

        fitsfile * fptr = nullptr;
        int status = 0;

        if (fits_open_image(&fptr, filename, mode, &status) != 0) {
            char err[FLEN_STATUS] = { 0 };
            fits_get_errstatus(status, err);
            throw std::runtime_error(err);
        }

        return fptr;
    }
}

// ----------------------------------------------------------------

using namespace MaRC;

FITS::file::file(char const * filename)
    : fptr_(open_fits_file(filename))
    , header_(fptr_.get())
    , data_(fptr_.get())
{
    int dataok = 0;
    int hduok  = 0;
    int status = 0;

    if (fits_verify_chksum(fptr_, &dataok, &hduok, &status) != 0) {
        if (dataok == -1)  // Incorrect data checksum
            MaRC::warn("Data checksum for FITS file \"{}\" is incorrect",
                       filename);

        if (hduok == -1)
            MaRC::warn("Header checksum for FITS file \"{}\" "
                       "is incorrect",
                       filename);
    }
}
