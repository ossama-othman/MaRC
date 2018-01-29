// -*- C++ -*-
/**
 * @file Mercator.h
 *
 * Copyright (C) 1999, 2004, 2017-2018  Ossama Othman
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
     * @brief Mercator concrete map factory.
     *
     * A Mercator map contains data mapped in a way that retains
     * shape, i.e. the projection is conformal.  It also retains true
     * direction (unlike its transverse variant).
     *
     * @note A maximum latitude is currently not accepted as an option
     *       since the full 360 longitude range is mapped across the
     *       entire width of the generated map.  The maximum latitude
     *       is determined by the number of lines in the map.  The
     *       greater the number of lines in the map, the greater the
     *       latitude range in the map.
     *
     * @note This implementation can only map oblate spheroids or
     *       spheres.
     */
    class MARC_API Mercator final : public MapFactory
    {
    public:

        /// @typedef Type returned from @c make_grid() method.
        using typename MapFactory::grid_type;

        /// @typedef Type of functor passed to @c plot_map() method.
        using typename MapFactory::plot_type;

        /// Constructor.
        /**
         * @param[in] body    Pointer to @c OblateSpheroid object
         *                    representing body being mapped.
         */
        Mercator(std::shared_ptr<OblateSpheroid> body);

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

        /// Scale distortion at given bodygraphic latitude @a latg on
        /// map.
        /**
         * @param[in] latg Bodygraphic latitude.
         */
        double distortion(double latg) const;

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
                               double lat_interval,
                               double lon_interval,
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

    private:

        /// @c OblateSpheroid object representing the body being
        /// mapped.
        std::shared_ptr<OblateSpheroid> const body_;

    };

}


#endif
