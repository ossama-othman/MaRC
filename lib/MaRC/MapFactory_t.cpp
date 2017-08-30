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

/**
 * @bug Exposing <MaRC/config.h> to the user pollutes the global
 *      namespace.
 */
#include "MaRC/config.h"      // For NDEBUG

#include <future>
#include <stdexcept>
#include <iostream>


template <typename T>
MaRC::MapFactory::map_type<T>
MaRC::MapFactory::make_map(SourceImage const & source,
                           std::size_t samples,
                           std::size_t lines,
                           double minimum,
                           double maximum)
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
                          std::cref(source),
                          minimum,
                          maximum,
                          _1,   // lat
                          _2,   // lon
                          _3,   // percent_complete
                          _4,   // map array offset
                          std::ref(map));

    this->plot_map(samples, lines, plot);

#if 0
    // Number of map tasks to spawn unless user specified.
    unsigned int tasks = std::thread::hardware_concurrency();

    // Concurrently plot the map across the specified number of
    // tasks.  Each task will plot an independent portion of the map.
    // For example, one task could plot half of the map, while another
    // task could map the other half in parallel.
    //
    // Concurrent map plotting is only performed and justified if the
    // size of the map is sufficiently large.

    auto begin = projection.begin(sample, lines);
    auto end   = projection.end  (sample, lines);

    auto const section_size = std::distance(begin, end) / tasks;

    std::list<std::future<void>> futures;
    for (unsigned int i = 0; i < tasks; ++i) {

        futures.emplace_back(
            std::async(std::launch::async,
                       [=]
                       {
                           // Plot the section of the map assigned to
                           // this task.

                       }));
    }

    // Wait for map plotting to complete.
    for (auto & f : futures) {
        /**
         * @todo Should we wait with timeout here?
         */
        f.wait();
    }
#endif

    return std::move(map);
}

template <typename T>
void
MaRC::MapFactory::plot(SourceImage const & source,
                       double minimum,
                       double maximum,
                       double lat,
                       double lon,
                       unsigned char percent_complete,
                       std::size_t offset,
                       map_type<T> & map)
{
    T & data =
#ifdef NDEBUG
        map[offset];     // No bounds check.
#else
        map.at(offset);  // Perform bounds check.
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
