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
#include <marc/extrema.h>

#include <vector>
#include <functional>
#include <cstdint>


namespace MaRC
{
    class SourceImage;
    template <typename T> class extrema;
    template <typename T> class plot_info;

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
         * @param[in] lat    Planetocentric latitude in radians.
         * @param[in] lon    Planetocentric longitude in radians.
         * @param[in] offset Map offset corresponding to the location
         *                   in the underlying map array where the
         *                   data will be plotted.
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
         * @tparam        T      Map element data type.
         * @param[in]     image  Image from which data to be
         *                       plotted to the map will be read.
         * @param[in]     minmax User-specified minimum and maximum
         *                       allowed physical data values on the
         *                       map.
         * @param[in,out] info   Map plotting information, such as
         *                       the number of samples and lines in
         *                       the map, the desired mininimum and
         *                       maximum allowed physical data
         *                       values, etc.  Some fields, such as
         *                       the minimum and maximum (not the
         *                       desired ones), may be updated to
         *                       reflect the actual values used when
         *                       creating the map.
         *
         * @return The generated map image.
         *
         * @note We rely on C++11 move semantics to avoid deep copying
         *       the returned map.
         */
        template <typename T>
        map_type<T> make_map(SourceImage const & image,
                             extrema<T> const & minmax,
                             plot_info<T> & info) const;

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
                            double lon_interval) const;

    private:

        /**
         * @class parameters
         *
         * @brief Group map parameters under a single object.
         *
         * The purpose of this internal class is to group map
         * parameters in one place to minimize the number of
         * arguments passed to the map plot() method.
         */
        template <typename T>
        class parameters
        {
        public:

            /**
             * @brief Constructor
             *
             * @param[in]     source Map source image.
             * @param[in]     minmax The minimum and maximum allowed
             *                       physical data values on the map,
             *                       i.e. data >= desired minimum and
             *                       data <= desired maximum.
             * @param[in,out] info   Map plotting information.
             * @param[in,out] map    Map image container.
             */
            parameters(SourceImage const & source,
                       extrema<T> const & minmax,
                       plot_info<T> & info,
                       map_type<T> & map)
                : source_(source)
                , minmax_(get_extrema(minmax))
                , info_(info)
                , map_(map)
            {
            }

            /// Get the map source image.
            auto const & source() const { return this->source_; }

            /// Get user-specified min/max map data values.
            auto const & minmax() const { return this->minmax_; }

            /// Get the map plotting information.
            auto & info() { return this->info_; }

            /// Get the map image container.
            auto & map() { return map_; }

        private:

            /**
             * @brief Get valid extrema.
             *
             * Obtain @c extrema<> object with the underlying minimum
             * and maximum both set with suitable values.  In
             * particular, if @a e.minimum() is not set, the returned
             * @c extrema<> will contain a minimum set to
             * @c std::numeric_limits<T>::lowest().  Similarly, the
             * maximum will be set to @c std::numeric_limits<T>::max()
             * if @a e.maximum() is not set.
             *
             * @param[in] e User-specified min/max map data values.
             *
             * @return Suitably initialized extrema.
             */
            MaRC::extrema<T> get_extrema(extrema<T> const & e);

        private:

            /// Map source image.
            SourceImage const & source_;

            /// User-specified allowed min/max map data values.
            extrema<T> const minmax_;

            /// Map plotting information.
            plot_info<T> & info_;

            /// Map image container.
            map_type<T> & map_;

        };

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
         * @tparam        T      Map element data type.
         * @param[in]     p      Map parameters.
         * @param[in]     lat    Planetocentric latitude in radians.
         * @param[in]     lon    Planetocentric longitude in radians.
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
         */
        template <typename T>
        void plot(parameters<T> & p,
                  double lat,
                  double lon,
                  std::size_t offset) const;

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
