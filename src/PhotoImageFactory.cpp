/**
 * @file PhotoImageFactory.cpp
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#include "PhotoImageFactory.h"
#include "FITS_file.h"

#include "marc/PhotoImage.h"

// Geometric correction strategies
#include "marc/GLLGeometricCorrection.h"

// Photometric correction strategies
//#include "marc/MinnaertPhotometricCorrection.h"

// Interpolation strategies
#include "marc/BilinearInterpolation.h"

#include "marc/utility.h"

#include <limits>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <type_traits>
#include <cmath>
#include <cassert>


MaRC::PhotoImageFactory::PhotoImageFactory(char const * filename)
    : ImageFactory()
    , filename_(filename)
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

std::unique_ptr<MaRC::SourceImage>
MaRC::PhotoImageFactory::make(scale_offset_functor /* calc_so */)
{
    if (!this->config_ || !this->geometry_)
        return nullptr;  // not set or make() already called!

    std::vector<double> img;
    std::size_t samples = 0;
    std::size_t lines   = 0;

    {
        FITS::input_file f(this->filename_.c_str());

        // Get the image data unit name (FITS standard BUNIT).
        /**
         * @todo Make unit related members consistent.  Right
         *       now we have a mix of char const * and std::string
         *       typed unit members.
         */
        std::string const & bunit = f.bunit();
        this->config_->unit(bunit);

        f.read(img, samples, lines);

        /**
         * @todo Get the minimum (@c DATAMIN) and maximum (@c DATAMAX)
         *       physical data values if available in the source image
         *       FITS file.
         */
    }

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
