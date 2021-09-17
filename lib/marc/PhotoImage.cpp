/**
 * @file PhotoImage.cpp
 *
 * Copyright (C) 1998-1999, 2003-2005, 2017, 2019, 2021  Ossama Othman
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

#include "config.h"  // For NDEBUG and FMT_HEADER_ONLY.

#include <fmt/core.h>

#include <stdexcept>
#include <cassert>


namespace
{
    /// Create body mask vector for use in "sky removal".
    auto make_body_mask(std::size_t samples,
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
    , left_     (config->nibble_left())
    , right_    (samples - config->nibble_right())
    , top_      (config->nibble_top())
    , bottom_   (lines - config->nibble_bottom())
    , config_   (std::move(config))
    , geometry_ (std::move(geometry))
    , body_mask_(make_body_mask(samples,
                                lines,
                                config_.get(),
                                geometry_.get()))
{
    if (samples < 2 || lines < 2) {
        // Why would there ever be a one pixel source image?
        throw std::invalid_argument(
            fmt::format("Source image samples ({}) and lines ({}) "
                        "must both be greater than one.",
                        samples,
                        lines));
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
    if (   i <  this->left_
        || i >= this->right_
        || k <  this->top_
        || k >= this->bottom_
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

void
MaRC::PhotoImage::scan_samples(std::size_t line,
                               std::size_t left,
                               std::size_t right,
                               std::size_t & weight) const
{
    // Scan across samples on given line.

    auto const offset    = line * this->samples_;
    auto       body_iter = std::cbegin(this->body_mask_) + offset;

    // Search the half-open interval [left, right).
    auto const begin  = body_iter + left;
    auto const end    = body_iter + right;
    auto const result = std::find(begin, end, true);

    assert(begin <= result);

    // The weight is the shortest distance.
    if (result != end)
        weight =
            std::min(
                static_cast<
                std::remove_reference<decltype(weight)>::type>(
                    std::distance(begin, result)),
                weight);

}

void
MaRC::PhotoImage::scan_lines(std::size_t i,
                             std::size_t top,
                             std::size_t bottom,
                             std::size_t & weight) const
{
    // Search the half-open interval [top, bottom).
    auto body_iter =
        std::cbegin(this->body_mask_) + (top * this->samples_) + i;

    auto const first = top;
    auto const last  = bottom;
    auto       line = first;

    for (; line < last && *body_iter; ++line)
        std::advance(body_iter, this->samples_); // Sample i in next
                                                 // line in mask.

    if (line != last)
        weight = std::min(line - first, weight);
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
     */

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

    // The weight is the shortest distance.
    weight =
        std::min(i,
                 std::min(this->samples_ - i,
                          std::min(k,
                                   this->lines_ - k)));

    // Scan across image for "off-body/image" pixels, giving less
    // weight to those on the body closer to the sky.
    if (this->body_mask_.empty())
        return;

    // -----------------------------------------------------

    // Scan across the half-open interval [left, i) on line k.
    this->scan_samples(k, this->left_, i, weight);

    // Scan across the half-open interval [i, right) on line k.
    this->scan_samples(k, i, this->right_, weight);

    // -----------------------------------------------------

    // Line numbers increase from top to bottom.

    // Scan across the half-open interval [top, k) on sample i.
    this->scan_lines(i, this->top_, k, weight);

    // Scan across the half-open interval [k, bottom) on sample i.
    this->scan_lines(i, k, this->bottom_, weight);
}
