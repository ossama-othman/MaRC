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
#include "OblateSpheroid.h"  /// @todo Remove
#include "NullGeometricCorrection.h"
#include "NullPhotometricCorrection.h"
#include "NullInterpolation.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

#ifndef MARC_DEFAULT_GEOM_CORR_STRATEGY
# define MARC_DEFAULT_GEOM_CORR_STRATEGY MaRC::NullGeometricCorrection
#endif  /* MARC_DEFAULT_GEOM_CORR_STRATEGY */

#ifndef MARC_DEFAULT_PHOTO_CORR_STRATEGY
# define MARC_DEFAULT_PHOTO_CORR_STRATEGY MaRC::NullPhotometricCorrection
#endif  /* MARC_DEFAULT_PHOTO_CORR_STRATEGY */

#ifndef MARC_DEFAULT_INTERPOLATION_STRATEGY
# define MARC_DEFAULT_INTERPOLATION_STRATEGY MaRC::NullInterpolation
#endif  /* MARC_DEFAULT_INTERPOLATION_STRATEGY */


MaRC::PhotoImageParameters::PhotoImageParameters(
    std::shared_ptr<OblateSpheroid> body)
    : body_(body)
    , image_()
    , samples_(0)
    , lines_(0)
    , nibble_left_   (0)
    , nibble_right_  (0)
    , nibble_top_    (0)
    , nibble_bottom_ (0)
    , geometric_correction_(
        std::make_unique<MARC_DEFAULT_GEOM_CORR_STRATEGY>())
    , photometric_correction_(
        std::make_unique<MARC_DEFAULT_PHOTO_CORR_STRATEGY>())
    , interpolation_(
        std::make_unique<MARC_DEFAULT_INTERPOLATION_STRATEGY>())
{
}

void
MaRC::PhotoImageParameters::image_info(std::vector<double> && image,
                                       std::size_t samples,
                                       std::size_t lines)
{
    if (samples < 2 || lines < 2) {
        // Why would there ever be a one pixel source image?
        std::ostringstream s;
        s << "Source image samples (" << samples
          << ") and lines (" << lines
          << ") must both be greater than one.";

        throw std::invalid_argument(s.str());
    }

    if (this->image_.size() != samples * lines) {
        throw std::invalid_argument(
            "Source image size does not match samples and lines");
    }
}

void
MaRC::PhotoImageParameters::geometric_correction(
    std::unique_ptr<GeometricCorrection> strategy)
{
    if (!strategy) {
        std::invalid_argument(
            "Null geometric correction strategy argument.");
    }

    this->geometric_correction_ = std::move(strategy);
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
MaRC::PhotoImageParameters::finalize_setup()
{
    // Run some sanity checks on nibbling values
    if (this->samples_ - this->nibble_right_ < this->nibble_left_) {
        std::cerr
            << "WARNING: Either the left or right (or both) nibble "
               "value is too large.\n"
               "         Both the left and right nibble values will "
               "be set to zero.\n";
        this->nibble_left_  = 0;
        this->nibble_right_ = 0;
    }

    if (this->lines_ - this->nibble_top_ < this->nibble_bottom_) {
        std::cerr
            << "WARNING: Either the top or bottom (or both) nibble value "
            << "is too large.\n"
            << "         Both the top and bottom nibble values will "
               "be set to zero.\n";
        this->nibble_top_    = 0;
        this->nibble_bottom_ = 0;
    }

    // All necessary image values and attributes should be set by now!

    // Scan across and determine where points lie off of the body, i.e.
    // remove the sky from the image.  The image will not actually be
    // modified.
    this->remove_sky(); // Remove sky from source image
}

void
MaRC::PhotoImageParameters::remove_sky(bool r)
{
    if (r)
        this->sky_mask_.resize(this->samples_ * this->lines_, false);
    else
        this->sky_mask_.clear();
}

void
MaRC::PhotoImageParameters::remove_sky()
{
    if (this->sky_mask_.empty())
        return;

    /**
     * @todo This routine is currently oblate spheroid specific.
     */

    std::size_t const llen = this->lines_   - this->nibble_bottom_;
    std::size_t const slen = this->samples_ - this->nibble_right_;

    for (std::size_t k = this->nibble_top_; k < llen; ++k) {
        std::size_t const offset = k * this->samples_;

        for (std::size_t i = this->nibble_left_; i < slen; ++i) {
            std::size_t const index =  offset + i;

            /**
             * Consider NaN data points invalid, i.e. "off the body".
             * No need to continue beyond this point.
             *
             * @todo Check for a user-specified "blank" value as
             *       well.
             */
            if (std::isnan(this->image_[index]))
                continue;

            double z = k;  // Reset "z" prior to geometric
                           // correction. Do not move to outer loop!
            double x = i;

            // Convert from image space to object space.
            this->geometric_correction_->image_to_object(z, x);

            z -= this->line_center_;
            x -= this->sample_center_;

            // ---------------------------------------------
            // Convert from observer coordinates to body coordinates
            DVector coord;
            coord[0] =  x;
            coord[1] =  0;
            coord[2] = -z; // Negative since line numbers increase top to
                           // bottom (?)

            // Do the transformation
            DVector rotated(this->observ2body_ * coord);
            rotated *= this->km_per_pixel_;

            // ---------------------------------------------

            // Vector from observer to point on image
            DVector const dVec(rotated - this->range_b_);

            double lat = 0, lon = 0;

            if (this->body_->ellipse_intersection(this->range_b_,
                                                  dVec,
                                                  lat,
                                                  lon) == 0) {
                // On body
                this->sky_mask_[index] = true;
            }
        }
    }
}

void
MaRC::PhotoImageParameters::nibble(std::size_t n)
{
    std::size_t const minimum_dimension =
        std::min(this->samples_, this->lines_);

    if (n < (minimum_dimension / 2)) {
      this->nibble_left_   = n;
      this->nibble_right_  = n;
      this->nibble_top_    = n;
      this->nibble_bottom_ = n;
    } else {
      std::ostringstream s;
      s << "Invalid overall nibble value (" << n << ")";

      throw std::invalid_argument(s.str());
    }
}

void
MaRC::PhotoImageParameters::nibble_left(std::size_t n)
{
    if (n < (this->samples_ - this->nibble_right_))
        this->nibble_left_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble left value (" << n << ")";

        throw std::invalid_argument(s.str());
    }
}

void
MaRC::PhotoImageParameters::nibble_right(std::size_t n)
{
    if (n < (this->samples_ - this->nibble_left_))
        this->nibble_right_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble right value (" << n << ")";

        throw std::invalid_argument(s.str());
    }
}

void
MaRC::PhotoImageParameters::nibble_top(std::size_t n)
{
    if (n < (this->lines_ - this->nibble_bottom_))
        this->nibble_top_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble top value (" << n << ")";

        throw std::invalid_argument(s.str());
    }
}

void
MaRC::PhotoImageParameters::nibble_bottom(std::size_t n)
{
    if (n < (this->lines_ - this->nibble_top_))
        this->nibble_bottom_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble bottom value (" << n << ")";

        throw std::invalid_argument (s.str());
    }
}
