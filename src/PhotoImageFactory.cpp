/**
 * @file PhotoImageFactory.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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
#include "FITS_memory.h"

#include "MaRC/PhotoImage.h"

// Geometric correction strategies
#include "MaRC/GLLGeometricCorrection.h"

// Photometric correction strategies
//#include "MaRC/MinnaertPhotometricCorrection.h"

// Interpolation strategies
#include "MaRC/BilinearInterpolation.h"

#include <fitsio.h>

#include <limits>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <cmath>
#include <cassert>


MaRC::PhotoImageFactory::PhotoImageFactory(
    char const * filename)
    : filename_(filename)
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

    fitsfile * fptr = nullptr;
    static constexpr int mode = READONLY;
    int status = 0;  // CFITSIO requires initialization of status
                     // variable.

    (void) fits_open_image(&fptr,
                           this->filename_.c_str(),
                           mode,
                           &status);

    if (status != 0) {
        // Report any errors before creating the map since map
        // creation can be time consuming.
        fits_report_error(stderr, status);

        std::ostringstream s;
        s << "Unable to open image \"" << this->filename_ << "\"";

        throw std::invalid_argument(s.str());
    }

    FITS::file_unique_ptr safe_fptr(fptr);

    int naxis = 0;
    int bitpix = 0;
    static constexpr int MAXDIM = 2;  // CFITSIO wants int.
    long naxes[MAXDIM] = { 0 };

    (void) fits_get_img_param(fptr,
                              MAXDIM,
                              &bitpix,
                              &naxis,
                              naxes,
                              &status);

    // Get the image data unit name (FITS standard BUNIT).
    char bunit[FLEN_VALUE];
    char bunit_comment[FLEN_COMMENT];

    fits_read_key_str(fptr, "BUNIT", bunit, bunit_comment, &status);
    if (status == 0) {
        /**
         * @todo Do something useful with BUNIT.
         */
    } else if (status != KEY_NO_EXIST) {
        throw std::logic_error("Problem reading FITS BUNIT value.");
    } else {
        status = 0;
    }

    LONGLONG const nelements =
        static_cast<LONGLONG>(naxes[0]) * naxes[1];

    // Note that we're only reading a 2-dimensional image above.
    std::vector<double> img(nelements,
                            std::numeric_limits<double>::signaling_NaN());

    /**
     * First pixel to be read.
     *
     * @attention First pixel in CFITSIO is { 1, 1 } not { 0, 0 }.
     */
    long fpixel[MAXDIM] = { 1, 1 };

    // For integer typed FITS images with a BLANK value, set the
    // "blank" value in our floating point converted copy of the image
    // to NaN.
    double nulval = std::numeric_limits<double>::signaling_NaN();
    int anynul = 0;  // Unused

    (void) fits_read_pix(fptr,
                         TDOUBLE, // Array of type "double".
                         fpixel,
                         nelements,
                         &nulval,  // "Blank" value in our image.
                         img.data(),
                         &anynul,  // Were any blank values found?
                         &status);

    fits_report_error(stderr, status);

    safe_fptr.reset();  // Done reading the FITS file.

    // Perform flat fielding if a flat field file was provided.
    int const result = flat_field_correct(naxes, img);

    if (result != 0) {
        // Report any errors before creating the map since map
        // creation can be time consuming.
        return nullptr;
    }

    std::size_t const samples = static_cast<std::size_t>(naxes[0]);
    std::size_t const lines   = static_cast<std::size_t>(naxes[1]);

    // Invert image if desired.
    if (this->invert_h_)
        this->invert_h(img, samples, lines);

    if (this->invert_v_)
        this->invert_v(img, samples, lines);

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
MaRC::PhotoImageFactory::invert_h(std::vector<double> & image,
                                  std::size_t samples,
                                  std::size_t lines)
{
    assert(image.size() == samples * lines);

    // Invert image from left to right.
    auto begin = image.begin();
    for (std::size_t line = 0; line < lines; ++line) {
        auto const end = begin + samples;
        std::reverse(begin, end);
        begin = end;
    }
}

void
MaRC::PhotoImageFactory::invert_v(std::vector<double> & image,
                                  std::size_t samples,
                                  std::size_t lines)
{
    assert(image.size() == samples * lines);

    // Invert image from top to bottom.
    std::size_t const middle = lines / 2;

    for (std::size_t line = 0; line < middle; ++line) {
        // Line from the one end.
        auto const top_begin = image.begin() + line * samples;
        auto const top_end   = top_begin + samples;

        // Line from the other end.
        std::size_t const offset = (lines - line - 1) * samples;
        auto const bottom_begin = image.begin() + offset;

        // Swap the lines.
        std::swap_ranges(top_begin, top_end, bottom_begin);
    }
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

int
MaRC::PhotoImageFactory::flat_field_correct(
    long const naxes[2],
    std::vector<double> & img) const
{
    std::vector<double> f_img;

    int status = 0;

    if (!this->flat_field_.empty()) {
        fitsfile * fptr = nullptr;
        static constexpr int mode = READONLY;

        if (fits_open_image(&fptr,
                            this->flat_field_.c_str(),
                            mode,
                            &status) != 0) {
            fits_report_error(stderr, status);

            std::ostringstream s;
            s << "Unable to open flat field image \""
              << this->filename_ << "\"";

            throw std::invalid_argument(s.str());
        }

        FITS::file_unique_ptr const safe_fptr(fptr);

        int f_bitpix = 0;
        int f_naxis = 0;
        static constexpr int MAXDIM = 2;  // CFITSIO wants int.
        long f_naxes[MAXDIM] = { 0 };

        if (fits_get_img_param(fptr,
                               MAXDIM,
                               &f_bitpix,
                               &f_naxis,
                               f_naxes,
                               &status) != 0) {
            fits_report_error(stderr, status);

            return status;
        }

        // Verify flat field image is same size as source photo
        // image.
        if (f_naxes[0] != naxes[0] && f_naxes[1] != naxes[1]) {
            std::ostringstream s;
            s << "Mismatched source (" << naxes[0] << "x" << naxes[1] << ")"
              << " and flat field image "
              << "(" << f_naxes[0] << "x" << f_naxes[1] << ")"
              << "dimensions";

            throw std::runtime_error(s.str());
        }

        // Note that we're only reading a 2-dimensional image
        // above.
        LONGLONG const nelements =
            static_cast<LONGLONG>(f_naxes[0]) * f_naxes[1];
        f_img.resize(nelements);

        long fpixel[MAXDIM] = { 1, 1 };
        double nulval = std::numeric_limits<double>::signaling_NaN();
        int anynul;  // Unused

        (void) fits_read_pix(fptr,
                             TDOUBLE, // Array of type "double".
                             fpixel,
                             nelements,
                             &nulval,
                             f_img.data(),
                             &anynul,
                             &status);

        fits_report_error(stderr, status);
    }

    if (status != 0) {
        char fits_error[FLEN_STATUS] = { 0 };
        fits_get_errstatus(status, fits_error);

        throw std::runtime_error(fits_error);
    }

    // Perform flat fielding if desired.
    if (!f_img.empty()) {
        std::size_t const size = img.size();
        assert(size == f_img.size());

        for (std::size_t i = 0; i < size; ++i)
            img[i] -= f_img[i];
    }

    return 0;
}
