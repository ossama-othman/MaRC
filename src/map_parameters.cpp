/**
 * @file map_parameters.cpp
 *
 * Copyright (C) 2018-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "map_parameters.h"

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

MaRC::map_parameters::map_parameters(int plane)
    : locked_parameters_()
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

MaRC::map_parameters::map_parameters()
    : map_parameters(0)
{
}

void
MaRC::map_parameters::author(std::string a)
{
    this->author_ = std::move(a);
    this->lock_parameter(param_key::author);
}

void
MaRC::map_parameters::bitpix(int n)
{
    if (!valid_bitpix(n))
        throw std::invalid_argument(
            fmt::format("Invalid FITS BITPIX value: {}", n));

    this->bitpix_ = n;
}

int
MaRC::map_parameters::bitpix() const
{
    // This should never happen!
    if (!valid_bitpix(this->bitpix_))
        throw std::runtime_error("Unable to determine BITPIX value.");

    return this->bitpix_;
}

void
MaRC::map_parameters::blank(blank_type blank)
{

    this->blank_ = blank;
    this->lock_parameter(param_key::blank);
}

void
MaRC::map_parameters::bscale(std::optional<double> scale)
{
    /**
     * @todo Verify that @a scale is valid, i.e. not the @c NaN
     *       constant.
     */
    this->bscale_ = scale;
    this->lock_parameter(param_key::bscale);
}

void
MaRC::map_parameters::bunit(std::string unit)
{
    this->bunit_ = std::move(unit);
    this->lock_parameter(param_key::bunit);
}

void
MaRC::map_parameters::bzero(std::optional<double> zero)
{
    /**
     * @todo Verify that @a zero is valid, i.e. not the @c NaN
     *       constant.
     */

    this->bzero_ = zero;
    this->lock_parameter(param_key::bzero);
}

void
MaRC::map_parameters::datamax(std::optional<double> max)
{
    /**
     * @todo Verify that @a max is valid, i.e. not the @c NaN
     *       constant.
     */

    this->datamax_ = max;
    this->lock_parameter(param_key::datamax);
}

void
MaRC::map_parameters::datamin(std::optional<double> min)
{
    /**
     * @todo Verify that @a min is valid, i.e. not the @c NaN
     *       constant.
     */

    this->datamin_ = min;
    this->lock_parameter(param_key::datamin);
}

void
MaRC::map_parameters::equinox(std::optional<double> e)
{
    /**
     * @todo Verify that @a e is valid, i.e. not the @c NaN constant.
     */

    this->equinox_ = e;
    this->lock_parameter(param_key::equinox);
}

void
MaRC::map_parameters::instrument(std::string i)
{
    this->instrument_ = std::move(i);
    this->lock_parameter(param_key::instrument);
}

void
MaRC::map_parameters::object(std::string o)
{
    this->object_ = std::move(o);
    this->lock_parameter(param_key::object);
}

void
MaRC::map_parameters::observer(std::string o)
{
    this->observer_ = std::move(o);
    this->lock_parameter(param_key::observer);
}

void
MaRC::map_parameters::origin(std::string o)
{
    this->origin_ = std::move(o);
    this->lock_parameter(param_key::origin);
}

void
MaRC::map_parameters::reference(std::string r)
{
    this->reference_ = std::move(r);
    this->lock_parameter(param_key::reference);
}

void
MaRC::map_parameters::telescope(std::string t)
{
    this->telescope_ = std::move(t);
    this->lock_parameter(param_key::telescope);
}

void
MaRC::map_parameters::push_comment(comment_list_type::value_type comment)
{
    this->comments_.push_back(std::move(comment));
    this->lock_parameter(param_key::comment);
}

void
MaRC::map_parameters::push_xcomment(comment_list_type::value_type comment)
{
    this->xcomments_.push_back(std::move(comment));
    this->lock_parameter(param_key::xcomment);
}

void
MaRC::map_parameters::lock_parameter(MaRC::map_parameters::param_key key)
{
    // Prevent plane parameter from overriding user supplied value
    // in the future.
    if (this->user_supplied())
        this->locked_parameters_.set(static_cast<std::size_t>(key));
}

bool
MaRC::map_parameters::is_mergeable(
    MaRC::map_parameters::param_key key) const
{
    return !this->locked_parameters_.test(static_cast<std::size_t>(key))
        || !this->user_supplied();
}

/**
 * @brief Convenience macro that merges a given map parameter.
 *
 * Merge a provided map parameter from @c p into @c *this.  Cause the
 * calling function (@c merge()) to return immediately on error, and
 * continue otherwise.
 *
 * @param[in] parameter @c map_parameters member, e.g. (@c origin_).
 */
#define MARC_MERGE_OPTIONAL(parameter)               \
    if (!this->merge_optional(param_key:: parameter, \
                              #parameter,            \
                              this->parameter ## _,  \
                              p.parameter ## _))     \
        return false

bool
MaRC::map_parameters::merge(MaRC::map_parameters & p)
{
    // Merge required parameters.
    if (p.bitpix_ != 0
        && (this->bitpix_ == 0 || !this->user_supplied())) {
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
     * @note This could cause data to be dropped from the map if
     *      the image factory for a given map plane doesn't set
     *      minimum and maximum data values.  Instead, do not attempt
     *      to automatically populate the @c DATAMAX and @c DATAMIN
     *      parameters from the source images since MaRC will either
     *      use the user supplied values or set them based on the data
     *      was actually mapped.
     */
    /*
      Choose minimum and maximum values large enough to cover the
      full range of data value in multiple map planes.
    */
    /*
    if (this->is_mergeable(param_key::datamax)
        && this->datamax_ < p.datamax_)
        this->datamax_ = p.datamax_;

    if (this->is_mergeable(param_key::datamin)
        && this->datamin_ > p.datamin_)
        this->datamin_ = p.datamin_;
    */

    if (this->is_mergeable(param_key::comment))
        this->comments_.splice(std::cend(this->comments_),
                               p.comments_);

    if (this->is_mergeable(param_key::xcomment))
        this->xcomments_.splice(std::cend(this->xcomments_),
                                p.xcomments_);

    return true;
}

bool
MaRC::map_parameters::merge_optional(MaRC::map_parameters::param_key key,
                                     char const * name,
                                     std::optional<double> & to,
                                     std::optional<double> & from)
{
    if (!from.has_value()
        || std::isnan(*from)
        || !this->is_mergeable(key))
        return true;

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
               name,
               *to,
               *from);

    if (this->user_supplied())
        MaRC::warn("User supplied value ({}) will be used.", *to);
    else {
        MaRC::warn("Neither will be used.");

        std::string history =
            fmt::format("Plane {}: {} = {}",
                        this->plane_,
                        name,
                        *to);

        this->histories_.emplace_back(history);
    }

    return true;
}

bool
MaRC::map_parameters::merge_optional(MaRC::map_parameters::param_key key,
                                     char const * name,
                                     MaRC::blank_type & to,
                                     MaRC::blank_type & from)
{
    if (!from.has_value() || !this->is_mergeable(key))
        return true;

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
               name,
               *to,
               *from);

    if (this->user_supplied())
        MaRC::warn("User supplied value ({}) will be used.", *to);
    else {
        MaRC::warn("Neither will be used.");

        std::string history =
            fmt::format("Plane {}: {} = {}",
                        this->plane_,
                        name,
                        *to);

        this->histories_.emplace_back(history);
    }

    return true;
}

bool
MaRC::map_parameters::merge_optional(MaRC::map_parameters::param_key key,
                                     char const * /* name */,
                                     std::string & to,
                                     std::string from)
{
    if (!this->is_mergeable(key))
        return true;

    if (to.empty())
        to = std::move(from);
    // else
    //     MaRC::warn();

    return true;
}
