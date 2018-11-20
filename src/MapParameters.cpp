/**
 * @file MapParameters.cpp
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

 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#include "MapParameters.h"

namespace
{
    /**
     * @brief Validate given %FITS BITPIX value.
     *
     * @param[in] bitpix The bits-per-pixel value for data stored in a
     *                   %FITS file, as defined in the %FITS
     *                   standard.  Valid values are 8, 16, 32, 64,
     *                   -32, and -64.  The corresponding CFITSIO
     *                   library symbolic constants are @c BYTE_IMG,
     *                   @c SHORT_IMG, @c LONG_IMG, @c LONGLONG_IMG,
     *                   @c FLOAT_IMG, @c DOUBLE_IMG.
     *
     * @retval true  Valid @a bitpix value.
     * @retval false Invalid @a bitpix value.
     */
    bool valid_bitpix(int bitpix)
    {
        return bitpix == BYTE_IMG
            || bitpix == SHORT_IMG
            || bitpix == LONG_IMG
            || bitpix == LONGLONG_IMG
            || bitpix == FLOAT_IMG
            || bitpix == DOUBLE_IMG;
    }
}


/*
AUTHOR
BITPIX
BLANK
BSCALE
BUNIT
BZERO
DATAMAX
DATAMIN
EQUINOX
INSTRUME
NAXES
OBJECT
OBSERVER
ORIGIN
REFERENC
TELESCOP
 */



MaRC::MapParameters::MapParameters()
    : author_()
    , bitpix_()
    , blank_()
    , bscale_()
    , bunit_()
    , bzero_()
    , data_max_()
    , data_min_()
    , equinox_()
    , instrument_()
    , object_()
    , observer_()
    , origin_()
    , reference_()
    , telescope_()
{
}

bool
MaRC::MapParameters::populate_from()
{
    return false;
}

void
MaRC::MapParameters::bitpix(int n)
{
    // Zero means choose bitpix automatically.
    if (n != 0 && !valid_bitpix(n))
        throw std::invalid_argument("Invalid FITS BITPIX value");

    this->bitpix_ = n;
}

int
MaRC::MapParameters::bitpix()
{
    /**
     * @bug Choose BITPIX value based on SourceImage, such as the
     *      BITPIX value in a PhotoImage, or a BITPIX for floating
     *      point values for VirtualImages with floating point
     *      values etc.  Fixes #62.
     *
     * @todo Warn the user if the their desired BITPIX (map data type)
     *       is smaller than the data type in a photo.  (e.g. 16 bits
     *       chosen vs 32 bits in photo).  Fixes #72.
     */

    // This should never happen!
    if (!valid_bitpix(this->bitpix_))
        throw std::runtime_error("Unable to determine BITPIX value.");

    return this->bitpix_;
}

void
MaRC::MapParameters::author(std::string author)
{
    this->author_ = std::move(author);
}

void
MaRC::MapParameters::bzero(double zero)
{
    this->bzero_ = zero;
}

void
MaRC::MapParameters::bscale(double scale)
{
    this->bscale_ = scale;
}

void
MaRC::MapParameters::blank(blank_type blank)
{
    this->blank_ = blank;
}

void
MaRC::MapParameters::object(std::string object)
{
    this->object_ = std::move(object);
}

void
MaRC::MapParameters::origin(std::string origin)
{
    this->origin_ = std::move(origin);
}
