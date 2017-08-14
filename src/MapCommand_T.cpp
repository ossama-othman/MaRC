/**
 * @file MapCommand_T.cpp
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

#include "MapCommand_T.h"
#include "FITS_traits.h"

#include <MaRC/config.h>  // For NDEBUG

#include <cassert>

#include <fitsio.h>


template <typename T>
MaRC::MapCommand_T<T>::MapCommand_T(
    std::string filename,
    std::string body_name,
    std::unique_ptr<MapFactory<T>> factory,
    long samples,
    long lines)
    : MapCommand(std::move(filename),
                 std::move(body_name),
                 samples,
                 lines)
    , factory_(std::move(factory))
{
}

template <typename T>
char const *
MaRC::MapCommand_T<T>::projection_name() const
{
    return this->factory_->projection_name();
}

template <typename T>
void
MaRC::MapCommand_T<T>::initialize_FITS_image(fitsfile * fptr,
                                             int & status)
{
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
    /**
     * @todo Check return value!
     */
    fits_create_img(fptr,
                    FITS::traits<T>::bitpix,
                    naxis,
                    naxes,
                    &status);

    // Write the BLANK keyword and value into the map FITS file.
    if (FITS::traits<T>::supports_blank_keyword && this->blank_set_) {
        fits_update_key(fptr,
                        TINT,
                        "BLANK",
                        &this->blank_,
                        "Value of pixels with undefined physical value.",
                        &status);
    }
}

template <typename T>
void
MaRC::MapCommand_T<T>::make_map_planes(fitsfile * fptr, int & status)
{
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

    // Create and write the map planes.
    for (auto const & i : this->image_factories_) {
        std::cout << "Plane "
                  << plane_count << " / " << num_planes
                  <<" : " << std::flush;

        // Create the SourceImage.
        std::unique_ptr<SourceImage> image(i->make(scale_and_offset<T>));

        // Create the map plane.
        /**
         * @todo Pass the FITS @c BLANK value (@see @c this->blank_)
         *       if one was supplied (@see @c this->blank_set_) to
         *       @c make_map() so that the map may be initialized with
         *       that value in the integer data typed map case.
         */
        auto map(this->factory_->make_map(*image,
                                          this->samples_,
                                          this->lines_,
                                          i->minimum(),
                                          i->maximum()));

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

template <typename T>
typename MaRC::MapCommand_T<T>::grid_type
MaRC::MapCommand_T<T>::make_grid(long samples,
                                 long lines,
                                 float lat_interval,
                                 float lon_interval)
{
    return this->factory_->make_grid(samples,
                                     lines,
                                     lat_interval,
                                     lon_interval);
}
