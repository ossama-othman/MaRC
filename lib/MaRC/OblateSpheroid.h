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

#include <MaRC/Geometry.h>
#include <MaRC/BodyData.h>
#include <MaRC/Vector.h>

#include <cmath>


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
         * @param[out] lon  Bodycentric (e.g. planetocentric) east
         *                  longitude
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


  /************************************
   Inline functions (very heavily used)
   ************************************/

  // These can be inlined into PhotoImage
  // because PhotoImage contains an OblateSpheroid directly
  // not a reference or pointer

  inline double
  MaRC::OblateSpheroid::centric_radius (double lat) const
  {
    /*
      Given a bodycentric latitude and longitude for a point (x, y, z)
      on the surface of a spheroid:

          x = r * cos(lat) * cos(lon)
          y = r * cos(lat) * sin(lon)
          z = r * sin(lat)

      Assume longitude zero is along the observer optical axis, we
      have:

          x = r * cos(lat)
          y = 0
          z = r * sin(lat)

      The Cartesian equation for an oblate spheroid is:

           2    2    2
          x  + y    z
          ------- + -- = 1
             2       2
            a       c

      We end up with:

                       2                  2
         (r * cos(lat))  +  (r * sin(lat))
         ---------------    --------------- = 1
                 2                   2
                a                   c

      and:

                            1
        r = -----------------------------------
                             2               2
            sqrt((cos(lat)/a)  + (sin(lat)/c) )

    */
    return 1 / std::hypot(std::cos(lat) / this->eq_rad_,
                          std::sin(lat) / this->pol_rad_);
  }
  
  inline double
  MaRC::OblateSpheroid::centric_latitude (double latg) const
  {
    /*
                                  2
                    (polar radius)
      tan(lat) = -------------------- * tan(latg)
                                    2
                 (equatorial radius)
    */
    return
        std::atan(std::pow(this->pol_rad_ / this->eq_rad_, 2)
                  * std::tan(latg));
  }
  
  inline double
  MaRC::OblateSpheroid::graphic_latitude (double lat) const
  {
    /*
                                     2
                  (equatorial radius)
      tan(latg) = -------------------- * tan(lat)
                                   2
                     (polar radius)
    */
    return
        std::atan(std::pow(this->eq_rad_ / this->pol_rad_, 2)
                  * std::tan(lat));
  }
  
  inline double
  MaRC::OblateSpheroid::mu (double sub_observ_lat,
                            double sub_observ_lon,
                            double lat,
                            double lon,
                            double range) const
  {
    // Compute the local normal-observer angle - Emission Angle (Mu)
  
    const double latg = this->graphic_latitude (lat);
    const double ellipse_radius = this->centric_radius (lat);
  
    return ((range * ::sin (sub_observ_lat) * ::sin (latg) - ellipse_radius *
  	   ::cos (lat - latg)) + range * ::cos (sub_observ_lat) *
  	  ::cos (latg) * ::cos (sub_observ_lon - lon)) /
      // dot product (above)
      // divided by the magnitude of vector
      // from observer to point on body
      ::sqrt (range * range + ellipse_radius * ellipse_radius -
              2 * range * ellipse_radius *
              (::sin (sub_observ_lat) * ::sin (lat) +
               ::cos (sub_observ_lat) *
               ::cos (lat) * ::cos (sub_observ_lon - lon)));
  }
  
  inline double
  MaRC::OblateSpheroid::mu0 (double sub_solar_lat,
                             double sub_solar_lon,
                             double lat,
                             double lon) const
  {
    // Compute the sun-local normal angle - Incidence Angle (Mu0)
  
    const double latg = this->graphic_latitude (lat);
  
    return ::sin (sub_solar_lat) * ::sin (latg) +
      ::cos (sub_solar_lat) * ::cos (latg) * ::cos (sub_solar_lon - lon);
    // The above equation assumes the sun to be an infinite distance away.
  }

} /* namespace MaRC */


#endif  /* MARC_OBLATE_SPHEROID_H */
