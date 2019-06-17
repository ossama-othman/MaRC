/**
 * @file MapCommand_T.cpp
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
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

#ifndef MARC_MAP_COMMAND_T_CPP
#define MARC_MAP_COMMAND_T_CPP

#include "MapCommand.h"
#include "FITS_traits.h"
#include "FITS_image.h"
#include "ProgressConsole.h"

#include <marc/MapFactory.h>
#include <marc/scale_and_offset.h>

#include <cassert>
#include <type_traits>
#include <iostream>

#include <fitsio.h>


template <typename T>
void
MaRC::MapCommand::make_map_planes(MaRC::FITS::output_file & file)
{
    // Create primary image array HDU.
    auto map_image =
        file.make_image(this->parameters_->bitpix(),
                        this->samples_,
                        this->lines_,
                        this->image_factories_.size());

    /*
      The underlying integer type for the MaRC library "blank_type" is
      potentially wider than the largest FITS integer type (64 bit
      signed integer).  Explicitly convert to the FITS blank integer
      type used in the MaRC program to address conversion related
      errors at compile-time exhibited by some compilers.
    */
    auto blank = this->parameters_->blank();
    if (blank.has_value()) {
        using fits_blank_type = FITS::image::blank_type::value_type;

        /*
          Truncation will not occur since the MaRC program will pass
          at most pass a 64 bit signed integer to the MaRC library.
        */
        fits_blank_type fits_blank =
            static_cast<fits_blank_type>(*blank);

        // Write the BLANK keyword and value into the map FITS file.
        map_image->template blank<T>(fits_blank);
    }

    // Write the author name if supplied.
    auto const author = this->parameters_->author();
    map_image->author(author);

    // Write the name of the organization or institution responsible
    // for creating the FITS file, if supplied.
    auto const origin = this->parameters_->origin();
    map_image->origin(origin);

    // Write the name of the object being mapped.
    auto const object = this->parameters_->object();

    if (object.empty()) {
        /**
         * @todo s/BODY/OBJECT/ once MaRC supports mapping objects on
         *       the Celestial Sphere.
         */
        MaRC::error("BODY not specified.");
    }

    map_image->object(object);

    // Write the map comments.
    for (auto const & comment : this->comments_)
        map_image->comment(comment);

    std::string const history =
        std::string(this->projection_name())
        + " projection created by " PACKAGE_STRING ".";

    // Write some MaRC-specific HISTORY comments.
    map_image->history(history);

/*
    // Write MaRC-specific HISTORY comments.
    for (auto & f : facts)
        map_image->history(f);
*/

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

        map_image->bscale(bscale);
        map_image->bzero(bzero);
    }

    /**
     * @todo Write all available %FITS keyword values in the given
     *       image parameters.
     */

    // Keep track of mapped planes for reporting to user.
    int plane_count = 1;
    std::size_t const num_planes = this->image_factories_.size();

    ImageFactory::scale_offset_functor const sof =
        scale_and_offset<T>;

    // Create and write the map planes.
    for (auto const & i : this->image_factories_) {
        // Create the SourceImage.
        std::unique_ptr<SourceImage> const image(i->make(sof));

        if (!image)
            continue;  // Problem creating SourceImage.  Move on.

        std::cout << "Plane "
                  << plane_count << " / " << num_planes
                  <<" : " << std::flush;

        // Add description of the source image.
        // comment_list_type descriptions;

        // std::string image_title =
        //     "Plane " + std::to_string(plane_count) ": " + image->name();

        // descriptions.push_back();

        /**
         * @todo Refactor this call so that it isn't specific to
         *       @c VirtualImage subclasses.
         */
        // Add description specific to the VirtualImage, if we have
        // one, in the map FITS file.
        this->write_virtual_image_facts(*map_image,
                                        plane_count,
                                        num_planes,
                                        image.get());

        /**
         * @bug These @c ImageFactory methods return a @c double
         *      value, which is not appropriate for 64 bit integer
         *      values greater than 2^53, i.e. the width of the
         *      significand in 64 bit IEEE 754 floating point values.
         */
        auto minimum = i->minimum();
        auto maximum = i->maximum();

        if (std::isnan(minimum))
            minimum = std::numeric_limits<T>::lowest();

        if (std::isnan(maximum))
            maximum = std::numeric_limits<T>::max();

        plot_info info(*image, minimum, maximum, blank);

        using namespace MaRC::Progress;
        info.notifier().subscribe(std::make_unique<Console>());

        // Create the map plane.
        auto map(this->factory_->template make_map<T>(info,
                                                      this->samples_,
                                                      this->lines_));

        if (!map_image->template write<decltype(map)>(map))
            MaRC::error("Unable to write plane to map file.");

        ++plane_count;
    }

    /**
     * @todo Set map DATAMIN and DATAMAX automatically based on actual
     *       mapped data.
     */
    // Write DATAMIN and DATAMAX keywords.
    // map_image->datamin(this->minimum_);
    // map_image->datamax(this->maximum_);
}


#endif  // MARC_MAP_COMMAND_T_CPP
