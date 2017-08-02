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
    class OblateSpheroid : public BodyData
    {
    public:

        /// Constructor
        /**
         * @param[in] prograde   Flag that states whether body
         *                       rotation is prograde or retrograde.
         * @param[in] eq_rad     Equatorial radius in kilometers.
         * @param[in] pol_rad    Polar radius in kilometers.
         * @param[in] flattening Flattening ((eq_rad-pol_rad)/eq_rad).
         */
        OblateSpheroid(bool prograde,
                       double eq_rad,
                       double pol_rad,
                       double flattening);

        // Disallow copying.
        OblateSpheroid(OblateSpheroid const &) = delete;
        OblateSpheroid & operator=(OblateSpheroid const &) = delete;

        /// Destructor
        virtual ~OblateSpheroid();

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

        /// Get the flattening.
        double flattening() const
        {
            return this->flattening_;
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

        virtual double centric_latitude(double lat) const;

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

        virtual double M(double lat);

        virtual double N(double lat);
        //@}

        /// Intersection of tri-axial ellipsoid with line.
        /**
         * Line = vec + k * dvec, where k will computed.
         *
         * @param[in]  vec  Vector from ellipsoid center to observer
         * @param[in]  dvec Vector along line
         * @param[out] lat  Bodycentric (e.g. planetocentric) latitude
         * @param[out] lon  Bodycentric (e.g. planetocentric) east
         *                  longitude
         *
         * @return Zero if ellipse intersection found, non-zero otherwise.
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

        /// Initialize equatorial radius, polar radius and flattening
        /// using at least two of the given parameters.
        /**
         * @param[in] eq_rad     Equatorial radius in kilometers.
         * @param[in] pol_rad    Polar radius in kilometers.
         * @param[in] flattening Flattening ((eq_rad-pol_rad)/eq_rad).
         */
        void initialize_radii(double eq_rad,
                              double pol_rad,
                              double flattening);

    private:

        /// Equatorial radius (kilometers).
        double eq_rad_;

        /// Polar radius (kilometers).
        double pol_rad_;

        /// Flattening.
        /**
         * flattening = (eq_rad - pol_rad) / eq_rad
         *
         *     flattening == 0 : Sphere
         * 0 < flattening <  1 : Oblate Spheroid
         *
         *     flattening <  0 : eq_rad < pol_rad : INVALID
         * 1 < flattening      : pol_rad < 0      : INVALID
         *     flattening == 1 : Disc             : INVALID
         */
        double flattening_;

        /// First eccentricity.
        /**
         * (1 - (1 - flattening) ^ 2)
         */
        double first_eccentricity_;

  };

}


#endif  /* MARC_OBLATE_SPHEROID_H */
