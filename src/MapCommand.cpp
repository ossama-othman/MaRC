/**
 * @file MapCommand.cpp
 *
 * Copyright (C) 2004, 2017-2020, 2024  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "MapCommand.h"
#include "MapCommand_t.cpp"

#include "FITS_traits.h"
#include "FITS_file.h"
#include "FITS_image.h"

#include <marc/VirtualImage.h>
#include <marc/Mathematics.h>
#include <marc/Log.h>
#include <marc/config.h>

#include <fitsio.h>

#include <iostream>
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
        constexpr int epsilons = 1;
        if (MaRC::almost_zero(value, epsilons))
            value = 0;

        // Work around inability to change precision in
        // std::to_string() by using fmt::format().
        return fmt::format("{:.{}g}", value, width);
    }
}


// ------------------------------------------------------------------

MaRC::MapCommand::MapCommand(std::string filename,
                             long samples,
                             long lines,
                             std::unique_ptr<MapFactory> factory,
                             std::unique_ptr<map_parameters> params)
    : samples_(samples)
    , lines_(lines)
    , factory_(std::move(factory))
    , image_factories_()
    , filename_(std::move(filename))
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

    assert(this->parameters_);
}

int
MaRC::MapCommand::execute()
{
    std::cout << "\nCreating map: " << this->filename_ << '\n';

    // All necessary map configuration parameters should now be in
    // place.  Populate other parameters automatically, if possible.
    if (!populate_map_parameters())
        return -1;

    (void) unlink(this->filename_.c_str());

    // Create the map file.
    FITS::output_file f(this->filename_.c_str());

    /**
     * @todo Map timing should not include %FITS file operations.
     */
    auto const start =  std::chrono::high_resolution_clock::now();

    // Create and write the map planes.
    switch (this->parameters_->bitpix()) {
    case BYTE_IMG:
        this->template make_map_planes<FITS::byte_type>(f);
        break;
    case SHORT_IMG:
        this->template make_map_planes<FITS::short_type>(f);
        break;
    case LONG_IMG:
        this->template make_map_planes<FITS::long_type>(f);
        break;
    case LONGLONG_IMG:
        this->template make_map_planes<FITS::longlong_type>(f);
        break;
    case FLOAT_IMG:
        this->template make_map_planes<FITS::float_type>(f);
        break;
    case DOUBLE_IMG:
        this->template make_map_planes<FITS::double_type>(f);
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

    // Write the map grid if requested.
    this->write_grid(f);

    std::cout << "Created map: " << this->filename_ << '\n';

    return 0;
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
MaRC::MapCommand::write_grid(MaRC::FITS::output_file & map_file)
{
    if (!this->create_grid_)
        return;

    constexpr std::size_t planes = 1;  // Only one grid image plane.
    static char const extname[] = "GRID";

    auto grid_image =
        map_file.make_image(FITS::traits<FITS::byte_type>::bitpix,
                            this->samples_,
                            this->lines_,
                            planes,
                            extname);

    // Write the grid comments.
    for (auto const & xcomment : this->parameters_->xcomments())
        grid_image->comment(xcomment);

    std::string const xhistory =
        std::string(this->projection_name())
        + " projection grid created using " PACKAGE_STRING ".";

    // Write some MaRC-specific HISTORY comments.
    grid_image->history(xhistory);

    // Write map grid DATAMIN and DATAMAX keywords.  Both are the
    // SAME, since only one valid value exists in the grid image.
    double minimum = std::numeric_limits<grid_type::value_type>::max();
    double maximum = minimum;

    grid_image->datamin(minimum);
    grid_image->datamax(maximum);

    int grid_blank = 0;
    grid_image->blank<decltype(grid_blank)>(grid_blank);

    auto const start = std::chrono::high_resolution_clock::now();

    grid_type grid(this->make_grid(this->samples_,
                                   this->lines_,
                                   this->lat_interval_,
                                   this->lon_interval_));

    auto const end =  std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> const seconds = end - start;

    std::cout << "Completed mapping grid in " << seconds.count()
              << " seconds.\n";

    if(!grid_image->template write<grid_type>(grid))
        MaRC::error("Unable to write grid image to map file.");
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
MaRC::MapCommand::write_virtual_image_facts(MaRC::FITS::image & map_image,
                                            std::size_t plane,
                                            std::size_t num_planes,
                                            SourceImage const * image)
{
    /**
     * @todo This entire method seems is a bit of hack.  Come up with
     *       a better design for writing map plane-specific
     *       information to the %FITS file.
     */

    auto const v = dynamic_cast<VirtualImage const *>(image);

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

    // Avoid writing "-0".  It's harmless but rather unsightly.
    constexpr int epsilons = 1;
    if (MaRC::almost_zero(offset, epsilons))
        offset = 0;

    // -------------------------------------------
    // Set physical value unit of the array values
    // -------------------------------------------
    map_image.bunit(this->parameters_->bunit());

    /**
     * @todo Write the BSCALE, BZERO and BUNIT value if a multi-plane
     *       map is comprised entirely of the unit of data
     *       (e.g. cosines, degrees, etc).  No need to limit that to
     *       single plane maps in that case.
     */
    if (num_planes == 1) {
        /*
          We're the sole plane in the map meaning we can update
          actual FITS BSCALE and BZERO cards instead of writing
          freeform text in a COMMENT or HISTORY card.
        */

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

        map_image.bscale(scale);
        map_image.bzero(offset);

        /*
          Set CFITSIO internal scaling factors for the current
          primary array or image extension.  They are independent
          of the FITS BSCALE and BZERO values set above.
        */
        map_image.internal_scale(internal_scale, internal_offset);
    } else {

        /**
         * @todo Improve appearance of map plane facts in %FITS file.
         */

        // Write some MaRC-specific HISTORY comments.
        map_image.history(fmt::format("Plane {} characteristics:",
                                      plane));

        map_image.history("    BSCALE: " + double_to_string(scale));
        map_image.history("    BZERO:  " + double_to_string(offset));

        /**
         * @bug Regression.  Map plane units are no longer written to
         *      @c HISTORY comments in the map %FITS file.
         */
        /*
        if (!unit.empty()) {
            // Single quote the unit string per FITS string value
            // conventions.
            map_image.history("    BUNIT:  '" + unit + "'");
        }
        */
    }
}

bool
MaRC::MapCommand::populate_map_parameters()
{
    /*
      Iterate through the list of source images (map planes) in an
      attempt to automatically set each of the map parameters.

      Mandatory Parameters:
        - Data type (BITPIX)
        - Map plane dimensions, i.e. samples and lines (NAXIS1 and
          NAXIS2)
     */

    // Automatically populated map parameters.
    map_parameters to_merge;

    int plane = 1;

    for (auto const & image : this->image_factories_) {
        // Automatically populated map plane parameters.
        map_parameters pp(plane++);

        if (!image->populate_parameters(pp))
            return false;

        /*
          Merge map plane parameters to previously populated
          parameters.
        */
        if (!to_merge.merge(pp))
            return false;
    }


    /*
      Merge automatically populated map parameters with the user
      supplied parameters.  Some user supplied parameters, such as
      bitpix (map data type), are given priority over automatically
      populated ones.
    */
    /**
     * @todo Implement user override semantics.
     */
    return this->parameters_->merge(to_merge);
}

int
MaRC::MapCommand::number_of_digits(std::size_t num)
{
    constexpr int base = 10;
    int digits = 0;

    for ( ; num != 0; num /= base)
        ++digits;

    /**
     * @note We could also determine the number of digits in the
     *       integer using std::log10(num) + 1, instead.
     */

    return digits;
}
