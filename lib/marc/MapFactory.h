// -*- C++ -*-
/**
 * @file MapFactory.h
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

#ifndef MARC_MAP_FACTORY_H
#define MARC_MAP_FACTORY_H

#include <marc/Export.h>

#include <vector>
#include <limits>
#include <functional>
#include <cstdint>


namespace MaRC
{
    class SourceImage;
    class plot_info;

    /**
     * @class MapFactory
     *
     * @brief Map Abstract Factory
     *
     * Abstract Factory class for map projections supported by MaRC.
     */
    class MARC_API MapFactory
    {
    public:

        /// @typedef Type returned from @c make_map() method.
        template <typename T>
        using map_type = std::vector<T>;

        /// @typedef Type returned from @c make_grid() method.
        using grid_type = std::vector<uint8_t>;

        /**
         * Map plot functor type.
         *
         * Concrete map factories will call a function of this type in
         * their @c plot_map() implementation.
         *
         * @see @c plot() for parameter details.
         *
         */
        using plot_type =
            std::function<void(double lat,
                               double lon,
                               std::size_t offset)>;

        /// Constructor.
        MapFactory() = default;

        // Disallow copying.
        MapFactory(MapFactory const &) = delete;
        MapFactory & operator=(MapFactory const &) = delete;

        /// Destructor.
        virtual ~MapFactory() = default;

        /// Return the name of the map projection.
        virtual char const * projection_name() const = 0;

        /// Create the desired map projection.
        /**
         * This method takes care of allocating and initializing the
         * underlying map array, and delegates actual mapping to the
         * subclass implementation of @c plot_map().
         *
         * @param[in,out] info    Map plotting information, such as
         *                        the source image, min/max allowed
         *                        data values, etc.  Some fields, such
         *                        as the minimum and maximum, may be
         *                        updated to reflect the actual values
         *                        used when creating the map.
         * @param[in]     samples Number of samples in map.
         * @param[in]     lines   Number of lines   in map.
         *
         * @return The generated map image.
         *
         * @note We rely on C++11 move semantics to avoid deep copying
         *       the returned map.
         */
        template <typename T>
        map_type<T> make_map(plot_info & info,
                             std::size_t samples,
                             std::size_t lines);

        /// Create the latitude/longitude grid for the desired map
        /// projection.
        /**
         * This method takes care of allocating and initializing the
         * underlying grid array, and delegates actual grid generation
         * to the subclass implementation of @c plot_grid().
         *
         * Blank grid elements will contain a value of zero, otherwise
         * they will contain the maximum 8 bit unsigned integer value,
         * i.e. 255.
         *
         * @param[in] samples      Number of samples in grid.
         * @param[in] lines        Number of lines   in grid.
         * @param[in] lat_interval Number of degrees between each
         *                         latitude grid line.
         * @param[in] lon_interval Number of degrees between each
         *                         longitude grid line.
         *
         * @return The generated grid image.
         *
         * @note We rely on C++11 move semantics to avoid deep copying
         *       the returned grid.
         */
        grid_type make_grid(std::size_t samples,
                            std::size_t lines,
                            double lat_interval,
                            double lon_interval);

    private:

        /**
         * Create the desired map projection.
         *
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         * @param[in] plot    Functor to be called when plotting data
         *                    on the map.
         */
        virtual void plot_map(std::size_t samples,
                              std::size_t lines,
                              plot_type plot) const = 0;

        /// Plot the data on the map.
        /**
         * Plot the data at given latitude and longitude on the map.
         * Map implementation end up calling this function indirectly
         * through a function object that shields the caller from most
         * of these parameters.
         *
         * @see @c plot_type type alias
         * @see @c plot_map()
         *
         * @param[in]     source           SourceImage object
         *                                 containing the data to be
         *                                 mapped.
         * @param[in]     minimum          Minimum allowed value on
         *                                 map, i.e. all data greater
         *                                 than or equal to
         *                                 @a minimum.
         * @param[in]     maximum          Maximum allowed value on
         *                                 map, i.e. all data less
         *                                 than or equal to
         *                                 @a maximum.
         * @param[in]     lat              Planetocentric latitude.
         * @param[in]     lon              Planetocentric longitude.
         * @param[in]     percent_complete Percent of map completed.
         * @param[in]     offset           Map offset corresponding to
         *                                 the location in the
         *                                 underlying map array where
         *                                 the data will be plotted.
         * @param[in,out] map              Map container where data
         *                                 will be plotted.
         *
         * @todo Currently subclasses must call this method in their
         *       @c plot_map() implementation.  That seems like a
         *       rather roundabout way of mapping the data.  Ideally
         *       @c make_map() should handle the map array iteration
         *       as well as calling this @c plot() method.
         */
        template <typename T>
        void plot(plot_info const & info,
                  double lat,
                  double lon,
                  std::size_t offset,
                  map_type<T> & map);

        /// Create the latitude/longitude grid for the desired map
        /// projection.
        /**
         * @param[in]     samples      Number of samples in grid.
         * @param[in]     lines        Number of lines   in grid.
         * @param[in]     lat_interval Number of degrees between each
         *                             latitude grid line.
         * @param[in]     lon_interval Number of degrees between each
         *                             longitude grid line.
         * @param[in,out] grid         Grid container to be populated
         *                             with grid data.  The caller
         *                             owns the storage.  Subclass
         *                             implementations should only
         *                             populate the grid with data.
         */
        virtual void plot_grid(std::size_t samples,
                               std::size_t lines,
                               double lat_interval,
                               double lon_interval,
                               grid_type & grid) const = 0;

    };

}


#include "marc/MapFactory_t.cpp"

#endif  // MARC_MAP_FACTORY_H
