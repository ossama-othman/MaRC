/**
 * @file PhotoImageFactory.cpp
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "PhotoImageFactory.h"
#include "MapParameters.h"

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
#include <sstream>
#include <memory>
#include <type_traits>
#include <cmath>
#include <cassert>


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

bool
MaRC::PhotoImageFactory::populate_parameters(MaRC::MapParameters &p) const
{
#define MARC_SET_PARAM(param) p.param(this->file_.param())

    MARC_SET_PARAM(author);
    MARC_SET_PARAM(bitpix);
    MARC_SET_PARAM(blank);
    MARC_SET_PARAM(bunit);
    MARC_SET_PARAM(datamax);
    MARC_SET_PARAM(datamin);
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
            std::ostringstream s;
            s << "Mismatched source ("
              << samples << "x" << lines
              << ") and flat field image ("
              << f_samples << "x" << f_lines
              << ") dimensions";

            throw std::runtime_error(s.str());
        }
    }

    // Perform flat fielding.
    std::size_t const size = img.size();
    assert(size == f_img.size());

    for (std::size_t i = 0; i < size; ++i)
        img[i] -= f_img[i];
}
