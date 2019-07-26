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

#include <fitsio.h>


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

/**
 * @todo Handle and/or validate parameters that are set multiple
 *       times!
 */

MaRC::MapParameters::MapParameters()
    : author_()
    , bitpix_(0)
    , blank_()
    , bscale_()
    , bunit_()
    , bzero_()
    , datamax_()
    , datamin_()
    , equinox_()
    , instrument_()
    , object_()
    , observer_()
    , origin_()
    , reference_()
    , telescope_()
    , comments_()
    , xcomments_()
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
    /**
     * @todo Verify that @a scale is valid, i.e. not the @c NaN
     *       constant.
     */
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
    /**
     * @todo Verify that @ a zero is valid, i.e. not the @c NaN
     *       constant.
     */

    this->bzero_ = zero;
}

void
MaRC::MapParameters::datamax(MaRC::optional<double> max)
{
    /**
     * @todo Verify that @ a max is valid, i.e. not the @c NaN
     *       constant.
     */

    this->datamax_ = max;
}

void
MaRC::MapParameters::datamin(MaRC::optional<double> min)
{
    /**
     * @todo Verify that @ a min is valid, i.e. not the @c NaN
     *       constant.
     */

    this->datamin_ = min;
}

void
MaRC::MapParameters::equinox(MaRC::optional<double> e)
{
    /**
     * @todo Verify that @ a equinox is valid, i.e. not the @c NaN
     *       constant.
     */

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

bool
MaRC::MapParameters::merge(MaRC::MapParameters & p)
{
    if (this->author_.empty())
        this->author_ = std::move(p.author_);

    if (this->bitpix_ == 0)
        this->bitpix_ = p.bitpix_;

    if (!this->blank_.has_value())
        this->blank_ = p.blank_;

    if (!this->bscale_.has_value())
        this->bscale_ = p.bscale_;

    if (this->bunit_.empty())
        this->bunit_ = std::move(p.bunit_);

    if (!this->bzero_.has_value())
        this->bzero_ = p.bzero_;

    if (!this->datamax_.has_value())
        this->datamax_ = p.datamax_;

    if (!this->datamin_.has_value())
        this->datamin_ = p.datamin_;

    if (!this->equinox_.has_value())
        this->equinox_ = p.equinox_;

    if (this->instrument_.empty())
        this->instrument_ = std::move(p.instrument_);

    if (this->object_.empty())
        this->object_ = std::move(p.object_);

    if (this->observer_.empty())
        this->observer_ = std::move(p.observer_);

    if (this->origin_.empty())
        this->origin_ = std::move(p.origin_);

    if (this->reference_.empty())
        this->reference_ = std::move(p.reference_);

    if (this->telescope_.empty())
        this->telescope_ = std::move(p.telescope_);

    this->comments_.splice(std::cend(this->comments_), p.comments_);
    this->xcomments_.splice(std::cend(this->xcomments_), p.xcomments_);

    return true;
}
