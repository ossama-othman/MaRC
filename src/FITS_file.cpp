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
#include "FITS_traits.h"

#include <MaRC/Log.h>

#include <limits>
#include <type_traits>
#include <stdexcept>


namespace MaRC
{
    namespace
    {
        void throw_on_cfitsio_error(int status)
        {
            if (status != 0) {
                char err[FLEN_STATUS] = { 0 };
                fits_get_errstatus(status, err);
                throw std::runtime_error(err);
            }
        }

        /**
         * @brief Open a FITS file for reading.
         *
         * Open FITS file in read-only mode.  This function exists
         * solely to make it easy to return a CFITSIO @c fitsfile
         * object as an rvalue.
         *
         * @param[in] filename Name of FITS file to opened.
         *
         * @return CFITSIO @c fitsfile object corresponding to the
         *         opened file.
         *
         * @throw std::runtime_error FITS file could not be opened.
         */
        fitsfile *
        open_fits_file(char const * filename)
        {
            static constexpr int mode = READONLY;  // CFITSIO mode

            fitsfile * fptr = nullptr;
            int status = 0;

            if (fits_open_image(&fptr, filename, mode, &status) != 0)
                throw_on_cfitsio_error(status);

            return fptr;
        }
    }
}

// ----------------------------------------------------------------

MaRC::FITS::header::header(fitsfile * fptr)
    : fptr_{fptr}
{
}

// ----------------------------------------------------------------

MaRC::FITS::data::data(fitsfile * fptr)
    : fptr_{fptr}
    , naxes_{}
{
    // Get the image parameters.
    int naxis = 0;
    int bitpix = 0;
    int status = 0;

    if (fits_get_img_param(fptr,
                           this->naxes_.size(),
                           &bitpix,
                           &naxis,
                           this->naxes_.data(),
                           &status) != 0)
        throw_on_cfitsio_error(status);

    // Sanity checks.
    if (naxis < static_cast<int>(this->naxes_.size()))
        throw std::runtime_error("too few dimensions in FITS image");

    constexpr naxes_array_type::value_type mindim = 2;
    for (auto const n : this->naxes_)
        if (n < mindim)
            throw("image dimension is too small");
}

void
MaRC::FITS::data::read(std::vector<double> & image) const
{
    LONGLONG const nelements =
        static_cast<LONGLONG>(this->naxes_[0]) * this->naxes_[1];

    using image_type = std::remove_reference_t<decltype(image)>;
    using value_type = image_type::value_type;

    constexpr auto nan = std::numeric_limits<value_type>::signaling_NaN();

    image_type tmp(nelements, nan);

    /**
     * First pixel to be read.
     *
     * @attention First pixel in CFITSIO is { 1, 1 } not { 0, 0 }.
     */
    naxes_array_type fpixel{ 1 , 1 };

    // For integer typed FITS images with a BLANK value, set the
    // "blank" value in our floating point converted copy of the image
    // to NaN.
    auto nulval = nan;
    int anynul = 0;  // Unused
    int status = 0;

    static_assert(std::is_same<value_type, decltype(nulval)>(),
                  "Nul value type doesn't match photo container type.");

    if (fits_read_pix(this->fptr_,
                      traits<value_type>::datatype,
                      fpixel.data(),
                      nelements,
                      &nulval,  // "Blank" value in our image.
                      tmp.data(),
                      &anynul,  // Were any blank values found?
                      &status) != 0)
        throw_on_cfitsio_error(status);

    image = std::move(tmp);
}

// ----------------------------------------------------------------

MaRC::FITS::file::file(char const * filename)
    : fptr_(open_fits_file(filename))
    , header_(fptr_.get())
    , data_(fptr_.get())
{
    // Verify checksums if present.
    int dataok = 0;
    int hduok  = 0;
    int status = 0;

    if (fits_verify_chksum(this->fptr_.get(),
                           &dataok,
                           &hduok,
                           &status) != 0) {
        if (dataok == -1)  // Incorrect data checksum
            MaRC::warn("Data checksum for FITS file \"{}\" "
                       "is incorrect",
                       filename);

        if (hduok == -1)
            MaRC::warn("Header checksum for FITS file \"{}\" "
                       "is incorrect",
                       filename);
    }
}
