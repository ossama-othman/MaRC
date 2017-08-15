/**
 * @file MapFactory.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#include "MapFactory.h"
#include "Map_traits.h"
#include "SourceImage.h"

#include <stdexcept>
#include <iostream>


template <typename T>
MaRC::MapFactory<T>::MapFactory()
    : percent_complete_old_(0)
{
}

template <typename T>
MaRC::MapFactory<T>::~MapFactory()
{
}

template <typename T>
typename MaRC::MapFactory<T>::map_type
MaRC::MapFactory<T>::make_map(SourceImage const & source,
                              std::size_t samples,
                              std::size_t lines,
                              double minimum,
                              double maximum)
{
    map_type map(samples * lines, Map_traits<T>::empty_value());

    using namespace std::placeholders;

    /**
     * @todo This is an absurd amount of parameters.  Move most, if
     *       not all, of the parameters to a structure that will be
     *       passed in as a single parameter instead.
     */
    auto plot = std::bind(&MapFactory<T>::plot,
                          this,
                          std::cref(source),
                          minimum,
                          maximum,
                          _1,   // lat
                          _2,   // lon
                          _3,   // percent_complete
                          _4,   // map array offset
                          std::ref(map));

    this->plot_map(samples, lines, plot);

    return std::move(map);
}

template <typename T>
void
MaRC::MapFactory<T>::plot(SourceImage const & source,
                          double minimum,
                          double maximum,
                          double lat,
                          double lon,
                          unsigned char percent_complete,
                          std::size_t offset,
                          map_type & map)
{
    T & data =
#ifndef NDEBUG
        map.at(offset);  // Perform bounds check.
#else
        map[offset];
#endif

    // Clip minimum, maximum and datum to fit within map data type
    // range, if necessary.
    double datum = 0;

    bool const found_data =
        (source.read_data(lat, lon, datum)
         && datum >= Map_traits<T>::minimum(minimum)
         && datum <= Map_traits<T>::maximum(maximum));

    if (found_data)
        data = static_cast<T>(datum);

    /**
     * @todo Remove map progress output.  Library calls should not
     *       provide output unless desired by the user.
     */

    // Provide some output on the mapping progress.
    if (percent_complete == 100 && this->percent_complete_old_ != 0) {
        std::cout << "100%\n";
        this->percent_complete_old_ = 0;
    } else if (percent_complete > this->percent_complete_old_) {
        if (percent_complete % 20 == 0)
            std::cout
                << static_cast<unsigned int>(percent_complete)
                << std::flush;
        else if (percent_complete % 2 == 0)
            std::cout << '.' << std::flush;

        this->percent_complete_old_ = percent_complete;
    }
}
