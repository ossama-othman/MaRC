/**
 * @file PhotoImageFactory.cpp
 *
 * Copyright (C) 2004, 2017-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "PhotoImageFactory.h"
#include "map_parameters.h"

#include "marc/PhotoImage.h"

// Geometric correction strategies
#include "marc/GLLGeometricCorrection.h"

// Photometric correction strategies
//#include "marc/MinnaertPhotometricCorrection.h"

// Interpolation strategies
#include "marc/BilinearInterpolation.h"

#include "marc/utility.h"
#include "marc/config.h"  // For NDEBUG.

#include <limits>
#include <stdexcept>
#include <memory>
#include <type_traits>
#include <cmath>
#include <cassert>

#include <fmt/format.h>


MaRC::PhotoImageFactory::PhotoImageFactory(char const * filename)
    : SourceImageFactory()
    , file_(filename)
    , flat_field_()
    , geometric_correction_(false)
    // , photometric_correction_(false)
    , interpolate_(false)
    , invert_v_(false)
    , invert_h_(false)
    , config_()
    , geometry_()
{
}

/**
 * @brief Set map parameter from photo/image %FITS file.
 *
 * @param parameter Name of map parameter to be set.
 */
#define MARC_SET_PARAM(parameter) p.parameter(this->file_.parameter())

bool
MaRC::PhotoImageFactory::populate_parameters(
    MaRC::map_parameters & p) const
{
    MARC_SET_PARAM(author);
    MARC_SET_PARAM(bitpix);
#if __cplusplus < 201703L
    /*
      Work around lack of a converting constructor in the
      C++14 based MaRC::optional<> implementation.
    */
    auto const fits_blank = this->file_.blank();
    if (fits_blank.has_value()) {
        MaRC::blank_type::value_type const blank = *fits_blank;
        p.blank(blank);
    }
#else
    MARC_SET_PARAM(blank);
#endif  // __cplusplus < 201703L
    MARC_SET_PARAM(bunit);

    /**
     * @note The %FITS @c DATAMIN and @c DATAMAX values are not set in
     *       the map parameters.  Instead they are set in this image
     *       factory so that they may be used when plotting the image
     *       to the map.  The %FITS @c DATAMIN and @c DATAMAX values
     *       corresponding to data that was actually plotted will be
     *       automatically written to map %FITS once mapping is done.
     */
    // MARC_SET_PARAM(datamin);
    // MARC_SET_PARAM(datamax);

    MARC_SET_PARAM(equinox);
    // MARC_SET_PARAM(date_obs);
    MARC_SET_PARAM(instrument);
    MARC_SET_PARAM(object);
    MARC_SET_PARAM(observer);
    MARC_SET_PARAM(origin);
    MARC_SET_PARAM(reference);
    MARC_SET_PARAM(telescope);

    return true;
}

std::unique_ptr<MaRC::SourceImage>
MaRC::PhotoImageFactory::make(scale_offset_functor /* calc_so */)
{
    if (!this->config_ || !this->geometry_)
        return nullptr;  // not set or make() already called!

    std::vector<double> img;
    std::size_t samples = 0;
    std::size_t lines   = 0;

    this->file_.read(img, samples, lines);

    // Perform flat fielding if a flat field file was provided.
    this->flat_field_correct(img, samples, lines);

    // Invert image if desired.
    if (this->invert_h_)
        MaRC::invert_samples(img, samples, lines);

    if (this->invert_v_)
        MaRC::invert_lines(img, samples, lines);

    if (this->geometric_correction_) {
        this->geometry_->geometric_correction(
            std::make_unique<MaRC::GLLGeometricCorrection>(samples));
    }

    if (this->interpolate_) {
        this->config_->interpolation_strategy(
            std::make_unique<BilinearInterpolation>(
                samples,
                lines,
                this->config_->nibble_left(),
                this->config_->nibble_right(),
                this->config_->nibble_top(),
                this->config_->nibble_bottom()));
    }

    /**
     * @todo Finalizing viewing geometry setup like this is
     *       brittle. Revisit.
     */
    this->geometry_->finalize_setup(samples, lines);

    // Set physical data extrema if not previously set.
    auto const & datamin = this->file_.datamin();
    auto const & datamax = this->file_.datamax();

    if (datamin)
        this->minimum(*datamin);
    if (datamax)
        this->maximum(*datamax);

    return
        std::make_unique<MaRC::PhotoImage>(std::move(img),
                                           samples,
                                           lines,
                                           std::move(this->config_),
                                           std::move(this->geometry_));
}

void
MaRC::PhotoImageFactory::flat_field(char const * name)
{
    this->flat_field_ = name;
}

void
MaRC::PhotoImageFactory::geometric_correction(bool enable)
{
    this->geometric_correction_ = enable;
}

void
MaRC::PhotoImageFactory::interpolate(bool enable)
{
    this->interpolate_ = enable;
}

void
MaRC::PhotoImageFactory::invert(bool vertical, bool horizontal)
{
    this->invert_v_ = vertical;
    this->invert_h_ = horizontal;
}

void
MaRC::PhotoImageFactory::photo_config(
    std::unique_ptr<PhotoImageParameters> config)
{
    this->config_ = std::move(config);
}

void
MaRC::PhotoImageFactory::viewing_geometry(
    std::unique_ptr<ViewingGeometry> geometry)
{
    this->geometry_ = std::move(geometry);
}

void
MaRC::PhotoImageFactory::flat_field_correct(std::vector<double> & img,
                                            std::size_t samples,
                                            std::size_t lines) const
{
    if (this->flat_field_.empty())
        return;

    std::vector<double> f_img;

    {
        FITS::input_file f(this->flat_field_.c_str());

        // Verify flat field image is same size as source photo
        // image.
        std::size_t f_samples = 0;
        std::size_t f_lines   = 0;

        f.read(f_img, f_samples, f_lines);

        if (f_samples != samples || f_lines != lines) {
            auto s =
                fmt::format("Mismatched source ({}x{}) and "
                            "flat field image ({}x{}) dimensions.",
                            samples,
                            lines,
                            f_samples,
                            f_lines);

            throw std::runtime_error(s);
        }
    }

    // Perform flat fielding.
    std::size_t const size = img.size();
    assert(size == f_img.size());

    for (std::size_t i = 0; i < size; ++i)
        img[i] -= f_img[i];
}
