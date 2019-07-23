/**
 * @file MapParameters.cpp
 *
 * Copyright (C) 2018-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "MapParameters.h"

#include <limits>

#include <fitsio.h>


namespace
{
    static constexpr double not_a_number =
        std::numeric_limits<double>::signaling_NaN();

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
. AUTHOR
. BITPIX
. BLANK
. BSCALE
. BUNIT
. BZERO
. DATAMAX
. DATAMIN
. EQUINOX
. INSTRUME
NAXES
. OBJECT
. OBSERVER
. ORIGIN
. REFERENC
. TELESCOP
 */

/**
 * @todo Handle and/or validate parameters that are set multiple
 *       times!
 */

MaRC::MapParameters::MapParameters()
    : author_()
    , bitpix_(0)
    , blank_()
    , bscale_(not_a_number)
    , bunit_()
    , bzero_(not_a_number)
    , datamax_(not_a_number)
    , datamin_(not_a_number)
    , equinox_(not_a_number)
    , instrument_()
    , object_()
    , observer_()
    , origin_()
    , reference_()
    , telescope_()
    , comments_()
{
}

void
MaRC::MapParameters::author(std::string a)
{
    this->author_ = std::move(a);
}

void
MaRC::MapParameters::bitpix(int n)
{
    if (!valid_bitpix(n))
        throw std::invalid_argument("Invalid FITS BITPIX value");

    /**
     * @bug Choose @c BITPIX value based on @c SourceImage, such as
     *      the @c BITPIX value in a @c PhotoImage, or a @c BITPIX for
     *      floating point values for a @c VirtualImage with floating
     *      point values etc.  Fixes #62.
     *
     * @todo Warn the user if the their desired @c BITPIX (map data
     *       type) is smaller than the data type in a photo.  (e.g. 16
     *       bits chosen vs 32 bits in photo).  Fixes #72.
     */

    /*
      Do not decrease map data type size.

      Choose BITPIX value corresponding to larger data type.
    */
    if (this->bitpix_ == 0               // not previously set
        || (n > 0 && n > this->bitpix_)  // integer (override float)
        || (n < 0 && n < this->bitpix_)) // floating point
        this->bitpix_ = n;
}

int
MaRC::MapParameters::bitpix() const
{
    // This should never happen!
    if (!valid_bitpix(this->bitpix_))
        throw std::runtime_error("Unable to determine BITPIX value.");

    return this->bitpix_;
}

void
MaRC::MapParameters::blank(blank_type blank)
{

    this->blank_ = blank;
}

void
MaRC::MapParameters::bscale(MaRC::optional<double> scale)
{
    this->bscale_ = scale;
}

void
MaRC::MapParameters::bunit(std::string unit)
{
    this->bunit_ = std::move(unit);
}

void
MaRC::MapParameters::bzero(MaRC::optional<double> zero)
{
    this->bzero_ = zero;
}

void
MaRC::MapParameters::datamax(MaRC::optional<double> max)
{
    this->datamax_ = max;
}

void
MaRC::MapParameters::datamin(MaRC::optional<double> min)
{
    this->datamin_ = min;
}

void
MaRC::MapParameters::equinox(MaRC::optional<double> e)
{
    this->equinox_ = e;
}

void
MaRC::MapParameters::instrument(std::string i)
{
    this->instrument_ = std::move(i);
}

void
MaRC::MapParameters::object(std::string o)
{
    this->object_ = std::move(o);
}

void
MaRC::MapParameters::observer(std::string o)
{
    this->observer_ = std::move(o);
}

void
MaRC::MapParameters::origin(std::string o)
{
    this->origin_ = std::move(o);
}

void
MaRC::MapParameters::reference(std::string r)
{
    this->reference_ = std::move(r);
}

void
MaRC::MapParameters::telescope(std::string t)
{
    this->telescope_ = std::move(t);
}

void
MaRC::MapParameters::push_comment(comment_list_type::value_type comment)
{
    this->comments_.push_back(std::move(comment));
}

void
MaRC::MapParameters::push_xcomment(comment_list_type::value_type comment)
{
    this->xcomments_.push_back(std::move(comment));
}
