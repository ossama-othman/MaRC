/**
 * @file FITS_file.cpp
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "FITS_file.h"
#include "FITS_image.h"

#include <marc/Log.h>

#include <limits>
#include <type_traits>
#include <stdexcept>


namespace MaRC
{
    namespace
    {
        /**
         * @brief Create or open a %FITS file.
         *
         * @note This function exists solely to make it easy to return
         *        a CFITSIO @c fitsfile object as an rvalue.
         *
         * @param[in] filename Name of %FITS file to opened.
         * @param[in] create   Create file @a filename for writing if
         *                     @c true.  Otherwise open @a filename
         *                     read-only if @c false.
         *
         * @return CFITSIO @c fitsfile object corresponding to the
         *         opened file.  Call @c fits_close_file() on the
         *         returned @c fitsfile object when done with the
         *         corresponding %FITS file.
         *
         * @throw std::runtime_error %FITS file could not be opened.
         */
        FITS::file::shared_ptr
        open_fits_file(char const * filename, bool create)
        {
            fitsfile * fptr = nullptr;
            int status = 0;

            if (create) {
                fits_create_file(&fptr, filename, &status);
            } else {
                static constexpr int mode = READONLY;  // CFITSIO mode

                // Go to the first IMAGE HDU.
                fits_open_image(&fptr, filename, mode, &status);
            }

            if (status != 0)
                FITS::throw_on_error(status);

            FITS::file::shared_ptr f(fptr, FITS::file::closer());

            return f;
        }

        /**
         * @brief Get the value for the given %FITS keyword.
         *
         * @tparam    T    Value type.
         * @param[in] fptr Pointer to CFITSIO @c fitsfile object.
         * @param[in] key  %FITS keyword.
         *
         * @return Value corresponding to @a key.  If no such key
         *         exists in the %FITS file the
         *         @c MaRC::optional::has_value() method of the
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
                FITS::throw_on_error(status);
            }

            return optional_value;
        }

        /**
         * @brief Get the string value of the given %FITS keyword.
         *
         * @param[in] fptr Pointer to CFITSIO @c fitsfile object.
         * @param[in] key  %FITS keyword.
         *
         * @return String value corresponding to @a key.  The returned
         *         string will be empty if no such key exists in the
         *         %FITS file.
         */
        std::string
        read_fits_string_key(fitsfile * fptr, char const * key)
        {
            int status = 0;
            char value[FLEN_VALUE] = { '\0' };
            std::string str;
            /**
             * @note @c fits_read_key() doesn't support the HEASARC
             *       continued long string convention.  If needed, use
             *       a combination of @c fits_get_key_strlen() and
             *       @c fits_read_string_key(), both introduced in
             *       CFITSIO 3.39.
             */
            if (fits_read_key(fptr,
                              TSTRING,
                              key,
                              value,
                              nullptr, // comment
                              &status) == 0)
                str = value;
            else if (status != KEY_NO_EXIST) {
                FITS::throw_on_error(status);
            }

            return str;
        }
    }
}

// ----------------------------------------------------------------

void MaRC::FITS::throw_on_error(int status)
{
    if (status != 0) {
        char err[FLEN_STATUS] = { 0 };
        fits_get_errstatus(status, err);
        throw std::runtime_error(err);
    }
}

// ----------------------------------------------------------------

MaRC::FITS::file::file(char const * filename, bool create)
    : fptr_(open_fits_file(filename, create))
{
}

std::string
MaRC::FITS::file::author() const
{
    constexpr char const key[] = "AUTHOR";

    return read_fits_string_key(this->fptr_.get(), key);
}

int
MaRC::FITS::file::bitpix() const
{
    int bp = 0;
    int status = 0;

    if (fits_get_img_type(this->fptr_.get(),
                          &bp,
                          &status) != 0)
        throw_on_error(status);

    return bp;
}

MaRC::optional<MaRC::FITS::longlong_type>
MaRC::FITS::file::blank() const
{
    constexpr char const key[] = "BLANK";

    return read_fits_key<longlong_type>(this->fptr_.get(), key);
}

MaRC::optional<double>
MaRC::FITS::file::bscale() const
{
    constexpr char const key[] = "BSCALE";

    return read_fits_key<double>(this->fptr_.get(), key);
}

std::string
MaRC::FITS::file::bunit() const
{
    constexpr char const key[] = "BUNIT";

    return read_fits_string_key(this->fptr_.get(), key);
}

MaRC::optional<double>
MaRC::FITS::file::bzero() const
{
    constexpr char const key[] = "BZERO";

    return read_fits_key<double>(this->fptr_.get(), key);
}

MaRC::optional<double>
MaRC::FITS::file::datamax() const
{
    constexpr char const key[] = "DATAMAX";

    return read_fits_key<double>(this->fptr_.get(), key);
}

MaRC::optional<double>
MaRC::FITS::file::datamin() const
{
    constexpr char const key[] = "DATAMIN";

    return read_fits_key<double>(this->fptr_.get(), key);
}

MaRC::optional<double>
MaRC::FITS::file::equinox() const
{
    constexpr char const key[] = "EQUINOX";

    return read_fits_key<double>(this->fptr_.get(), key);
}

std::string
MaRC::FITS::file::instrument() const
{
    constexpr char const key[] = "INSTRUME";

    return read_fits_string_key(this->fptr_.get(), key);
}

int
MaRC::FITS::file::naxis() const
{
    int n = 0;
    int status = 0;

    if (fits_get_img_dim(this->fptr_.get(),
                         &n,
                         &status) != 0)
        throw_on_error(status);

    return n;
}

std::array<LONGLONG, 3>
MaRC::FITS::file::naxes() const
{
    std::array<LONGLONG, 3> n{0};
    int status = 0;

    if (fits_get_img_sizell(this->fptr_.get(),
                            n.size(),
                            n.data(),
                            &status) != 0)
        throw_on_error(status);

    return n;
}

std::string
MaRC::FITS::file::object() const
{
    constexpr char const key[] = "OBJECT";

    return read_fits_string_key(this->fptr_.get(), key);
}

std::string
MaRC::FITS::file::observer() const
{
    constexpr char const key[] = "OBSERVER";

    return read_fits_string_key(this->fptr_.get(), key);
}

std::string
MaRC::FITS::file::origin() const
{
    constexpr char const key[] = "ORIGIN";

    return read_fits_string_key(this->fptr_.get(), key);
}

std::string
MaRC::FITS::file::reference() const
{
    constexpr char const key[] = "REFERENC";

    return read_fits_string_key(this->fptr_.get(), key);
}

std::string
MaRC::FITS::file::telescope() const
{
    constexpr char const key[] = "TELESCOP";

    return read_fits_string_key(this->fptr_.get(), key);
}

// ----------------------------------------------------------------

MaRC::FITS::output_file::output_file(char const * filename)
    : file(filename, true)
{
}

std::unique_ptr<MaRC::FITS::image>
MaRC::FITS::output_file::make_image(int bitpix,
                                    size_t samples,
                                    size_t lines,
                                    size_t planes,
                                    char const * extname)
{
    return std::make_unique<MaRC::FITS::image>(this->fptr_,
                                               bitpix,
                                               samples,
                                               lines,
                                               planes,
                                               extname);
}


// ----------------------------------------------------------------

MaRC::FITS::input_file::input_file(char const * filename)
    : file(filename, false)
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

void
MaRC::FITS::input_file::read(std::vector<double> & image,
                             std::size_t & samples,
                             std::size_t & lines) const
{
    // Get the image parameters.

    /**
     * @note Only two-dimensional %FITS images are currently
     *       supported.
     */
    using naxes_array_type = std::array<LONGLONG, 2>;

    /// Array containing %FITS image dimensions.
    naxes_array_type naxes;

    int naxis = 0;
    int bitpix = 0;
    int status = 0;

    if (fits_get_img_paramll(this->fptr_.get(),
                             naxes.size(),
                             &bitpix,
                             &naxis,
                             naxes.data(),
                             &status) != 0)
        throw_on_error(status);

    // Sanity checks.
    if (naxis < static_cast<int>(naxes.size()))
        throw std::runtime_error("too few dimensions in FITS image");

    // Smallest image size MaRC will accept is 2x2.  Even that is too
    // small, but let's not be too picky.
    constexpr naxes_array_type::value_type mindim = 2;
    if (naxes[0] < mindim || naxes[1] < mindim)
        throw std::runtime_error("image dimension is too small");

    // CFITSIO wants its own LONGLONG type, not size_t.
    LONGLONG const nelements = naxes[0] * naxes[1];

    using image_type = std::remove_reference_t<decltype(image)>;
    using value_type = image_type::value_type;

    constexpr auto nan =
        std::numeric_limits<value_type>::signaling_NaN();

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
    LONGLONG fpixel[] = {1, 1};

    // For integer typed FITS images with a BLANK value, set the
    // "blank" value in our floating point converted copy of the image
    // to NaN.
    auto nulval = nan;
    int anynul = 0;  // Unused

    static_assert(std::is_same<value_type, decltype(nulval)>(),
                  "Nul value type doesn't match photo container type.");

    if (fits_read_pixll(this->fptr_.get(),
                        traits<value_type>::datatype,
                        fpixel,
                        nelements,
                        &nulval,  // "Blank" value in our image.
                        tmp.data(),
                        &anynul,  // Were any blank values found?
                        &status) != 0)
        throw_on_error(status);

    image   = std::move(tmp);
    samples = static_cast<std::size_t>(naxes[0]);
    lines   = static_cast<std::size_t>(naxes[1]);
}
