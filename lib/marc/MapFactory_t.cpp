/**
 * @file MapFactory_t.cpp
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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

    T const actual_min = Map_traits<T>::minimum(info.desired_minimum());
    T const actual_max = Map_traits<T>::maximum(info.desired_maximum());
    info.desired_minimum(actual_min);
    info.desired_maximum(actual_max);

    map_type<T> map(samples * lines, blank);

    auto plot =
        [this, &info, &map](double lat, double lon, std::size_t offset)
        {
            this->plot(info, lat, lon, offset, map);
        };

    this->plot_map(samples, lines, plot);

    // Inform "observers" of map completion.
    info.notifier().notify_done(map.size());

    if (!info.data_mapped())
        map.clear();  // No data was mapped!

    return map;
}

template <typename T>
void
MaRC::MapFactory::plot(plot_info & info,
                       double lat,
                       double lon,
                       std::size_t offset,
                       map_type<T> & map)
{
    // Clip datum to fit within map data type range, if necessary.
    double datum = 0;

    bool const found_data =
        (info.source().read_data(lat, lon, datum)
         && datum >= info.desired_minimum()
         && datum <= info.desired_maximum());

    if (found_data) {
        map[offset] = static_cast<T>(datum);
        info.minimum(datum);
        info.maximum(datum);
    }


    // Inform "observers" of mapping progress.
    info.notifier().notify_plotted(map.size());
}


#endif  // MARC_MAP_FACTORY_T_CPP
