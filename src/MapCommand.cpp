/**
 * @file MapCommand.cpp
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

#include "MapCommand.h"
#include "MapCommand_t.cpp"

#include "FITS_traits.h"
#include "FITS_memory.h"

#include <marc/VirtualImage.h>
#include <marc/Mathematics.h>
#include <marc/Log.h>
#include <marc/config.h>

#include <fitsio.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <type_traits>  // For sanity check below.
#include <chrono>
#include <cassert>

#include <unistd.h>


namespace
{
    /// Obtain string representation of the given value.
    std::string double_to_string(double value)
    {
        // Values in FITS headers go in columns 11 through 30,
        // i.e. there is room for 20 characters;
        constexpr int fits_width = 20;

        // Maximum precision of a 64 bit floating value (double) in
        // base 10, leaving room for characters used in scientific
        // notation (e.g. 1.0e-03)
        constexpr int double_width =
            std::numeric_limits<double>::digits10 - 4;

        constexpr int width =
            fits_width < double_width ? fits_width : double_width;

        // Avoid writing "-0".  It's harmless but rather unsightly.
        constexpr double ulps = 1;
        if (MaRC::almost_zero(value, ulps))
            value = 0;

        /**
         * @todo Replace this std::ostringstream based conversion with
         *       a fmt library based approach using, such as one that
         *       leverages @c fmt::memory_buffer.
         */
        // Work around inability to change precision in
        // std::to_string().  Yes, this is ugly.
        std::ostringstream os;

        os << std::setprecision(width) << value;

        return os.str();
    }
}


MaRC::MapCommand::MapCommand(std::string filename,
                             long samples,
                             long lines,
                             std::unique_ptr<MapFactory> factory,
                             std::unique_ptr<MapParameters> params)
    : samples_(samples)
    , lines_(lines)
    , factory_(std::move(factory))
    , image_factories_()
    , filename_(std::move(filename))
    , comments_()
    , xcomments_()
    , lat_interval_(0)
    , lon_interval_(0)
    , transform_data_(false)
    , create_grid_(false)
    , parameters_(std::move(params))
{
    // Compile-time FITS data type sanity check.
    static_assert(
        /**
         * Make sure the %MaRC %FITS types satisfy standard %FITS data
         * type requirements, as well ensuring they match CFITSIO
         * expectations.
         *
         * @todo Find a better place to put this static assertion.
         *
         * @note We could achieve a similar affect in a simpler manner
         *       through @c std::is_same but the below approach
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

int
MaRC::MapCommand::execute()
{
    // All necessary map configuration parameters should now be in
    // place.  Populate other parameters automatically, if possible.
    if (!populate_map_parameters())
        return -1;

    std::cout << "\nCreating map: " << this->filename_ << '\n';

    (void) unlink(this->filename_.c_str());

    // Create the map file.
    fitsfile * fptr = 0;
    int status = 0;
    if (fits_create_file(&fptr, this->filename_.c_str(), &status) != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    FITS::file_unique_ptr safe_fptr(fptr);

    // Create primary image array HDU.
    this->initialize_FITS_image(fptr, status);

    /**
     * Establish that %MaRC created this %FITS files, e.g.
     * "MaRC 1.0".
     *
     * @note The @c CREATOR keyword is commonly used, but not part of
     *       the %FITS standard.
     * @note We could also use the @c PROGRAM keyword here instead.
     *       It is also commonly used but not in the %FITS standard.
     */
    fits_update_key(fptr,
                    TSTRING,
                    "CREATOR",
                    const_cast<char *>(PACKAGE_STRING),
                    "software that created this FITS file",
                    &status);

    // Write the author name if supplied.
    auto const author = this->parameters_->author();
    if (!author.empty()) {
        fits_update_key(fptr,
                        TSTRING,
                        "AUTHOR",
                        const_cast<char *>(author.c_str()),
                        "who compiled original data that was mapped",
                        &status);
    }

    // Write the name of the organization or institution responsible
    // for creating the FITS file, if supplied.
    auto const origin = this->parameters_->origin();
    if (!origin.empty()) {
        fits_update_key(fptr,
                        TSTRING,
                        "ORIGIN",
                        const_cast<char *>(origin.c_str()),
                        "map creator organization",
                        &status);
    }

    // Write the current date and time (i.e. the creation time) into
    // the map FITS file.
    fits_write_date(fptr, &status);

    // Write the name of the object being mapped.
    auto const object = this->parameters_->object();
    if (!object.empty()) {
        fits_update_key(fptr,
                        TSTRING,
                        "OBJECT",
                        const_cast<char *>(object.c_str()),
                        "name of observed object",
                        &status);
    } else {
        /**
         * @todo s/BODY/OBJECT/ once MaRC supports mapping objects on
         *       the Celestial Sphere.
         */
        MaRC::error("BODY not specified.");
        return -1;
    }

    // Write the map comments.
    for (auto const & comment : this->comments_) {
        fits_write_comment(fptr, comment.c_str(), &status);
    }

    std::string const history =
        std::string(this->projection_name())
        + " projection created by " PACKAGE_STRING ".";

    // Write some MaRC-specific HISTORY comments.
    fits_write_history(fptr,
                       history.c_str(),
                       &status);

    // Write the BSCALE and BZERO keywords and value into the map FITS
    // file.
    if (this->transform_data_) {
        /**
         * @bug This code is never called since @c transform_data_ is
         *      always false.  Support for allowing the user to
         *      specify the BSCALE and BZERO values has been broken
         *      for years so it is better to leave this code disabled
         *      until that is corrected.
         *
         * @todo Should we disable CFITSIO automatic scaling via
         *       @c fits_set_bscale() as we do for @c VirtualImage map
         *       planes?
         */
        double bscale = this->parameters_->bscale();
        double bzero  = this->parameters_->bzero();

        if (!std::isnan(bscale))
            fits_update_key(fptr,
                            TDOUBLE,
                            "BSCALE",
                            &bscale,
                            "linear data scaling coefficient",
                            &status);

        if (!std::isnan(bzero))
            fits_update_key(fptr,
                            TDOUBLE,
                            "BZERO",
                            &bzero,
                            "physical value corresponding to "
                            "zero in the map",
                            &status);
    }

    if (status != 0) {
        // Report any errors before creating the map since map
        // creation can be time consuming.
        fits_report_error(stderr, status);

        return status;
    }

    /**
     * @todo Map timing should not include %FITS file operations.
     */
    auto const start =  std::chrono::high_resolution_clock::now();

    // Create and write the map planes.
    switch (this->parameters_->bitpix()) {
    case BYTE_IMG:
        this->template make_map_planes<FITS::byte_type>(fptr, status);
        break;
    case SHORT_IMG:
        this->template make_map_planes<FITS::short_type>(fptr, status);
        break;
    case LONG_IMG:
        this->template make_map_planes<FITS::long_type>(fptr, status);
        break;
    case LONGLONG_IMG:
        this->template make_map_planes<FITS::longlong_type>(fptr, status);
        break;
    case FLOAT_IMG:
        this->template make_map_planes<FITS::float_type>(fptr, status);
        break;
    case DOUBLE_IMG:
        this->template make_map_planes<FITS::double_type>(fptr, status);
        break;
    default:
        // We should never get here.
        MaRC::error("Unexpected BITPIX value\n");
        return -1;
    }

    auto const end =  std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> const seconds = end - start;

    std::cout << "Completed mapping data in " << seconds.count()
              << " seconds.\n";

    /**
     * @todo Set map DATAMIN and DATAMAX automatically based actual
     *       mapped data.
     */
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

    // Write the map grid if requested.
    this->write_grid(fptr, status);

    fits_report_error(stderr, status);

    return status;
}

char const *
MaRC::MapCommand::projection_name() const
{
    return this->factory_->projection_name();
}

MaRC::MapCommand::grid_type
MaRC::MapCommand::make_grid(long samples,
                            long lines,
                            double lat_interval,
                            double lon_interval)
{
    return this->factory_->make_grid(samples,
                                     lines,
                                     lat_interval,
                                     lon_interval);
}

void
MaRC::MapCommand::write_grid(fitsfile * fptr, int & status)
{
    if (!this->create_grid_)
        return;

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
                    PACKAGE_NAME " grid extension name",
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
    double minimum = std::numeric_limits<grid_type::value_type>::max();
    double maximum = minimum;

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

    auto const start = std::chrono::high_resolution_clock::now();

    grid_type grid(this->make_grid(this->samples_,
                                   this->lines_,
                                   this->lat_interval_,
                                   this->lon_interval_));

    auto const end =  std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> const seconds = end - start;

    std::cout << "Completed mapping grid in " << seconds.count()
              << " seconds.\n";

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
MaRC::MapCommand::grid_intervals(double lat_interval, double lon_interval)
{
    /**
     * @todo Verify arguments.
     */

    this->create_grid_  = true;
    this->lat_interval_ = lat_interval;
    this->lon_interval_ = lon_interval;
}

void
MaRC::MapCommand::image_factories(image_factories_type factories)
{
    this->image_factories_ = std::move(factories);
}

void
MaRC::MapCommand::initialize_FITS_image(fitsfile * fptr, int & status)
{
    // The CFITSIO 'naxes' value is of type 'long'.  It is extremely
    // unlikely that a map with more than LONG_MAX planes will ever be
    // created, so this implicit conversion should be safe.
    long const planes = this->image_factories_.size();

    int const naxis =
        (planes > 1
         ? 3  /* 3 dimensions -- map "cube"  */
         : 2  /* 2 dimensions -- map "plane" */);

    // Specify map cube dimensions.  Note that in the two-dimensional
    // map case, the third "planes" dimension will be ignored since
    // the above "naxis" variable will be set to two, not three.
    long naxes[] = { this->samples_, this->lines_, planes };

    // Create the primary array.
    (void) fits_create_img(fptr,
                           this->parameters_->bitpix(),
                           naxis,
                           naxes,
                           &status);
}

void
MaRC::MapCommand::write_virtual_image_facts(fitsfile * fptr,
                                            std::size_t plane,
                                            std::size_t num_planes,
                                            SourceImage const * image,
                                            int & status)
{
    /**
     * @todo This entire method seems is a bit of hack.  Come up with
     *       a better design for writing map plane-specific
     *       information to the %FITS file.
     */

    VirtualImage const * const v =
        dynamic_cast<VirtualImage const *>(image);

    if (!v)
        return;  // Not a VirtualImage subclass instance.

    /**
     * @bug This prevents the BUNIT card from being written for
     *      floating point virtual images.
     */
    // bitpix > 0: integer data
    // bitpix < 0: floating point data
    if (this->parameters_->bitpix() < 0)
        return;

    // The map contains integer type data meaning data was scaled to
    // maximize significant digits.  Write map plane-specific scaling
    // factors to the FITS file.

    double scale  = v->scale();
    double offset = v->offset();
    std::string unit = v->unit();

    // Avoid writing "-0".  It's harmless but rather unsightly.
    constexpr double ulps = 1;
    if (MaRC::almost_zero(offset, ulps))
        offset = 0;

    /**
     * @todo Write the BSCALE, BZERO and BUNIT value if a multi-plane
     *       map is comprised entirely of the unit of data
     *       (e.g. cosines, degrees, etc).  No need to limit that to
     *       single plane maps in that case.
     */
    if (num_planes == 1) {
        // We're the sole plane in the map meaning we can update
        // actual FITS BSCALE and BZERO cards instead of writing
        // freeform text in a COMMENT or HISTORY card.

        if (this->transform_data_)
            MaRC::warn("computed scale and offset will override "
                       "user supplied values");

        // -------------------
        // Set scaling factors
        // -------------------
        /*
          The MaRC library already scales the VirtualImage values.
          Set the CFITSIO internal scaling factors to force raw values
          to be written by effectively disabling automatic data
          scaling.  Otherwise CFITSIO issues a numerical overflow
          error when writing the array (data) values to the FITS
          file.
        */
        constexpr double internal_scale  = 1;
        constexpr double internal_offset = 0;

        if (fits_update_key(fptr,
                            TDOUBLE,
                            "BSCALE",
                            &scale,
                            "linear data scaling coefficient",
                            &status) != 0
            || fits_update_key(fptr,
                               TDOUBLE,
                               "BZERO",
                               &offset,
                               "physical value corresponding "
                               "to zero in the map",
                               &status) != 0

            /*
              Set CFITSIO internal scaling factors for the current
              primary array of image extension.  They are independent
              of the FITS BSCALE and BZERO values set above.
            */
            || fits_set_bscale(fptr,
                               internal_scale,
                               internal_offset,
                               &status) != 0) {
            return;
        }

        // -------------------------------------------
        // Set physical value unit of the array values
        // ----------------------------------------
        if (!unit.empty()
            && fits_update_key(fptr,
                               TSTRING,
                               "BUNIT",
                               const_cast<char *>(unit.c_str()),
                               "physical unit of the array values",
                               &status) != 0) {
            return;
        }
    } else {
        comment_list_type facts;

        /**
         * @todo Improve appearance of map plane facts in %FITS file.
         */
        facts.emplace_back("Plane "
                           + std::to_string(plane)
                           + " characteristics:");
        facts.emplace_back("    BSCALE: " + double_to_string(scale));
        facts.emplace_back("    BZERO:  " + double_to_string(offset));

        if (!unit.empty()) {
            // Single quote the unit string per FITS string value
            // conventions.
            facts.emplace_back("    BUNIT:  '" + unit + "'");
        }

        for (auto & f : facts) {
            // Write some MaRC-specific HISTORY comments.
            fits_write_history(fptr,
                               f.c_str(),
                               &status);
        }
    }
}

bool
MaRC::MapCommand::populate_map_parameters()
{
    bool populated = true;

    /**
     * @todo Populate the following parameters from source image
     *       factories if they exist, and if possible:
     * @li AUTHOR
     * @li BITPIX
     * @li BLANK
     * @li BSCALE
     * @li BUNIT
     * @li BZERO
     * @li DATAMAX
     * @li DATAMIN
     * @li EQUINOX
     * @li INSTRUME
     * @li NAXES
     * @li OBJECT
     * @li OBSERVER
     * @li ORIGIN
     * @li REFERENC
     * @li TELESCOP
     */

    return populated;
}
