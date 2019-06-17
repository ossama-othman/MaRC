/**
 * @file FITS_image.cpp
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
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

#include "FITS_image.h"

#include <cmath>


MaRC::FITS::image::image(MaRC::FITS::file::shared_ptr fptr,
                         int bitpix,
                         std::size_t samples,
                         std::size_t lines,
                         std::size_t planes,
                         char const * extname)
    : fptr_(fptr)
    , fpixel_(1)  // CFITSIO first pixel is 1, not 0.
    , nelements_(samples * lines)
    , max_elements_(nelements_ * planes)
{
    int const naxis =
        (planes > 1
         ? 3  /* 3 dimensions -- image "cube"  */
         : 2  /* 2 dimensions -- image "plane" */);

    /*
      Specify image cube dimensions.  Note that in the
      two-dimensional map case, the third "planes" dimension will be
      ignored since the above "naxis" variable will be set to two,
      not three.

      The CFITSIO 'naxes' value is of type 'LONGLONG'.  It is
      extremely unlikely that a map with more than LONGLONG_MAX
      samples,lines, or planes will ever be created, so the implicit
      conversions should be safe.
    */
    LONGLONG naxes[] = { static_cast<LONGLONG>(samples),
                         static_cast<LONGLONG>(lines),
                         static_cast<LONGLONG>(planes) };

    int status = 0;

    /* Bits per pixel values:
          8  8 bit unsigned integer data.
          16 16 bit signed   integer data.
          32 32 bit signed   integer data.
          64 64 bit signed   integer data.
          32 32 bit floating point   data.
         -64 64 bit floating point   data.
    */

    // Create the primary array.
    (void) fits_create_imgll(fptr.get(),
                             bitpix,
                             naxis,
                             naxes,
                             &status);

    update_fits_key(fptr.get(),
                    "EXTNAME",
                    extname,
                    PACKAGE_NAME " extension name");

    /**
     * Establish that %MaRC created this %FITS image HDU by setting
     * the @c CREATOR keyword appropriately, e.g. "MaRC 1.0".
     *
     * @note The @c CREATOR keyword is commonly used, but not part of
     *       the %FITS standard.
     * @note We could also use the @c PROGRAM keyword here instead.
     *       It is also commonly used but not in the %FITS standard.
     */
    update_fits_key(fptr.get(),
                    "CREATOR",
                    PACKAGE_STRING,
                    "software that created this FITS image");

    MaRC::FITS::throw_on_error(status);
}

MaRC::FITS::image::~image()
{
    int status = 0;

    // Write the current date and time (i.e. the creation time) into
    // the map FITS file.
    fits_write_date(this->fptr_.get(), &status);

    // Write a checksum for the image.
    fits_write_chksum(this->fptr_.get(), &status);

    fits_report_error(stderr, status);  // Do not throw in destructor!
}

void
MaRC::FITS::image::author(std::string const & a)
{
    this->update_fits_key(this->fptr_.get(),
                          "AUTHOR",
                          a,
                          "who compiled original data that was mapped");
}

void
MaRC::FITS::image::bzero(double zero)
{
    if (!std::isnan(zero))
        this->update_fits_key(this->fptr_.get(),
                              "BZERO",
                              zero,
                              "physical value corresponding to "
                              "zero in the map");
}

void
MaRC::FITS::image::bscale(double scale)
{
    if (!std::isnan(scale))
        this->update_fits_key(this->fptr_.get(),
                              "BSCALE",
                              scale,
                              "linear data scaling coefficient");
}

void
MaRC::FITS::image::bunit(std::string const & unit)
{
    this->update_fits_key(this->fptr_.get(),
                          "BUNIT",
                          unit,
                          "physical unit of the array values");
}

void
MaRC::FITS::image::datamin(double min)
{
    if (!std::isnan(min))
        this->update_fits_key(this->fptr_.get(),
                              "DATAMIN",
                              min,
                              "minimum valid physical data value");
}

void
MaRC::FITS::image::datamax(double max)
{
    if (!std::isnan(max))
        this->update_fits_key(this->fptr_.get(),
                              "DATAMAX",
                              max,
                              "maximum valid physical data value");
}

void
MaRC::FITS::image::object(std::string const & o)
{
    this->update_fits_key(this->fptr_.get(),
                          "OBJECT",
                          o,
                          "name of observed object");
}

void
MaRC::FITS::image::origin(std::string const & o)
{
    this->update_fits_key(this->fptr_.get(),
                          "ORIGIN",
                          o,
                          "map creator organization");
}

void
MaRC::FITS::image::comment(std::string const & c)
{
    int status = 0;

    fits_write_comment(this->fptr_.get(), c.c_str(), &status);

    MaRC::FITS::throw_on_error(status);
}

void
MaRC::FITS::image::history(std::string const & h)
{
    int status = 0;

    fits_write_history(this->fptr_.get(), h.c_str(), &status);

    MaRC::FITS::throw_on_error(status);
}

void
MaRC::FITS::image::internal_scale(double scale, double offset)
{
    int status = 0;

    fits_set_bscale(this->fptr_.get(), scale, offset, &status);

    MaRC::FITS::throw_on_error(status);
}

void
MaRC::FITS::image::update_fits_key(fitsfile * fptr,
                                   char const * key,
                                   std::string const & value,
                                   char const * comment)
{
    int status = 0;

    if (!value.empty())
        fits_update_key(fptr,
                        TSTRING,
                        key,
                        const_cast<char *>(value.c_str()),
                        comment,
                        &status);

    MaRC::FITS::throw_on_error(status);
}

void
MaRC::FITS::image::update_fits_key(fitsfile * fptr,
                                   char const * key,
                                   char const * value,
                                   char const * comment)
{
    int status = 0;

    if (value != nullptr)
        fits_update_key(fptr,
                        TSTRING,
                        key,
                        const_cast<char *>(value),
                        comment,
                        &status);

    MaRC::FITS::throw_on_error(status);
}
