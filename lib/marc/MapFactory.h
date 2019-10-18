// -*- C++ -*-
/**
 * @file MapFactory.h
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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
     * @class MapFactory MapFactory.h <marc/MapFactory.h>
     *
     * @brief Map Abstract Factory
     *
     * Abstract Factory class for map projections supported by %MaRC.
     */
    class MARC_API MapFactory
    {
    public:

        /// Type returned from @c make_map() method.
        template <typename T>
        using map_type = std::vector<T>;

        /// Type returned from @c make_grid() method.
        using grid_type = std::vector<std::uint8_t>;

        /**
         * @brief Map plot functor type.
         *
         * Concrete map factories will call a function of this type in
         * their @c plot_map() implementation.
         *
         * @see @c plot()
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

        // Disallow moving.
        MapFactory(MapFactory &&) noexcept = delete;
        MapFactory & operator=(MapFactory &&) = delete;

        /// Destructor.
        virtual ~MapFactory() = default;

        /// Return the name of the map projection.
        virtual char const * projection_name() const = 0;

        /**
         * @brief Create the map projection.
         *
         * This method takes care of allocating and initializing the
         * underlying map array, and delegates actual mapping to the
         * subclass implementation of @c plot_map().
         *
         * @tparam        T       Map element data type.
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

        /**
         * @brief Create the latitude/longitude grid for the map
         *        projection.
         *
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
         * @brief Create the desired map projection.
         *
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         * @param[in] plot    Functor to be called when plotting data
         *                    on the map.
         */
        virtual void plot_map(std::size_t samples,
                              std::size_t lines,
                              plot_type const & plot) const = 0;

        /**
         * @brief Plot the data on the map.
         *
         * Plot the data at given latitude and longitude on the map.
         * Map implementation end up calling this function indirectly
         * through a function object that shields the caller from most
         * of these parameters.
         *
         * @see @c plot_type
         * @see @c plot_map()
         *
         * @tparam        T       Map element data type.
         * @param[in]     info   Map plotting information.
         * @param[in]     lat    Planetocentric latitude.
         * @param[in]     lon    Planetocentric longitude.
         * @param[in]     offset Map offset corresponding to the
         *                       location in the underlying map array
         *                       where the data will be plotted.
         * @param[in,out] map    Map container where data will be
         *                       plotted.
         *
         * @todo Currently subclasses must call this method in their
         *       @c plot_map() implementation.  That seems like a
         *       rather roundabout way of mapping the data.  Ideally
         *       @c make_map() should handle the map array iteration
         *       as well as calling this @c plot() method.
         *
         * @todo This method has too many parameters.  Move most,
         *       if not all, of the parameters to a structure that
         *       will be passed in as a single parameter instead.
         */
        template <typename T>
        void plot(plot_info const & info,
                  double lat,
                  double lon,
                  std::size_t offset,
                  map_type<T> & map);

        /**
         * @brief Plot latitude/longitude grid for the map.
         *
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
