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
#include "PhotoImageParameters.h"
#include "ViewingGeometry.h"
#include "OblateSpheroid.h"

#include "GeometricCorrection.h"
#include "InterpolationStrategy.h"
#include "PhotometricCorrection.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cassert>


MaRC::PhotoImage::PhotoImage(std::vector<double> && image,
                             std::size_t samples,
                             std::size_t lines,
                             std::unique_ptr<PhotoImageParameters> config,
                             std::unique_ptr<ViewingGeometry> geometry)
    : SourceImage()
    , image_(std::move(image))
    , samples_(samples)
    , lines_(lines)
    , config_(std::move(config))
    , geometry_(std::move(geometry))
{
    // All necessary image values and attributes should be set by now!

    // Scan across and determine where points lie off of the body, i.e.
    // remove the sky from the image.  The image will not actually be
    // modified.
    this->remove_sky(); // Remove sky from source image
}

MaRC::PhotoImage::~PhotoImage()
{
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

    if (!this->geometry_->latlon2pix(lat, lon, x, z))
        return false;

    auto const & config = this->config_;

    // Convert from object space to image space.
    config->geometric_correction()->object_to_image(z, x);

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
    if (i < config->nibble_left()
        || i >= this->samples_ - config->nibble_right()
        || k < config->nibble_top()
        || k >= this->lines_ - config->nibble_bottom()
        || (!this->sky_mask_.empty() && !this->sky_mask_[index]))
        return false;

    data = this->image_[index];

    if (!config->interpolation_strategy()->interpolate(
            this->image_.data(),
            this->samples_,
            this->lines_,
            x,
            z,
            data)
        || !config->photometric_correction()->correct(*this->geometry_,
                                                      data)
        || std::isnan(data)) {
        return false;
    }

    // Scan across image for "off-planet/image" pixels and compute
    // data weight.
    if (scan)
        this->data_weight(i, k, weight);

    return true;  // Success
}

void
MaRC::PhotoImage::data_weight(std::size_t i,
                              std::size_t k,
                              std::size_t & weight) const
{
    std::size_t & shortest_distance = weight;

    // Give less weight to pixels close to an edge of the image.
    //
    // No need to include nibble values in this calculation since
    // we're guaranteed to be within the non-nibbled image area due to
    // the earlier nibble value check.
    //
    // For most purposes, this quickly computed weight should be
    // sufficient.  If the image has gaps, determining weights through
    // the sky mask scanning code below may be a better choice in
    // terms of quality.
    //
    // Note that a weight is computed regardless of whether or not sky
    // removal is enabled.

    shortest_distance =
        std::min(i,
                 std::min(this->samples_ - i,
                          std::min(k,
                                   this->lines_ - k)));

    // Scan across image for "off-body/image" pixels, giving less
    // weight to those on the body closer to the sky.
    if (this->sky_mask_.empty())
        return;

    auto const sky_iter = this->sky_mask_.cbegin();

    auto const & config = this->config_;

    // Scan across samples.

    // Search from nibble_left to i.
    //   Beginning of line: index - i
    auto begin = sky_iter + (index - i + config->nibble_left());
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
    end   =
        sky_iter + (index + (this->samples_ - config->nibble_right()));

    result = std::find(begin, end, true);

    assert(begin < result);

    shortest_distance =
        std::min(
            static_cast<std::size_t>(std::distance(begin, result)),
            shortest_distance);

    // -----------------------------------------------------

    // Scan across lines.  Line numbers increase from top to bottom.

    std::size_t kk;

    // Search from nibble_top to k.
    for (kk = config->nibble_top();
         kk <= k && this->sky_mask_[kk * this->samples_ + i];
         ++kk)
        ; // Nothing

    shortest_distance = std::min(kk - k, shortest_distance);

    // Search from k to nibble_bottom.
    std::size_t const kend = this->lines_ - config->nibble_bottom();

    for (kk = k;
         kk < kend && this->sky_mask_[kk * this->samples_ + i];
         ++kk)
        ; // Nothing

    shortest_distance = std::min(kk - k, shortest_distance);
}
