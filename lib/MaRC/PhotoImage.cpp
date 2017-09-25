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

#include "PhotoImage.h"
#include "Constants.h"
#include "OblateSpheroid.h"
#include "NullGeometricCorrection.h"
#include "NullPhotometricCorrection.h"
#include "NullInterpolationStrategy.h"
#include "PhotoInterpolationStrategy.h"
#include "Mathematics.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cassert>

#ifndef MARC_DEFAULT_GEOM_CORR_STRATEGY
# define MARC_DEFAULT_GEOM_CORR_STRATEGY MaRC::NullGeometricCorrection
#endif  /* MARC_DEFAULT_GEOM_CORR_STRATEGY */

#ifndef MARC_DEFAULT_PHOTO_CORR_STRATEGY
# define MARC_DEFAULT_PHOTO_CORR_STRATEGY MaRC::NullPhotometricCorrection
#endif  /* MARC_DEFAULT_PHOTO_CORR_STRATEGY */

#ifndef MARC_DEFAULT_INTERPOLATION_STRATEGY
# define MARC_DEFAULT_INTERPOLATION_STRATEGY MaRC::NullInterpolationStrategy
#endif  /* MARC_DEFAULT_INTERPOLATION_STRATEGY */


MaRC::PhotoImage::PhotoImage(std::shared_ptr<OblateSpheroid> body,
                             std::vector<double> && image,
                             std::size_t samples,
                             std::size_t lines,
                             std::unique_ptr<GeometricCorrection> gc)
    : SourceImage()
    , body_(body)
    , image_(std::move(image))
    , samples_(samples)
    , lines_(lines)
    , geometric_correction_(gc
                            ? std::move(gc)
                            : std::make_unique<MARC_DEFAULT_GEOM_CORR_STRATEGY>())
    , photometric_correction_(
        std::make_unique<MARC_DEFAULT_PHOTO_CORR_STRATEGY>())
    , interpolation_strategy_(
        std::make_unique<MARC_DEFAULT_INTERPOLATION_STRATEGY>())
    , sky_mask_      (samples * lines, false) // Enable sky removal.
    , nibble_left_   (0)
    , nibble_right_  (0)
    , nibble_top_    (0)
    , nibble_bottom_ (0)
    , geometry_      (nullptr)
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

MaRC::PhotoImage::~PhotoImage (void)
{
}

bool
MaRC::PhotoImage::geometric_correction(
    std::unique_ptr<GeometricCorrection> strategy)
{
    if (strategy) {
        this->geometric_correction_ = std::move(strategy);

        return true;  // Success
    } else {
        std::cerr
            << "ERROR: Null geometric correction strategy pointer.\n";

        return false;  // Failure
    }
}

bool
MaRC::PhotoImage::photometric_correction(
    std::unique_ptr<PhotometricCorrection> strategy)
{
    if (strategy) {
        this->photometric_correction_ = std::move(strategy);

        return true;  // Success
    } else {
        std::cerr
            << "ERROR: Null photometric correction strategy pointer.\n";

        return false;  // Failure
    }
}

void
MaRC::PhotoImage::finalize_setup()
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

void MaRC::PhotoImage::remove_sky(bool r)
{
    if (r)
        this->sky_mask_.resize(this->samples_ * this->lines_, false);
    else
        this->sky_mask_.clear();
}

void
MaRC::PhotoImage::remove_sky()
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
            coord[0] = x;
            coord[1] = 0;
            coord[2] = -z; // Negative since line numbers increase top to
                           // bottom (?)
            //      coord[2] = z;

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
MaRC::PhotoImage::nibble(std::size_t n)
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

      throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_left(std::size_t n)
{
    if (n < (this->samples_ - this->nibble_right_))
        this->nibble_left_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble left value (" << n << ")";

        throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_right(std::size_t n)
{
    if (n < (this->samples_ - this->nibble_left_))
        this->nibble_right_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble right value (" << n << ")";

        throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_top(std::size_t n)
{
    if (n < (this->lines_ - this->nibble_bottom_))
        this->nibble_top_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble top value (" << n << ")";

        throw std::invalid_argument(s.str ());
    }
}

void
MaRC::PhotoImage::nibble_bottom(std::size_t n)
{
    if (n < (this->lines_ - this->nibble_top_))
        this->nibble_bottom_ = n;
    else {
        std::ostringstream s;
        s << "Invalid nibble bottom value (" << n << ")";

        throw std::invalid_argument (s.str ());
    }
}

bool
MaRC::PhotoImage::read_data(double lat, double lon, double & data) const
{
    std::size_t weight = 1;  // Unused.

    static const bool scan = false; // Do not scan for data weight.

    return this->read_data(lat, lon, data, weight, scan);
}

bool
MaRC::PhotoImage::read_data(double lat,
                            double lon,
                            double & data,
                            std::size_t & weight,
                            bool scan) const
{
    /**
     * @todo Validate @a lat and @a lon.
     */

    double x = 0, z = 0;

    if (!this->latlon2pix(lat, lon, x, z))
        return false;

    // Convert from object space to image space.
    this->geometric_correction_->object_to_image(z, x);

    assert(x >= 0 && z >= 0);

    // x and z are 'pixel coordinates'.  In 'pixel coordinates', the
    // half-open interval [0,1) is inside pixel 0, [1,2) is inside
    // pixel 1, etc.
    std::size_t const i = static_cast<std::size_t>(std::floor(x));
    std::size_t const k = static_cast<std::size_t>(std::floor(z));
    std::size_t const index = k * this->samples_ + i;

    // e.g., if (i < 0 || i >= samples_ || k < 0 || k >= lines_)
    // The following assumes that line numbers increase downward.
    // CHECK ME!
    if (i < this->nibble_left_   || i >= this->samples_ - this->nibble_right_
        || k < this->nibble_top_ || k >= this->lines_ - this->nibble_bottom_
        || (!this->sky_mask_.empty() && !this->sky_mask_[index]))
        return false;

    data = this->image_[index];

    if (!this->interpolation_strategy_->interpolate(this->image_.data(),
                                                    x,
                                                    z,
                                                    data)
        || this->photometric_correction_->correct(*this->body_,
                                                  this->sub_observ_lat_,
                                                  this->sub_observ_lon_,
                                                  this->sub_solar_lat_,
                                                  this->sub_solar_lon_,
                                                  lat,
                                                  lon,
                                                  this->range_,
                                                  data) != 0
        || std::isnan(data)) {
        return false;
    }

    // Scan across image for "off-planet/image" pixels and compute
    // averaging weights.
    //
    // Note that a weight is computed regardless of whether or not sky
    // removal is enabled.
    if (scan) {
        std::size_t & shortest_distance = weight;

        // Give less weight to pixels close to an edge of the image.
        //
        // No need to include nibble values in this calculation since
        // we're guaranteed to be within the non-nibbled image area due
        // to the nibble value check earlier in this method.
        //
        // For most purposes, this quickly computed weight should be
        // sufficient.  If the image has gaps, determining weights
        // through the sky mask scanning code below may be a better
        // choice in terms of quality.
        shortest_distance =
            std::min(i,
                     std::min(this->samples_ - i,
                              std::min(k,
                                       this->lines_ - k)));

        // Scan across image for "off-body/image" pixels, giving less
        // weight to those on the body closer to the sky.
        if (!this->sky_mask_.empty()) {
            auto const sky_iter = this->sky_mask_.cbegin();

            // Scan across samples.

            // Search from nibble_left to i.
            //   Beginning of line: index - i
            auto begin = sky_iter + (index - i + this->nibble_left_);
            auto end   = sky_iter + (index + 1);  // one past "i"

            auto result = std::find(begin, end, true);

            assert(begin < result);

            shortest_distance =
                std::min(
                    static_cast<std::size_t>(std::distance(begin, result)),
                    shortest_distance);

            // Search from i to nibble_right.
            //   "index" offsets to "i".
            begin = sky_iter + index;
            end   = sky_iter
                + (index + (this->samples_ - this->nibble_right_));

            result = std::find(begin, end, true);

            assert(begin < result);

            shortest_distance =
                std::min(
                    static_cast<std::size_t>(std::distance(begin, result)),
                    shortest_distance);

            // -----------------------------------------------------

            // Scan across lines.  Line numbers increase from top to
            // bottom.

            std::size_t kk;

            // Search from nibble_top to k.
            for (kk = this->nibble_top_;
                 kk <= k && this->sky_mask_[kk * this->samples_ + i];
                 ++kk)
                ; // Nothing

            shortest_distance = std::min(kk - k, shortest_distance);

            // Search from k to nibble_bottom.
            std::size_t const kend = this->lines_ - this->nibble_bottom_;

            for (kk = k;
                 kk < kend && this->sky_mask_[kk * this->samples_ + i];
                 ++kk)
                ; // Nothing

            shortest_distance = std::min(kk - k, shortest_distance);
        }
    }

    return true;  // Success
}
