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
#include "MaRC/GLLGeometricCorrection.h"
#include "MaRC/NullGeometricCorrection.h"

#include <fitsio.h>

#include <stdexcept>
#include <sstream>
#include <memory>
#include <cmath>
#include <cassert>


MaRC::PhotoImageFactory::PhotoImageFactory(
    char const * filename,
    std::shared_ptr<OblateSpheroid> body,
    std::unique_ptr<ViewingGeometry> geometry)
    : filename_(filename)
    , flat_field_()
    , body_(body)
    , geometry_(std::move(geometry))
    , geometric_correction_(false)
    , photometric_correction_(false)
    , nibble_left_(0)
    , nibble_right_(0)
    , nibble_top_(0)
    , nibble_bottom_(0)
    , remove_sky_(true)
    , interpolate_(false)
    , invert_v_(false)
    , invert_h_(false)
{
}

std::unique_ptr<MaRC::SourceImage>
MaRC::PhotoImageFactory::make(scale_offset_functor /* calc_so */)
{
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
        return std::unique_ptr<SourceImage>();
    }

    std::size_t const samples = static_cast<std::size_t>(naxes[0]);
    std::size_t const lines   = static_cast<std::size_t>(naxes[1]);

    // Invert image if desired.
    if (this->invert_h_)
        this->invert_h(img, samples, lines);

    if (this->invert_v_)
        this->invert_v(img, samples, lines);

    std::unique_ptr<MaRC::GeometricCorrection> gc;
    if (this->geometric_correction_) {
        gc =
            std::make_unique<MaRC::GLLGeometricCorrection>(samples);
    } else {
        gc = std::make_unique<MaRC::NullGeometricCorrection>();
    }

    std::unique_ptr<PhotoImage> photo(
        std::make_unique<MaRC::PhotoImage>(
            this->body_,
            std::move(img),
            samples,
            lines,
            std::move(gc)));

    photo->nibble_left  (this->nibble_left_);
    photo->nibble_right (this->nibble_right_);
    photo->nibble_top   (this->nibble_top_);
    photo->nibble_bottom(this->nibble_bottom_);

    if (this->km_per_pixel_ > 0)
        photo->km_per_pixel(this->km_per_pixel_);

    if (this->arcsec_per_pixel_ > 0)
        photo->arcsec_per_pixel(this->arcsec_per_pixel_);

    if (this->focal_length_ > 0)
        photo->focal_length(this->focal_length_);

    if (this->scale_ > 0)
        photo->scale(this->scale_);

    if (!std::isnan(this->OA_s_))
        photo->optical_axis_sample(this->OA_s_);

    if (!std::isnan(this->OA_l_))
        photo->optical_axis_line(this->OA_l_);

    if (!std::isnan(this->sample_center_))
        photo->body_center_sample(this->sample_center_);

    if (!std::isnan(this->line_center_))
        photo->body_center_line(this->line_center_);

    if (!std::isnan(this->lat_at_center_))
        photo->lat_at_center(this->lat_at_center_);

    if (!std::isnan(this->lon_at_center_))
        photo->lon_at_center(this->lon_at_center_);

    if (this->emi_ang_limit_ > 0)
        photo->emi_ang_limit(this->emi_ang_limit_);

    photo->sub_observ(this->sub_observ_lat_, this->sub_observ_lon_);
    photo->position_angle(this->position_angle_);
    photo->sub_solar(this->sub_solar_lat_, this->sub_solar_lon_);
    photo->range(this->range_);

    photo->remove_sky(this->remove_sky_);
    photo->interpolate(this->interpolate_);
    photo->use_terminator(this->use_terminator_);

    photo->finalize_setup();

    return std::move(photo);
}

void
MaRC::PhotoImageFactory::flat_field(char const * name)
{
    this->flat_field_ = name;
}

void
MaRC::PhotoImageFactory::nibbling(std::size_t left,
                                  std::size_t right,
                                  std::size_t top,
                                  std::size_t bottom)
{
    this->nibble_left_   = left;
    this->nibble_right_  = right;
    this->nibble_top_    = top;
    this->nibble_bottom_ = bottom;
}

void
MaRC::PhotoImageFactory::invert(bool vertical, bool horizontal)
{
    this->invert_v_ = vertical;
    this->invert_h_ = horizontal;
}

void
MaRC::PhotoImageFactory::interpolate(bool enable)
{
    this->interpolate_ = enable;
}

void
MaRC::PhotoImageFactory::remove_sky(bool enable)
{
    this->remove_sky_ = enable;
}

void
MaRC::PhotoImageFactory::emi_ang_limit(double angle)
{
    this->emi_ang_limit_ = angle;
}

void
MaRC::PhotoImageFactory::body_center(double sample, double line)
{
    this->sample_center_ = sample;
    this->line_center_   = line;
}

void
MaRC::PhotoImageFactory::lat_lon_center(double lat, double lon)
{
    this->lat_at_center_ = lat;
    this->lon_at_center_ = lon;
}

void
MaRC::PhotoImageFactory::optical_axis(double sample, double line)
{
    this->OA_s_ = sample;
    this->OA_l_ = line;
}

void
MaRC::PhotoImageFactory::geometric_correction(bool enable)
{
    this->geometric_correction_ = enable;
}

void
MaRC::PhotoImageFactory::photometric_correction(bool enable)
{
    this->photometric_correction_ = enable;
}

void
MaRC::PhotoImageFactory::sub_observ(double lat, double lon)
{
    this->sub_observ_lat_ = lat;
    this->sub_observ_lon_ = lon;
}

void
MaRC::PhotoImageFactory::sub_solar(double lat, double lon)
{
    this->sub_solar_lat_ = lat;
    this->sub_solar_lon_ = lon;
}

void
MaRC::PhotoImageFactory::range(double r)
{
    this->range_ = r;
}

void
MaRC::PhotoImageFactory::focal_length(double len)
{
    this->focal_length_ = len;
}

void
MaRC::PhotoImageFactory::scale(double s)
{
    this->scale_ = s;
}

void
MaRC::PhotoImageFactory::position_angle(double north)
{
    this->position_angle_ = north;
}

void
MaRC::PhotoImageFactory::arcsec_per_pixel(double a)
{
    this->arcsec_per_pixel_ = a;
}

void
MaRC::PhotoImageFactory::km_per_pixel(double k)
{
    this->km_per_pixel_ = k;
}

void
MaRC::PhotoImageFactory::use_terminator(bool u)
{
    this->use_terminator_ = u;
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
        fits_get_errstatus (status, fits_error);

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
