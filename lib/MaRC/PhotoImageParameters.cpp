/**
 * @file PhotoImage.cpp
 *
 * Copyright (C) 1998-1999, 2003-2005, 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#include "PhotoImageParameters.h"
#include "NullPhotometricCorrection.h"
#include "NullInterpolation.h"

#include <stdexcept>
// #include <iostream>
// #include <sstream>

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
    //   std::ostringstream s;
    //   s << "Invalid overall nibble value (" << n << ")";

    //   throw std::invalid_argument(s.str());
    // }
}

void
MaRC::PhotoImageParameters::nibble_left(std::size_t n)
{
    // if (n < (this->samples_ - this->nibble_right_))
        this->nibble_left_ = n;
    // else {
    //     std::ostringstream s;
    //     s << "Invalid nibble left value (" << n << ")";

    //     throw std::invalid_argument(s.str());
    // }
}

void
MaRC::PhotoImageParameters::nibble_right(std::size_t n)
{
    // if (n < (this->samples_ - this->nibble_left_))
        this->nibble_right_ = n;
    // else {
    //     std::ostringstream s;
    //     s << "Invalid nibble right value (" << n << ")";

    //     throw std::invalid_argument(s.str());
    // }
}

void
MaRC::PhotoImageParameters::nibble_top(std::size_t n)
{
    // if (n < (this->lines_ - this->nibble_bottom_))
        this->nibble_top_ = n;
    // else {
    //     std::ostringstream s;
    //     s << "Invalid nibble top value (" << n << ")";

    //     throw std::invalid_argument(s.str());
    // }
}

void
MaRC::PhotoImageParameters::nibble_bottom(std::size_t n)
{
    // if (n < (this->lines_ - this->nibble_top_))
        this->nibble_bottom_ = n;
    // else {
    //     std::ostringstream s;
    //     s << "Invalid nibble bottom value (" << n << ")";

    //     throw std::invalid_argument(s.str());
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
