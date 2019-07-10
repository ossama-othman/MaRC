/**
 * @file MapCommand.cpp
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
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

    // -------------------------------------------------------------------

    template <typename T>
    struct parameter_matcher
    {
        static bool
        match(T const & lhs, T const & rhs)
        {
            return lhs == rhs;
        }
    };

    template <>
    struct parameter_matcher<double>
    {
        static bool
        match(double lhs, double rhs)
        {
            constexpr int ulps = 2;
            return MaRC::almost_equal(lhs, rhs, ulps);
        }
    };

    template <typename T>
    bool
    match_parameter(T const & lhs, T const & rhs)
    {
        return parameter_matcher<T>::match(lhs, rhs);
    }

#if 0
    template <typename T, typename U>
    bool
    populate_param(
        char const * keyword,
        T && param,
        MaRC::MapCommand::image_factories_type const & factories,
        U set_param,
        bool require_match)
    {
        T new_param(param);

        for (auto const & plane : factories) {
            auto const & plane_param = plane->param();

            if (new_param.empty()) {
                new_param = plane_param;
            } else if (!match_parameter(new_param, plane_param)) {
                MaRC::info("Different {} values between map planes:",
                           keyword);
                MaRC::info("\t'{}' and '{}'.", new_param, plane_param);

                if (require_match) {
                    /**
                     * @todo Should we treat this case as a soft error,
                     *       and simply warn the user instead?
                     */
                    MaRC::error("They must match.");

                    return false;
                } else {
                    MaRC::info("{} will only be embedded in the map "
                               "file in a HISTORY comment.",
                               keyword);

                    return true;
                }
            }
        }

        set_param(std::move(new_param));

        return true;
    }
#elif 0
    template <typename T, typename U>
    bool
    populate_param(
        char const * keyword,
        T && param,
        MaRC::MapCommand::image_factories_type const & factories,
        U set_param,
        bool require_match)
    {
        T new_param(param);


            auto const & plane_param = plane->param();

            if (new_param.empty()) {
                new_param = plane_param;
            } else if (!match_parameter(new_param, plane_param)) {
                MaRC::info("Different {} values between map planes:",
                           keyword);
                MaRC::info("\t'{}' and '{}'.", new_param, plane_param);

                if (require_match) {
                    /**
                     * @todo Should we treat this case as a soft error,
                     *       and simply warn the user instead?
                     */
                    MaRC::error("They must match.");

                    return false;
                } else {
                    MaRC::info("{} will only be embedded in the map "
                               "file in a HISTORY comment.",
                               keyword);

                    return true;
                }
            }

        set_param(std::move(new_param));

        return true;
    }
#endif  // 0
}


// ------------------------------------------------------------------

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

        map_image.bscale(scale);
        map_image.bzero(offset);

        /*
          Set CFITSIO internal scaling factors for the current
          primary array of image extension.  They are independent
          of the FITS BSCALE and BZERO values set above.
        */
        map_image.internal_scale(internal_scale, internal_offset);

        // -------------------------------------------
        // Set physical value unit of the array values
        // -------------------------------------------
        map_image.bunit(unit);
    } else {

        /**
         * @todo Improve appearance of map plane facts in %FITS file.
         */

        // Write some MaRC-specific HISTORY comments.
        map_image.history("Plane "
                           + std::to_string(plane)
                           + " characteristics:");

        map_image.history("    BSCALE: " + double_to_string(scale));
        map_image.history("    BZERO:  " + double_to_string(offset));

        if (!unit.empty()) {
            // Single quote the unit string per FITS string value
            // conventions.
            map_image.history("    BUNIT:  '" + unit + "'");
        }
    }
}

bool
MaRC::MapCommand::populate_map_parameters()
{
    bool populated = true;

    /**
     * Populate the following parameters from source image factories
     * if possible:
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

    /*
      Iterate through the list of source images (map planes) in an
      attempt to automatically set each of the map parameters.

      Mandatory Parameters:
        - Data type (BITPIX)
        - Map plane dimensions, i.e. samples and lines (NAXIS1 and
          NAXIS2)
     */

#if 0
    populated =
        populate_param(
            "AUTHOR",
            this->parameters_->author(),
            this->image_factories_,
            [&](auto const & value)
            {
                this->parameters_->author(value);
            },
            false);


    for (auto const & plane : this->image_factories_) {

    }
#endif  // 0

    return populated;
}
