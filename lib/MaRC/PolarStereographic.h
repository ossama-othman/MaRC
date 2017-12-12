// -*- C++ -*-
/**
 * @file PolarStereographic.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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

#ifndef MARC_POLAR_STEREOGRAPHIC_H
#define MARC_POLAR_STEREOGRAPHIC_H

#include <MaRC/MapFactory.h>
#include <MaRC/OblateSpheroid.h>


namespace MaRC
{

    /**
     * @class PolarStereographic
     *
     * @brief Polar Stereographic concrete map factory.
     *
     * A Polar Stereographic map contains data mapped in a way that
     * retains shape, i.e. the projection is conformal.  One of the
     * poles will be at the center of the map.
     *
     * @note This implementation can only map oblate spheroids or
     *       spheres.
     */
    template <typename T>
    class PolarStereographic : public MapFactory<T>
    {
    public:

        /// @typedef Type returned from @c make_grid() method.
        typedef typename MapFactory<T>::grid_type grid_type;

        /// @typedef Type of functor passed to @c plot_map() method.
        using typename MapFactory<T>::plot_type;

        /// Constructor.
        /**
         * @param[in] body       Pointer to BodyData object
         *                       representing body being mapped.
         * @param[in] max_lat    Maximum bodyCENTRIC latitude to map
         *                       in degrees.
         * @param[in] north_pole Place north_pole if @c true.  South
         *                       pole otherwise.
         */
        PolarStereographic(std::shared_ptr<OblateSpheroid> body,
                           double max_lat,
                           bool north_pole);

        /// Destructor
        virtual ~PolarStereographic() = default;

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
         * Create the Polar Stereographic map projection.
         *
         * @see @c MaRC::MapFactory<T>::plot_map().
         */
        virtual void plot_map(std::size_t samples,
                              std::size_t lines,
                              plot_type plot) const;

        /**
         * Create the Polar Stereographic map latitude/longitude
         * grid.
         *
         * @see @c MaRC::MapFactoryBase::plot_grid().
         */
        virtual void plot_grid(std::size_t samples,
                               std::size_t lines,
                               float lat_interval,
                               float lon_interval,
                               grid_type & grid) const;

        /// The underlying Polar Stereographic projection equation.
        /**
         * @param[in] latg Bodygraphic latitude.
         *
         * @return Value of point on projection along a radial line
         *         (e.g. along a longitude line).
         */
        double stereo_rho(double latg) const;

        /// Scale distortion at given bodygraphic latitude @a latg on
        /// map.
        /**
         * @param[in] latg Bodygraphic latitude.
         */
        double distortion(double latg) const;

    private:

        /// BodyData object representing the body being mapped.
        std::shared_ptr<OblateSpheroid> const body_;

        /// Maximum bodyCENTRIC latitude to map in radians.
        double const max_lat_;

        /// Coefficient used in map equation.
        double const rho_coeff_;

        /// Coefficient used in scale distortion equation.
        double const distortion_coeff_;

        /// @c true if north pole is at center of map.  @c false if
        /// south pole is at center.
        bool const north_pole_;

    };

}


#include "MaRC/PolarStereographic.cpp"

#endif
