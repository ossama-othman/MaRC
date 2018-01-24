//   -*- C++ -*-
/**
 * @file OblateSpheroid.h
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
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

#ifndef MARC_OBLATE_SPHEROID_H
#define MARC_OBLATE_SPHEROID_H

#include <MaRC/BodyData.h>
#include <MaRC/Vector.h>
#include <MaRC/Export.h>



namespace MaRC
{
    typedef Vector<double, 3> DVector;

    /**
     * @class OblateSpheroid
     *
     * @brief Representation of a body modeled as an oblate spheroid.
     *
     * An oblate spheroid is an ellipsoidal body with potentially
     * different equatorial and polar radii.
     */
    class MARC_API OblateSpheroid : public BodyData
    {
    public:

        /// Constructor
        /**
         * @param[in] prograde   Flag that states whether body
         *                       rotation is prograde or retrograde.
         * @param[in] eq_rad     Equatorial radius in kilometers.
         * @param[in] pol_rad    Polar radius in kilometers.
         *
         * @note @a eq_rad must be greater than or equal to
         *       @a pol_rad.
         */
        OblateSpheroid(bool prograde, double eq_rad, double pol_rad);

        // Disallow copying.
        OblateSpheroid(OblateSpheroid const &) = delete;
        OblateSpheroid & operator=(OblateSpheroid const &) = delete;

        /// Destructor
        virtual ~OblateSpheroid() = default;

        /// Get the equatorial radius of the body.
        double eq_rad() const
        {
            return this->eq_rad_;
        }

        /// Get the polar radius of the body.
        double pol_rad() const
        {
            return this->pol_rad_;
        }

        /// Get the first eccentricity.
        double first_eccentricity() const
        {
            return this->first_eccentricity_;
        }

        /**
         * @name MaRC::BodyData Virtual Methods
         *
         * Methods required by the MaRC::BodyData abstract base
         * class.
         */
        //@{
        virtual double centric_radius(double lat) const;

        virtual double centric_latitude(double latg) const;

        virtual double graphic_latitude(double lat) const;

        virtual double mu(double sub_observ_lat,
                          double sub_observ_lon,
                          double lat,
                          double lon,
                          double range) const;

        virtual double mu0(double sub_solar_lat,
                           double sub_solar_lon,
                           double lat,
                           double lon) const;

        virtual double cos_phase(double sub_observ_lat,
                                 double sub_observ_lon,
                                 double sub_solar_lat,
                                 double sub_solar_lon,
                                 double lat,
                                 double lon,
                                 double range) const;
        //@}

        /// Radius of curvature of the meridian.
        /**
         * @param[in] lat Bodycentric latitude
         *
         * @return Radius of curvature of the meridian.
         */
        double M(double lat);

        /// Radius of curvature in the direction of the prime vertical
        /// perpendicular to the direction of the meridian.
        /**
         * @param[in] lat Bodycentric latitude
         *
         * @return Radius of curvature in the direction of the prime
         *         vertical perpendicular to the direction of the
         *         meridian.
         */
        double N(double lat);

        /// Intersection of tri-axial ellipsoid with line.
        /**
         * Line = vec + k * dvec, where k will computed.
         *
         * @param[in]  vec  Vector from ellipsoid center to observer
         * @param[in]  dvec Vector along line
         * @param[out] lat  Bodycentric (e.g. planetocentric) latitude
         *                  in radians
         * @param[out] lon  Bodycentric (e.g. planetocentric) east
         *                  longitude in radians
         *
         * @retval  0 ellipse intersection was found
         * @retval  1 no ellipse intersection was found
         * @retval -1 bad inputs
         *
         * @note Longitude is east longitude.
         * @note Zero longitude is at x = 0, y < 0, y is increasing
         *       moving away from observer.
         */
        int ellipse_intersection(DVector const & vec,
                                 DVector const & dvec,
                                 double & lat,
                                 double & lon) const;

    private:

        /// Equatorial radius (kilometers).
        double const eq_rad_;

        /// Polar radius (kilometers).
        double const pol_rad_;

        /// First eccentricity.
        /**
         * A measure of the oblate spheroid's deviation from being
         * spherical.
         *
         * e = sqrt(1 - (pol_rad / eq_rad) ^ 2)
         *
         * or:
         *
         * e = sqrt(1 - (1 - flattening) ^ 2)
         */
        double const first_eccentricity_;

  };

}


#endif  /* MARC_OBLATE_SPHEROID_H */
