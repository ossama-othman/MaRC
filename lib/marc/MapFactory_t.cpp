/**
 * @file MapFactory_t.cpp
 *
 * Copyright (C) 2003-2004, 2017-2018, 2024  Ossama Othman
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
MaRC::extrema<T>
MaRC::MapFactory::parameters<T>::get_extrema(extrema<T> const & e)
{
    auto const & minimum = e.minimum();
    auto const & maximum = e.maximum();

    extrema<T> ex(minimum ? *minimum : std::numeric_limits<T>::lowest(),
                  maximum ? *maximum : std::numeric_limits<T>::max());

    return ex;
}

// -----------------------------------------------------------------------

template <typename T>
MaRC::MapFactory::map_type<T>
MaRC::MapFactory::make_map(SourceImage const & image,
                           extrema<T> const & minmax,
                           plot_info<T> & info) const
{
    // Set up blank value for the map.
    auto blank = Map_traits<T>::empty_value();

    if (std::is_integral<T>::value && info.blank()) {
        if (info.blank() < std::numeric_limits<T>::lowest()
            || info.blank() > std::numeric_limits<T>::max()) {
            throw std::invalid_argument("Blank map value does not fit "
                                        "within map data type.");
        }

        blank = static_cast<T>(*info.blank());
    }

    // Set up physical data value extrema.
    map_type<T> map(info.samples() * info.lines(), blank);

    // Begin mapping.
    parameters<T> p(image, minmax, info, map);

    auto plot =
        [this, &p](double lat, double lon, std::size_t offset)
        {
            this->plot(p, lat, lon, offset);
        };

    this->plot_map(info.samples(), info.lines(), plot);

    // Inform "observers" of map completion.
    info.notifier().notify_done(map.size());

    return map;
}

template <typename T>
void
MaRC::MapFactory::plot(parameters<T> & p,
                       double lat,
                       double lon,
                       std::size_t offset) const
{
    auto const & source = p.source();
    auto const & e      = p.minmax();
    auto       & info   = p.info();
    auto       & map    = p.map();

    // Clip datum to fit within map data type range, if necessary.
    double datum = 0;

    bool const found_data =
        (source.read_data(lat, lon, datum) && e.in_range(datum));

    if (found_data) {
        map[offset] = static_cast<T>(datum);
        info.update_extrema(map[offset]);
    }

    /**
     * @todo Should we only notify observers if data was actually
     *       plotted?
     */
    // Inform "observers" of mapping progress.
    info.notifier().notify_plotted(map.size());
}


#endif  // MARC_MAP_FACTORY_T_CPP
