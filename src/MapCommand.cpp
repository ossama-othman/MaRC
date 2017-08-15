/**
 * @file MapCommand.cpp
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

#include "MapCommand.h"
#include "FITS_traits.h"
#include "FITS_memory.h"

#include <MaRC/config.h>

#include <fitsio.h>

#include <iostream>
#include <memory>
#include <type_traits>  // For sanity check below.
#include <cassert>

#include <unistd.h>


MaRC::MapCommand::MapCommand(std::string filename,
                             std::string body_name,
                             long samples,
                             long lines)
    : samples_(samples)
    , lines_(lines)
    , image_factories_()
    , blank_set_(false)
    , blank_(0)
    , filename_(std::move(filename))
    , author_()
    , origin_()
    , comments_()
    , xcomments_()
    , body_name_(std::move(body_name))
    , lat_interval_(0)
    , lon_interval_(0)
    , bscale_(1)
    , bzero_(0)
    , transform_data_(false)
    , create_grid_(false)
{
    // Compile-time FITS data type sanity check.
    static_assert(
        /**
         * Make sure the MaRC FITS types satisfy standard FITS data
         * type requirements, as well ensuring they match CFITSIO
         * expectations.
         *
         * @todo Find a better place to put this static assertion.
         *
         * @note We could achieve a similar affect in a simpler manner
         *       through @c std::is_same<> but the below approach
         *       doesn't make assumptions of the underlying types.
         */
        sizeof(FITS::byte_type) == 1
        && sizeof(FITS::byte_type) == sizeof(unsigned char)
        && std::is_integral<FITS::byte_type>()
        && std::is_unsigned<FITS::byte_type>()  // unsigned

        && sizeof(FITS::short_type) == 2
        && sizeof(FITS::short_type) == sizeof(short)
        && std::is_integral<FITS::short_type>()
        && std::is_signed<FITS::short_type>()   // signed

        && sizeof(FITS::long_type) == 4
        && sizeof(FITS::long_type) == (sizeof(long) == 4
                                       ? sizeof(long)
                                       : sizeof(int))
        && std::is_integral<FITS::long_type>()
        && std::is_signed<FITS::long_type>()

        && sizeof(FITS::longlong_type) == 8
        && sizeof(FITS::longlong_type) == sizeof(LONGLONG)
        && std::is_integral<FITS::longlong_type>()
        && std::is_signed<FITS::longlong_type>()

        // Floating point values are always signed.
        && sizeof(FITS::float_type) == 4
        && std::is_floating_point<FITS::float_type>()

        && sizeof(FITS::double_type) == 8
        && std::is_floating_point<FITS::double_type>(),

        "Underlying types do not satisfy FITS data type requirements.");
}

MaRC::MapCommand::~MapCommand()
{
}

int
MaRC::MapCommand::execute()
{
    std::cout << "\nCreating map: " << this->filename_ << '\n';

    (void) unlink(this->filename_.c_str());

    // Create the map file.
    fitsfile * fptr = 0;
    int status = 0;
    if (fits_create_file(&fptr, this->filename_.c_str(), &status) != 0)
        return status;

    FITS::file_unique_ptr safe_fptr(fptr);

    // Create primary image array HDU.
    this->initialize_FITS_image(fptr, status);

    /**
     * Establish that MaRC created this FITS files, e.g. "MaRC 1.0".
     *
     * @note The @c CREATOR keyword is a commonly used, but not part
     *       of the FITS standard.
     * @note We could also use the @c PROGRAM keyword here instead.
     *       It is also commonly used but not in the FITS standard.
     */
    fits_update_key(fptr,
                    TSTRING,
                    "CREATOR",
                    const_cast<char *>(PACKAGE_STRING),
                    "Software that created this FITS file",
                    &status);

    // Write the author name if supplied.
    if (!this->author_.empty()) {
        char const * const author = this->author_.c_str();
        fits_update_key(fptr,
                        TSTRING,
                        "AUTHOR",
                        const_cast<char *>(author),
                        "Who compiled original data that was mapped",
                        &status);
    }

    // Write the name of the organization or institution responsible
    // for creating the FITS file, if supplied.
    if (!this->origin_.empty()) {
        char const * const origin = this->origin_.c_str();
        fits_update_key(fptr,
                        TSTRING,
                        "ORIGIN",
                        const_cast<char *>(origin),
                        "Map creator organization",
                        &status);
    }

    // Write the current date and time (i.e. the creation time) into
    // the map FITS file.
    fits_write_date(fptr, &status);

    // Write the name of the object being mapped.
    char const * const object = this->body_name_.c_str();
    fits_update_key(fptr,
                    TSTRING,
                    "OBJECT",
                    const_cast<char *>(object),
                    "Name of observed object.",
                    &status);

    // Write the map comments.
    for (auto const & comment : this->comments_) {
        fits_write_comment(fptr, comment.c_str(), &status);
    }

    std::string const history =
        std::string(this->projection_name())
        + " projection created using MaRC " PACKAGE_VERSION ".";

    // Write some MaRC-specific HISTORY comments.
    fits_write_history(fptr,
                       history.c_str(),
                       &status);

    // Write the BSCALE and BZERO keywords and value into the map FITS
    // file.
    if (this->transform_data_) {
        fits_update_key(fptr,
                        TFLOAT,
                        "BSCALE",
                        &this->bscale_,
                        "Coefficient of linear term in the "
                        "scaling equation.",
                        &status);

        fits_update_key(fptr,
                        TFLOAT,
                        "BZERO",
                        &this->bzero_,
                        "Physical value corresponding to an array "
                        "value of zero.",
                        &status);
    }

    if (status != 0)  {
        // Report any errors before creating the map since map
        // creation can be time consuming.
        fits_report_error(stderr, status);

        return status;
    }

    // Create and write the map planes.
    this->make_map_planes(fptr, status);

//   // Write DATAMIN and DATAMAX keywords.
//   fits_update_key(fptr,
//                   TFLOAT,
//                   "DATAMIN",
//                   &this->minimum_,
//                   "Minimum valid physical value.",
//                   &status);

//   fits_update_key(fptr,
//                   TFLOAT,
//                   "DATAMAX",
//                   &this->maximum_,
//                   "Maximum valid physical value.",
//                   &status);

    // Write a checksum for the map.
    fits_write_chksum(fptr, &status);

    if (this->create_grid_) {
        long naxes[] = { this->samples_, this->lines_ };
        constexpr int grid_naxis = sizeof(naxes) / sizeof(naxes[0]);

        static_assert(grid_naxis == 2,
                      "Map grid is not two dimensional.");

        // Create the image extension containing the map grid.
        /**
         * @todo Check return value!
         */
        fits_create_img(fptr,
                        FITS::traits<FITS::byte_type>::bitpix,
                        grid_naxis,
                        naxes,
                        &status);

        static char const extname[] = "GRID";
        fits_update_key(fptr,
                        TSTRING,
                        "EXTNAME",
                        const_cast<char *>(extname),
                        "MaRC grid extension name",
                        &status);


        // Write the grid comments.
        for (auto const & xcomment : this->xcomments_) {
            fits_write_comment(fptr, xcomment.c_str(), &status);
        }

        std::string const xhistory =
            std::string(this->projection_name())
            + " projection grid created using " PACKAGE_STRING ".";

        // Write some MaRC-specific HISTORY comments.
        fits_write_history(fptr,
                           xhistory.c_str(),
                           &status);

        // Write map grid DATAMIN and DATAMAX keywords.  Both are the
        // SAME, since only one valid value exists in the grid image.
        float minimum = std::numeric_limits<grid_type::value_type>::max();
        float maximum = minimum;

        fits_update_key(fptr,
                        TFLOAT,
                        "DATAMIN",
                        &minimum,
                        "minimum valid grid value",
                        &status);

        fits_update_key(fptr,
                        TFLOAT,
                        "DATAMAX",
                        &maximum,
                        "maximum valid grid value",
                        &status);

        int grid_blank = 0;
        fits_update_key(fptr,
                        TINT,
                        "BLANK",
                        &grid_blank,
                        "value of off-grid pixels",
                        &status);

        grid_type grid(this->make_grid(this->samples_,
                                       this->lines_,
                                       this->lat_interval_,
                                       this->lon_interval_));

        // Sanity check.
        assert(grid.size()
               == static_cast<std::size_t>(this->samples_) * this->lines_);

        // LONGLONG is a CFITSIO type.
        constexpr LONGLONG fpixel = 1;  // First pixel/element
        LONGLONG const nelements = grid.size();

        /**
         * @todo Check return value!
         */
        (void) fits_write_img(fptr,
                              FITS::traits<grid_type::value_type>::datatype,
                              fpixel,
                              nelements,
                              grid.data(),
                              &status);

        // Write a checksum for the grid.
        fits_write_chksum(fptr, &status);
    }

    fits_report_error(stderr, status);

    return status;
}

void
MaRC::MapCommand::author(std::string author)
{
    this->author_ = std::move(author);
}

void
MaRC::MapCommand::origin(std::string origin)
{
    this->origin_ = std::move(origin);
}

void
MaRC::MapCommand::comment_list(comment_list_type comments)
{
    this->comments_= std::move(comments);
}

void
MaRC::MapCommand::xcomment_list(comment_list_type comments)
{
    this->xcomments_ = std::move(comments);
}

void
MaRC::MapCommand::grid_intervals(float lat_interval, float lon_interval)
{
    /**
     * @todo Verify arguments.
     */

    this->create_grid_  = true;
    this->lat_interval_ = lat_interval;
    this->lon_interval_ = lon_interval;
}

void
MaRC::MapCommand::data_zero(double zero)
{
    this->bzero_ = zero;
    this->transform_data_ = true;
}

void
MaRC::MapCommand::data_scale(double scale)
{
    this->bscale_ = scale;
    this->transform_data_ = true;
}

void
MaRC::MapCommand::data_blank(int blank)
{
    this->blank_ = blank;
    this->blank_set_ = true;
}

void
MaRC::MapCommand::image_factories(image_factories_type factories)
{
    this->image_factories_ = std::move(factories);
}
