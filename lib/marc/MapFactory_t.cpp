/**
 * @file MapFactory_t.cpp
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
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

#ifndef MARC_MAP_FACTORY_T_CPP
#define MARC_MAP_FACTORY_T_CPP

#include "marc/MapFactory.h"
#include "marc/Map_traits.h"
#include "marc/SourceImage.h"
#include "marc/plot_info.h"

#include <type_traits>
#include <limits>
#include <stdexcept>


template <typename T>
MaRC::MapFactory::map_type<T>
MaRC::MapFactory::make_map(plot_info & info,
                           std::size_t samples,
                           std::size_t lines)
{
    T blank = Map_traits<T>::empty_value();

    if (std::is_integral<T>::value && info.blank()) {
        if (info.blank() < std::numeric_limits<T>::lowest()
            || info.blank() > std::numeric_limits<T>::max()) {
            throw std::invalid_argument("Blank map value does not fit "
                                        "within map data type.");
        }

        blank = static_cast<T>(*info.blank());
    }

    T const actual_min = Map_traits<T>::minimum(info.minimum());
    T const actual_max = Map_traits<T>::maximum(info.maximum());
    info.minimum(actual_min);
    info.maximum(actual_max);

    map_type<T> map(samples * lines, blank);

    using namespace std::placeholders;

    auto plot = std::bind(&MapFactory::plot<T>,
                          this,
                          std::cref(info),
                          _1,   // lat
                          _2,   // lon
                          _3,   // map array offset
                          std::ref(map));

    this->plot_map(samples, lines, plot);

    // Inform "observers" of map completion.
    info.notifier().notify_done(map.size());

    return std::move(map);
}

template <typename T>
void
MaRC::MapFactory::plot(plot_info const & info,
                       double lat,
                       double lon,
                       std::size_t offset,
                       map_type<T> & map)
{
    // Clip datum to fit within map data type range, if necessary.
    double datum = 0;

    bool const found_data =
        (info.source().read_data(lat, lon, datum)
         && datum >= info.minimum()
         && datum <= info.maximum());

    if (found_data)
        map[offset] = static_cast<T>(datum);

    // Inform "observers" of mapping progress.
    info.notifier().notify_plotted(map.size());
}


#endif  // MARC_MAP_FACTORY_T_CPP
