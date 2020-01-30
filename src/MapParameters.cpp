/**
 * @file MapParameters.cpp
 *
 * Copyright (C) 2018-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "MapParameters.h"

#include "marc/Log.h"
#include "marc/Mathematics.h"

#include <fitsio.h>

#include <stdexcept>
#include <cmath>


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

MaRC::MapParameters::MapParameters(int plane)
    : locked_keywords_()
    , user_supplied_(plane == 0)
    , plane_(plane)
    , author_()
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
    , histories_()
{
    if (plane < 0)
        throw std::invalid_argument(
            fmt::format("Invalid map plane: {}", plane));
}

MaRC::MapParameters::MapParameters()
    : MapParameters(0)
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
        throw std::invalid_argument(
            fmt::format("Invalid FITS BITPIX value: {}", n));

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
     * @todo Verify that @a zero is valid, i.e. not the @c NaN
     *       constant.
     */

    this->bzero_ = zero;
}

void
MaRC::MapParameters::datamax(MaRC::optional<double> max)
{
    /**
     * @todo Verify that @a max is valid, i.e. not the @c NaN
     *       constant.
     */

    this->datamax_ = max;
}

void
MaRC::MapParameters::datamin(MaRC::optional<double> min)
{
    /**
     * @todo Verify that @a min is valid, i.e. not the @c NaN
     *       constant.
     */

    this->datamin_ = min;
}

void
MaRC::MapParameters::equinox(MaRC::optional<double> e)
{
    /**
     * @todo Verify that @a e is valid, i.e. not the @c NaN constant.
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

/**
 * @brief Convenience macro that merges a given map parameter.
 *
 * Merge a provided map parameter from @c p into @c *this.  Cause the
 * calling function (@c merge()) to return immediately on error, and
 * continue otherwise.
 *
 * @param[in] key       Parameter name.
 * @param[in] parameter @c MapParameter member, e.g. (@c origin_).
 *
 */
#define MARC_MERGE_OPTIONAL(parameter)              \
    if (!this->merge_optional(#parameter,           \
                              this->parameter ## _, \
                              p.parameter ## _))    \
        return false

bool
MaRC::MapParameters::merge(MaRC::MapParameters & p)
{
    // Merge required parameters.
    if (p.bitpix_ != 0
        && (this->bitpix_ == 0 || !this->user_supplied_)) {
        /**
         * @bug Choose @c BITPIX value based on @c SourceImage, such
         *      as the @c BITPIX value in a @c PhotoImage, or a
         *      @c BITPIX for floating point values for a
         *      @c VirtualImage with floating point values etc.  Fixes
         *      #62.
         *
         * @todo Warn the user if the their desired @c BITPIX (map
         *       data type) is smaller than the data type in a photo.
         *       (e.g. 16 bits chosen vs 32 bits in photo).  Fixes
         *       #72.
         *
         * @todo Should we allow floating point @c BITPIX override
         *       with integer @c BITPIX?  The original use case was to
         *       allow the user to prefer integer map data over
         *       floating point map data.  Is that relevant for the
         *       automatically populated @c BITPIX case?
         */

        /*
          Do not decrease map data type size.

          Choose BITPIX value corresponding to larger data type.
        */
        if (this->bitpix_ == 0                 // not previously set
            || (p.bitpix_ > 0
                && p.bitpix_ > this->bitpix_)  // integer (override float)
            || (p.bitpix_ < 0
                && p.bitpix_ < this->bitpix_)) // floating point
            this->bitpix(p.bitpix_);
    }

    // Merge optional parameters.
    MARC_MERGE_OPTIONAL(author);
    MARC_MERGE_OPTIONAL(blank);
    MARC_MERGE_OPTIONAL(bscale);
    MARC_MERGE_OPTIONAL(bunit);
    MARC_MERGE_OPTIONAL(bzero);
    MARC_MERGE_OPTIONAL(equinox);
    MARC_MERGE_OPTIONAL(instrument);
    MARC_MERGE_OPTIONAL(object);
    MARC_MERGE_OPTIONAL(observer);
    MARC_MERGE_OPTIONAL(origin);
    MARC_MERGE_OPTIONAL(reference);
    MARC_MERGE_OPTIONAL(telescope);

    /**
     * @bug This could cause data to be dropped from the map if the
     *      image factory for a given map plane doesn't set minimum
     *      and maximum data values.
     */
    /*
      Choose minimum and maximum values large enough to cover the
      full range of data value in multiple map planes.
     */
    if (this->datamax_ < p.datamax_)
        this->datamax_ = p.datamax_;

    if (this->datamin_ > p.datamin_)
        this->datamin_ = p.datamin_;

    this->comments_.splice(std::cend(this->comments_), p.comments_);
    this->xcomments_.splice(std::cend(this->xcomments_), p.xcomments_);

    return true;
}

bool
MaRC::MapParameters::merge_optional(char const * key,
                                    MaRC::optional<double> & to,
                                    MaRC::optional<double> & from)
{
    if (!from.has_value() || std::isnan(*from))
        return true;

    auto const r = this->locked_keywords_.emplace(key);
    bool const assignable = r.second;
    constexpr int ulps = 2;

    if (!to) {
        // Not previously set.
        to = from;
        return true;
    } else if (MaRC::almost_equal(*to, *from, ulps)) {
        // Same value.  No need to reassign.
        return true;
    }

    // Different values between map planes.

    /**
     * @todo Document different values for the same map parameter in
     *       %FITS %c HISTORY cards, and clear the @a "to"
     *       value if it wasn't supplied by the user since we
     *       can't use it.
     */
    MaRC::warn("Different {} values between planes ({}, {}).",
               key,
               *to,
               *from);

    if (this->user_supplied_)
        MaRC::warn("User supplied value {} will be used.", *to);
    else {
        MaRC::warn("Neither will be used.");

        std::string history =
            fmt::format("Plane {}: {} = {}",
                        this->plane_,
                        key,
                        *to);

        this->histories_.emplace_back(history);
    }

    return true;
}

bool
MaRC::MapParameters::merge_optional(char const * key,
                                    MaRC::blank_type & to,
                                    MaRC::blank_type & from)
{
    if (!from.has_value())
        return true;

    auto const r = this->locked_keywords_.emplace(key);
    bool const assignable = r.second;

    if (!to.has_value()) {
        // Not previously set.
        to = from;
        return true;
    } else if (to == from) {
        // Same value.  No need to reassign.
        return true;
    }

    // Different values between map planes.

    /**
     * @todo Document different values for the same map parameter in
     *       %FITS %c HISTORY cards, and clear the @a "to"
     *       value if it wasn't supplied by the user since we
     *       can't use it.
     */
    MaRC::warn("Different {} values between planes ({}, {}).",
               key,
               *to,
               *from);

    if (this->user_supplied_)
        MaRC::warn("User supplied value {} will be used.", *to);
    else {
        MaRC::warn("Neither will be used.");

        std::string history =
            fmt::format("Plane {}: {} = {}",
                        this->plane_,
                        key,
                        *to);

        this->histories_.emplace_back(history);
    }

    return true;
}

bool
MaRC::MapParameters::merge_optional(char const * /* key */,
                                    std::string & to,
                                    std::string from)
{
    if (to.empty())
        to = std::move(from);
    // else
    //     MaRC::warn();

    return true;
}
