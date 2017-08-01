// -*- C++ -*-
/**
 * @file MapFactory.h
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

#ifndef MARC_MAP_FACTORY_H
#define MARC_MAP_FACTORY_H

#include <vector>
#include <limits>


namespace MaRC
{
    class SourceImage;

    class MapFactoryBase
    {
    public:

        /// @typedef Type returned from @c make_grid() method.
        using grid_type = std::vector<unsigned char>;

        /// Create the latitude/longitude grid for the desired map
        /// projection.
        /**
         * This method takes care of allocating and initializing the
         * underlying grid array, and delegates actual grid generation
         * to the subclass implementation of @c plot_grid().
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
                            float lat_interval,
                            float lon_interval)
        {
            grid_type grid(samples * lines,
                           grid_type::value_type());

            this->plot_grid(samples,
                            lines,
                            lat_interval,
                            lon_interval,
                            grid);

            return grid;
        }

    private:

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
                               float lat_interval,
                               float lon_interval,
                               grid_type & grid) = 0;

    };

    /**
     * @class MapFactory
     *
     * @brief Map Abstract Factory
     *
     * Abstract Factory class for map projections supported by MaRC.
     */
    template <typename T>
    class MapFactory : private MapFactoryBase
    {
    public:

        /// @typedef Type of data to be stored in map.
        typedef T        data_type;

        /// @typedef Type returned from @c make_map() method.
        using map_type = std::vector<T>;

        // Bring @c MapFactoryBase::make_grid() in this public scope.
        using MapFactoryBase::grid_type;
        using MapFactoryBase::make_grid;

        /// Constructor.
        MapFactory();

        // Disallow copying.
        MapFactory(MapFactory const &) = delete;
        MapFactory & operator=(MapFactory const &) = delete;

        /// Destructor.
        virtual ~MapFactory();

        /// Return the name of the map projection.
        virtual char const * projection_name() const = 0;

        /// Create the desired map projection.
        /**
         * This method takes care of allocating and initializing the
         * underlying map array, and delegates actual mapping to the
         * subclass implementation of @c plot_map().
         *
         * @param[in] source  SourceImage object containing the data
         *                    to be mapped.
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         * @param[in] minimum Minimum allowed value map, i.e. all data
         *                    greater than @ a minimum.
         * @param[in] maximum Maximum allowed value map, i.e. all data
         *                    less than @a maximum.
         *
         * @return The generated map image via C++11 move semantics.
         *
         * @note We rely on C++11 move semantics to avoid deep copying
         *       the returned map.
         */
        map_type make_map(SourceImage const & source,
                          std::size_t samples,
                          std::size_t lines,
                          double minimum,
                          double maximum);

    private:

        /**
         * Create the desired map projection.
         *
         * @param[in]     source  SourceImage object containing the
         *                        data to be mapped.
         * @param[in]     samples Number of samples in map.
         * @param[in]     lines   Number of lines   in map.
         * @param[in]     minimum Minimum allowed value map, i.e. all
         *                        data greater than @ a minimum.
         * @param[in]     maximum Maximum allowed value map, i.e. all
         *                        data less than @a maximum.
         * @param[in,out] map     Map container to be populated with
         *                        map data.  The caller owns the
         *                        storage.  Subclass implementations
         *                        should only populate the map with
         *                        data.
         */
        virtual void plot_map(SourceImage const & source,
                              std::size_t samples,
                              std::size_t lines,
                              double minimum,
                              double maximum,
                              map_type & map) = 0;

    protected:

        /// Plot the data on the map.
        /**
         * Plot the data at given latitude and longitude on the map.
         *
         * @param[in] source           SourceImage object containing
         *                             the datato be mapped.
         * @param[in] lat              Bodycentric latitude
         * @param[in] lon              Bodycentric longitude
         * @param[in] minimum          Minimum allowed value map,
         *                             i.e. all data greater than
         *                             @a minimum.
         * @param[in] maximum          Maximum allowed value map,
         *                             i.e. all data less than
         *                             @a maximum.
         * @param[in] percent_complete Percent of map completed
         * @param[out] data            Data retrieved from source
         *                             image.
         *
         * @todo Currently subclasses must call this method in their
         *       @c plot_map() implementation.  That seems like a
         *       rather roundabout way of mapping the data.  Ideally
         *       @c make_map() should handle the map array iteration
         *       as well as calling this @c plot() method.
         */
        void plot(SourceImage const & source,
                  double lat,
                  double lon,
                  double minimum,
                  double maximum,
                  unsigned char percent_complete,
                  T & data);

    private:

        /**
         * Previously measured percentage of map completed.
         *
         * @todo Look into a better way of allow the MaRC program to
         *       track mapping progress.
         */
        unsigned char percent_complete_old_;

    };

}


#include "MaRC/MapFactory.cpp"

#endif
