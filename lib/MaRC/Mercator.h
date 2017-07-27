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
#include <MaRC/OblateSpheroid.h>


namespace MaRC
{
    class BodyData;

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
    template <typename T>
    class Mercator : public MapFactory<T>
    {
    public:

        /// @typedef Type returned from @c make_map() method.
        using map_type = typename MapFactory<T>::map_type;

        /// @typedef Type returned from @c make_grid() method.
        using grid_type = typename MapFactory<T>::grid_type;

        /// Constructor.
        /**
         * @param[in,out] body Pointer to BodyData object representing
         *                     body being mapped.
         */
        Mercator(std::unique_ptr<OblateSpheroid> body);

        /// Destructor
        ~Mercator();

        /**
         * @name @c MapFactory Methods
         *
         * Factory methods required by the @c MapFactory abstract base
         * class.
         *
         * @see @c MapFactory
         */
        //@{
        virtual const char * projection_name (void) const;
        virtual map_type make_map (SourceImage const & source,
                                   std::size_t samples,
                                   std::size_t lines,
                                   double minimum,
                                   double maximum);
        virtual grid_type make_grid(std::size_t samples,
                                    std::size_t lines,
                                    float lat_interval,
                                    float lon_interval);
        //@}

    private:

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

        /// BodyData object representing the body being mapped.
        std::unique_ptr<OblateSpheroid> const body_;

    };

}


#include "MaRC/Mercator.cpp"

#endif
