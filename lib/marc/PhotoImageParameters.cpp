/**
 * @file PhotoImageParameters.cpp
 *
 * Copyright (C) 1998-1999, 2003-2005, 2017, 2019  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "PhotoImageParameters.h"
#include "NullPhotometricCorrection.h"
#include "NullInterpolation.h"

#include <stdexcept>

#ifndef MARC_DEFAULT_PHOTO_CORR_STRATEGY
# define MARC_DEFAULT_PHOTO_CORR_STRATEGY MaRC::NullPhotometricCorrection
#endif  /* MARC_DEFAULT_PHOTO_CORR_STRATEGY */

#ifndef MARC_DEFAULT_INTERPOLATION_STRATEGY
# define MARC_DEFAULT_INTERPOLATION_STRATEGY MaRC::NullInterpolation
#endif  /* MARC_DEFAULT_INTERPOLATION_STRATEGY */


MaRC::PhotoImageParameters::PhotoImageParameters()
    : nibble_left_  (0)
    , nibble_right_ (0)
    , nibble_top_   (0)
    , nibble_bottom_(0)
    , photometric_correction_(
        std::make_unique<MARC_DEFAULT_PHOTO_CORR_STRATEGY>())
    , interpolation_strategy_(
        std::make_unique<MARC_DEFAULT_INTERPOLATION_STRATEGY>())
    , remove_sky_(false)
{
}

void
MaRC::PhotoImageParameters::photometric_correction(
    std::unique_ptr<PhotometricCorrection> strategy)
{
    if (!strategy) {
        throw
            std::invalid_argument(
                "Null photometric correction strategy argument.");
    }

    this->photometric_correction_ = std::move(strategy);
}

void
MaRC::PhotoImageParameters::interpolation_strategy(
    std::unique_ptr<InterpolationStrategy> strategy)
{
    if (!strategy) {
        throw
            std::invalid_argument("Null interpolation strategy argument.");
    }

    this->interpolation_strategy_ = std::move(strategy);
}

void
MaRC::PhotoImageParameters::nibble(std::size_t n)
{
    // std::size_t const minimum_dimension =
    //     std::min(this->samples_, this->lines_);

    // if (n < (minimum_dimension / 2)) {
        this->nibble_left_   = n;
        this->nibble_right_  = n;
        this->nibble_top_    = n;
        this->nibble_bottom_ = n;
    // } else {
    //     auto s = fmt::format("Invalid overall nibble value ({}).", n);

    //     throw std::invalid_argument(s);
    // }
}

void
MaRC::PhotoImageParameters::nibble_left(std::size_t n)
{
    // if (n < (this->samples_ - this->nibble_right_))
        this->nibble_left_ = n;
    // else {
    //     auto s = fmt::format("Invalid nibble left value ({}).", n);

    //     throw std::invalid_argument(s);
    // }
}

void
MaRC::PhotoImageParameters::nibble_right(std::size_t n)
{
    // if (n < (this->samples_ - this->nibble_left_))
        this->nibble_right_ = n;
    // else {
    //     auto s = fmt::format("Invalid nibble right value ({}).", n);

    //     throw std::invalid_argument(s);
    // }
}

void
MaRC::PhotoImageParameters::nibble_top(std::size_t n)
{
    // if (n < (this->lines_ - this->nibble_bottom_))
        this->nibble_top_ = n;
    // else {
    //     auto s = fmt::format("Invalid nibble top value ({}).", n);

    //     throw std::invalid_argument(s);
    // }
}

void
MaRC::PhotoImageParameters::nibble_bottom(std::size_t n)
{
    // if (n < (this->lines_ - this->nibble_top_))
        this->nibble_bottom_ = n;
    // else {
    //     auto s = fmt::format("Invalid nibble bottom value ({}).", n);

    //     throw std::invalid_argument(s);
    // }
}

void
MaRC::PhotoImageParameters::remove_sky(bool r)
{
    this->remove_sky_ = r;
}

void
MaRC::PhotoImageParameters::validate_parameters(std::size_t samples,
                                                std::size_t lines) const
{
    // Run some sanity checks on nibbling values
    if (samples - this->nibble_right_ <= this->nibble_left_) {
        throw
            std::invalid_argument(
                "Either the left or right (or both) "
                "nibble value is too large.");
    }

    if (lines - this->nibble_top_ <= this->nibble_bottom_) {
        throw
            std::invalid_argument(
                "Either the top or bottom (or both) "
                "nibble value is too large.");
    }
}
