/**
 * @file PhotoImage.cpp
 *
 * Copyright (C) 1998-1999, 2003-2005, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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
#include <sstream>
#include <cassert>


namespace
{
    /// Create body mask vector for use in "sky removal".
    auto body_mask(std::size_t samples,
                   std::size_t lines,
                   MaRC::PhotoImageParameters const * config,
                   MaRC::ViewingGeometry const * geometry)
    {
        if (!config || !geometry) {
            throw std::invalid_argument(
                "Null PhotoImage parameters or geometry.");
        }

        if (config->remove_sky()) {
            return geometry->body_mask(samples, lines);
        }

        return std::vector<bool>();
    }
}

MaRC::PhotoImage::PhotoImage(std::vector<double> && image,
                             std::size_t samples,
                             std::size_t lines,
                             std::unique_ptr<PhotoImageParameters> config,
                             std::unique_ptr<ViewingGeometry> geometry)
    : SourceImage()
    , image_    (std::move(image))
    , samples_  (samples)
    , lines_    (lines)
    , config_   (std::move(config))
    , geometry_ (std::move(geometry))
    , body_mask_(body_mask(samples,
                           lines,
                           config_.get(),
                           geometry_.get()))
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

    /**
     * @note Null config and geometry parameter checks are done in the
     *       call to body_mask() in the anonymous namespace above.
     */

    // All necessary image values and attributes should be set by now!

    /**
     * @todo This configuration validation approach seems rather
     *       brittle.  Revisit.
     */
    this->config_->validate_parameters(samples, lines);
}

bool
MaRC::PhotoImage::read_data(double lat, double lon, double & data) const
{
    std::size_t weight = 1;  // Unused.

    static constexpr bool scan = false; // Do not scan for data weight.

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

    if (!this->geometry_->latlon2pix(lat, lon, x, z) || x < 0 || z < 0)
        return false;

    // x and z are 'pixel coordinates'.  In 'pixel coordinates', the
    // half-open interval [0,1) is inside pixel 0, [1,2) is inside
    // pixel 1, etc.
    auto const i = static_cast<std::size_t>(std::floor(x));
    auto const k = static_cast<std::size_t>(std::floor(z));
    auto const index = k * this->samples_ + i;

    auto const & config = this->config_;

    /**
     * The following assumes that line numbers increase downward.
     *
     * @todo Verify that this "nibbling" works as described.
     *
     * Consider NaN data points invalid, i.e. "off the body".
     * No need to continue beyond this point.
     *
     * @todo Check for a user-specified "blank" value as
     *       well.
     */
    if (i < config->nibble_left()
        || i >= this->samples_ - config->nibble_right()
        || k < config->nibble_top()
        || k >= this->lines_ - config->nibble_bottom()
        || (!this->body_mask_.empty() && !this->body_mask_[index])
        || std::isnan(data))
        return false;

    data = this->image_[index];

    if (!config->interpolation_strategy()->interpolate(
            this->image_.data(),
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

char const *
MaRC::PhotoImage::unit() const
{
    return this->config_->unit();
}

void
MaRC::PhotoImage::data_weight(std::size_t i,
                              std::size_t k,
                              std::size_t & weight) const
{
    /**
     * @note This method assumes at "i" is in the range
     *       [nibble_left, samples - nibble_right), "k" is in the
     *       range [nibble_top, lines - nibble_bottom).
     *
     * @todo Reduce redundant code in this method.
     */

    auto & shortest_distance = weight;

    // Give less weight to pixels close to an edge of the image.
    //
    // No need to include nibble values in this calculation since
    // we're guaranteed to be within the non-nibbled image area due to
    // the earlier nibble value check.
    //
    // For most purposes, this quickly computed weight should be
    // sufficient.  If the image has gaps, determining weights through
    // the body mask scanning code below may be a better choice in
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
    if (this->body_mask_.empty())
        return;

    auto const body_iter = this->body_mask_.cbegin();

    auto const & config = this->config_;

    auto const index = k * this->samples_ + i;

    // Scan across samples.

    // Search from nibble_left to i.
    //   Beginning of line: index - i
    auto begin = body_iter + (index - i + config->nibble_left());
    auto end   = body_iter + (index + 1);  // one past column "i"

    auto result = std::find(begin, end, true);

    assert(begin <= result);

    if (result != end)
        shortest_distance =
            std::min(
                static_cast<
                std::remove_reference<decltype(shortest_distance)>::type>(
                    std::distance(begin, result)),
                shortest_distance);

    // Search from i to nibble_right.
    //   "index" offsets to "i".
    begin = body_iter + index;
    end   =
        body_iter + (index + (this->samples_ - config->nibble_right()));

    result = std::find(begin, end, true);

    assert(begin <= result);

    if (result != end)
        shortest_distance =
            std::min(
                static_cast<
                std::remove_reference<decltype(shortest_distance)>::type>(
                    std::distance(begin, result)),
                shortest_distance);

    // -----------------------------------------------------

    // Scan across lines.  Line numbers increase from top to bottom.

    // Search from nibble_top to k.
    auto first = config->nibble_top();
    auto last  = k;
    auto line  = first;

    for (;
         line < last && this->body_mask_[line * this->samples_ + i];
         ++line)
        ; // Nothing

    if (line != last)
        shortest_distance = std::min(line - first, shortest_distance);

    // Search from k to nibble_bottom.
    first = k;
    last  = this->lines_ - config->nibble_bottom();

    for (line = first;
         line < last && this->body_mask_[line * this->samples_ + i];
         ++line)
        ; // Nothing

    if (line != last)
        shortest_distance = std::min(line - first, shortest_distance);
}
