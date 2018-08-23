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
        /**
         * @brief Throw an exception if a CFITSIO error occurred.
         *
         * @throw std::runtime_error CFITSIO error occurred.
         */
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

        /**
         * @brief Get the value of the given FITS keyword.
         *
         * @param[in] fptr Pointer to CFITSIO @c fitsfile object.
         * @param[in] key  FITS keyword.
         *
         * @return Value corresponding to @a key.  If no such key
         *         exists in the FITS file the
         *         @c MaRC::optional<>::has_value() method of the
         *         returned value will return @c false.
         */
        template <typename T>
        MaRC::optional<T>
        read_fits_key(fitsfile * fptr, char const * key)
        {
            MaRC::optional<T> optional_value;

            int status = 0;
            T value = 0;
            if (fits_read_key(fptr,
                              FITS::traits<T>::datatype,
                              key,
                              &value,
                              nullptr, // comment
                              &status) == 0)
                optional_value = MaRC::make_optional(value);
            else if (status != KEY_NO_EXIST) {
                throw_on_cfitsio_error(status);
            }

            return optional_value;
        }

        /**
         * @brief Get the string value of the given FITS keyword.
         *
         * @param[in] fptr Pointer to CFITSIO @c fitsfile object.
         * @param[in] key  FITS keyword.
         *
         * @return String value corresponding to @a key.  If no such
         *         key exists in the FITS file the
         *         @c MaRC::optional<>::has_value() method of the
         *         returned value will return @c false.
         */
        MaRC::optional<std::string>
        read_fits_string_key(fitsfile * fptr, char const * key)
        {
            int length = 0;  // CFITSIO wants 'int'.
            int status = 0;

            if (fits_get_key_strlen(fptr,
                                    key,
                                    &length,
                                    &status) != 0
                && status != KEY_NO_EXIST) {
                throw_on_cfitsio_error(status);
            }

            MaRC::optional<std::string> optional_value;

            if (status == 0) {  // key exists
                std::string value(length + 1, '\0');
                constexpr int firstchar = 1;

                if (fits_read_string_key(fptr,
                                         key,
                                         firstchar,
                                         length,
                                         &value[0], // data() is const
                                                    // until C++17
                                         nullptr, // valuelen
                                         nullptr, // comm
                                         &status) != 0) {
                    throw_on_cfitsio_error(status);
                }

                optional_value = MaRC::make_optional(value);
            }

            return optional_value;
        }
    }
}

// ----------------------------------------------------------------

MaRC::FITS::header::header(fitsfile * fptr)
    : fptr_{fptr}
{
}

MaRC::optional<std::string>
MaRC::FITS::header::author() const
{
    constexpr char const key[] = "AUTHOR";

    return read_fits_string_key(this->fptr_, key);
}

int
MaRC::FITS::header::bitpix() const
{
    int bp = 0;
    int status = 0;

    if (fits_get_img_type(this->fptr_,
                          &bp,
                          &status) != 0)
        throw_on_cfitsio_error(status);

    return bp;
}

MaRC::optional<MaRC::FITS::longlong_type>
MaRC::FITS::header::blank() const
{
    constexpr char const key[] = "BLANK";

    return read_fits_key<longlong_type>(this->fptr_, key);
}

MaRC::optional<double>
MaRC::FITS::header::bscale() const
{
    constexpr char const key[] = "BSCALE";

    return read_fits_key<double>(this->fptr_, key);
}

MaRC::optional<std::string>
MaRC::FITS::header::bunit() const
{
    constexpr char const key[] = "BUNIT";

    return read_fits_string_key(this->fptr_, key);
}

MaRC::optional<double>
MaRC::FITS::header::bzero() const
{
    constexpr char const key[] = "BZERO";

    return read_fits_key<double>(this->fptr_, key);
}

MaRC::optional<double>
MaRC::FITS::header::datamax() const
{
    constexpr char const key[] = "DATAMAX";

    return read_fits_key<double>(this->fptr_, key);
}

MaRC::optional<double>
MaRC::FITS::header::datamin() const
{
    constexpr char const key[] = "DATAMIN";

    return read_fits_key<double>(this->fptr_, key);
}

MaRC::optional<double>
MaRC::FITS::header::equinox() const
{
    constexpr char const key[] = "EQUINOX";

    return read_fits_key<double>(this->fptr_, key);
}

MaRC::optional<std::string>
MaRC::FITS::header::instrument() const
{
    constexpr char const key[] = "INSTRUME";

    return read_fits_string_key(this->fptr_, key);
}

int
MaRC::FITS::header::naxis() const
{
    int n = 0;
    int status = 0;

    if (fits_get_img_dim(this->fptr_,
                         &n,
                         &status) != 0)
        throw_on_cfitsio_error(status);

    return n;
}

std::array<long, 3>
MaRC::FITS::header::naxes() const
{
    std::array<long, 3> n;
    int status = 0;

    if (fits_get_img_size(this->fptr_,
                          n.size(),
                          n.data(),
                          &status) != 0)
        throw_on_cfitsio_error(status);

    return n;
}

MaRC::optional<std::string>
MaRC::FITS::header::object() const
{
    constexpr char const key[] = "OBJECT";

    return read_fits_string_key(this->fptr_, key);
}

MaRC::optional<std::string>
MaRC::FITS::header::observer() const
{
    constexpr char const key[] = "OBSERVER";

    return read_fits_string_key(this->fptr_, key);
}

MaRC::optional<std::string>
MaRC::FITS::header::origin() const
{
    constexpr char const key[] = "ORIGIN";

    return read_fits_string_key(this->fptr_, key);
}

MaRC::optional<std::string>
MaRC::FITS::header::reference() const
{
    constexpr char const key[] = "REFERENC";

    return read_fits_string_key(this->fptr_, key);
}

MaRC::optional<std::string>
MaRC::FITS::header::telescope() const
{
    constexpr char const key[] = "TELESCOP";

    return read_fits_string_key(this->fptr_, key);
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
    // CFITSIO wants its own LONGLONG type, not size_t.
    LONGLONG const nelements =
        static_cast<LONGLONG>(this->naxes_[0]) * this->naxes_[1];

    using image_type = std::remove_reference_t<decltype(image)>;
    using value_type = image_type::value_type;

    constexpr auto nan = std::numeric_limits<value_type>::signaling_NaN();

    /**
     * @todo Should we bother initializing the image vector with NaN?
     *       CFITSIO will just overwrite each element regardless,
     *       right?
     */
    image_type tmp(nelements, nan);

    /**
     * First pixel to be read.
     *
     * @attention First pixel in CFITSIO is {1, 1} not {0, 0}.
     */
    constexpr naxes_array_type fpixel{1, 1};

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
                      const_cast<long *>(fpixel.data()),
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
