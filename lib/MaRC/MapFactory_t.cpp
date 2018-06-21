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

#include "MaRC/MapFactory.h"
#include "MaRC/Map_traits.h"
#include "MaRC/SourceImage.h"
#include "MaRC/plot_info.h"


template <typename T>
MaRC::MapFactory::map_type<T>
MaRC::MapFactory::make_map(plot_info const & info,
                           std::size_t samples,
                           std::size_t lines)
{
    map_type<T> map(samples * lines, Map_traits<T>::empty_value());

    using namespace std::placeholders;

    /**
     * @todo This is an absurd amount of parameters.  Move most, if
     *       not all, of the parameters to a structure that will be
     *       passed in as a single parameter instead.
     */
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
                       // unsigned char percent_complete,
                       std::size_t offset,
                       map_type<T> & map)
{
    // Clip datum to fit within map data type range, if necessary.
    double datum = 0;

    bool const found_data =
        (info.source().read_data(lat, lon, datum)
         && datum >= Map_traits<T>::minimum(info.minimum())
         && datum <= Map_traits<T>::maximum(info.maximum()));

    if (found_data) {
        T & data =
#ifdef NDEBUG
            map[offset];     // No bounds check.
#else
            map.at(offset);  // Perform bounds check.
#endif

        data = static_cast<T>(datum);
    }

    // Inform "observers" of mapping progress.
    info.notifier().notify_plotted(map.size());
}
