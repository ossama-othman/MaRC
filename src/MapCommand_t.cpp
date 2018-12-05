/**
 * @file MapCommand_T.cpp
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

#ifndef MARC_MAP_COMMAND_T_CPP
#define MARC_MAP_COMMAND_T_CPP

#include "MapCommand.h"
#include "FITS_traits.h"
#include "ProgressConsole.h"

#include <marc/MapFactory.h>
#include <marc/scale_and_offset.h>

#include <cassert>
#include <type_traits>
#include <iostream>

#include <fitsio.h>


template <typename T>
void
MaRC::MapCommand::make_map_planes(fitsfile * fptr, int & status)
{
    auto blank = this->parameters_->blank();

    // Write the BLANK keyword and value into the map FITS file.
    if (std::is_integral<T>() && blank) {
        T blank_value = *blank;

        // Don't bother checking if the blank value fits within the
        // range of type T.  The make_map() call below already does
        // that.

        fits_update_key(fptr,
                        FITS::traits<T>::datatype,
                        "BLANK",
                        &blank_value,
                        "value of pixels with undefined physical value",
                        &status);
    }

    // First pixel/element in FITS array (1-based).
    //   Plane 1: fpixel =  1
    //   Plane 2: fpixel += nelements
    //   Plane 3: fpixel += nelements
    //   Plane 4: ... etc ...

    // LONGLONG is a CFITSIO type.
    LONGLONG fpixel = 1;  // First pixel.

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
        this->write_virtual_image_facts(fptr,
                                        plane_count,
                                        num_planes,
                                        image.get(),
                                        status);

        plot_info info(*image,
                       i->minimum(),
                       i->maximum(),
                       blank);

        using namespace MaRC::Progress;
        info.notifier().subscribe(std::make_unique<Console>());

        // Create the map plane.
        auto map(this->factory_->template make_map<T>(info,
                                                      this->samples_,
                                                      this->lines_));

        // Sanity check.
        assert(map.size()
               == static_cast<std::size_t>(this->samples_) * this->lines_);

        LONGLONG const nelements = map.size();

        /**
         * @todo Check return value!
         */
        fits_write_img(fptr,
                       FITS::traits<T>::datatype,
                       fpixel,
                       nelements,
                       map.data(),
                       &status);

        // Set offset in the FITS array to the next plane.
        fpixel += nelements;

        ++plane_count;
    }
}


#endif  // MARC_MAP_COMMAND_T_CPP
