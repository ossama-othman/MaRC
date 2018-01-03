// -*- C++ -*-
/**
 * @file Mercator.h
 *
 * Copyright (C) 1999, 2004, 2017  Ossama Othman
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

#ifndef MARC_MERCATOR_H
#define MARC_MERCATOR_H

#include <MaRC/MapFactory.h>

#include <memory>


namespace MaRC
{
    class OblateSpheroid;

    /**
     * @class Mercator
     *
     * @brief Tranverse Mercator concrete map factory.
     *
     * A Transverse Mercator map contains data mapped in a way that
     * retains shape, i.e. the projection is conformal.  The
     * "transverse" basically means that latitudes and longitudes are
     * vertical and horizontal lines, respectively.
     *
     * @note This implementation can only map oblate spheroids or
     *       spheres.
     */
    class MARC_API Mercator : public MapFactory
    {
    public:

        /// @typedef Type returned from @c make_grid() method.
        using typename MapFactory::grid_type;

        /// @typedef Type of functor passed to @c plot_map() method.
        using typename MapFactory::plot_type;

        /**
         * @brief Default maximum latitude to map.
         *
         * If no maximum latitude is supplied this will be the maximum
         * latitude in degrees to map.  For example, a maximum
         * latitude of 84 will result in a map projection containing
         * latitudes between -84 and 84, inclusive.  The maximum
         * latitude must be less than 90 since it is not possible to
         * map the poles in this map projection.
         */
        static constexpr double default_max_lat = 84;

        /// Constructor.
        /**
         * @param[in] body    Pointer to @c OblateSpheroid object
         *                    representing body being mapped.
         * @param[in] max_lat Maximum bodyCENTRIC latitude to map in
         *                    degrees.
         */
        Mercator(std::shared_ptr<OblateSpheroid> body,
                 double max_lat);

        /// Destructor
        virtual ~Mercator() = default;

        /**
         * @name @c MapFactory Methods
         *
         * Factory methods required by the @c MapFactory abstract base
         * class.
         *
         * @see @c MapFactory
         */
        //@{
        virtual char const * projection_name() const;
        //@}

    private:

        /**
         * Create the Mercator map projection.
         *
         * @see @c MaRC::MapFactory::plot_map().
         */
        virtual void plot_map(std::size_t samples,
                              std::size_t lines,
                              plot_type plot) const;

        /**
         * Create the Mercator map latitude/longitude grid.
         *
         * @see @c MaRC::MapFactory::plot_grid().
         */
        virtual void plot_grid(std::size_t samples,
                               std::size_t lines,
                               float lat_interval,
                               float lon_interval,
                               grid_type & grid) const;

        /// Orient longitude according to rotation direction
        /// (prograde/retrograde).
        /**
         * @param[in] i       Sample in map being mapped.
         * @param[in] samples Number of samples in image.
         *
         * @return Longitude at given sample @a i.
         */
        double get_longitude(std::size_t i, std::size_t samples) const;

        /// The underlying Transverse Mercator projection equation.
        /**
         * @param[in] latg Bodygraphic latitude.
         *
         * @return Value of point on projection along a vertical axis
         *         (e.g. along a longitude line).
         */
        double mercator_x(double latg) const;

        /// Scale distortion at given bodygraphic latitude @a latg on
        /// map.
        /**
         * @param[in] latg Bodygraphic latitude.
         */
        double distortion(double latg) const;

    private:

        /// @c OblateSpheroid object representing the body being
        /// mapped.
        std::shared_ptr<OblateSpheroid> const body_;

        /**
         * @brief Range of bodyCENTRIC latitudes to map in radians.
         *
         * The latitude range is currently defined as the difference
         * between the highest and lowest latitude to be mapped.  For
         * example, given a maximum latitude of 84 degrees, the
         * latitude range will be 168 degrees:
         * @code
         *     84 - (-84) = 84 * 2 = 168
         * @endcode
         *
         * This is value is in radians.
         */
        double const lat_range_;

    };

}


#endif
