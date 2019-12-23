//   -*- C++ -*-
/**
 * @file OblateSpheroid.h
 *
 * Copyright (C) 1999, 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_OBLATE_SPHEROID_H
#define MARC_OBLATE_SPHEROID_H

#include <marc/BodyData.h>
#include <marc/Vector.h>
#include <marc/Export.h>


namespace MaRC
{
    /// Internal convenience type.
    using DVector = Vector<double, 3>;

    /**
     * @class OblateSpheroid OblateSpheroid.h <marc/OblateSpheroid.h>
     *
     * @brief Representation of a body modeled as an oblate spheroid.
     *
     * An oblate spheroid is an ellipsoidal body with potentially
     * different equatorial and polar radii.
     */
    class MARC_API OblateSpheroid final : public BodyData
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

        // Disallow moving.
        OblateSpheroid(OblateSpheroid &&) noexcept = delete;
        OblateSpheroid & operator=(OblateSpheroid &&) = delete;

        /// Destructor
        ~OblateSpheroid() override = default;

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
            return (this->eq_rad_ - this->pol_rad_) / this->eq_rad_;
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
        double centric_radius(double lat) const override;

        double centric_latitude(double latg) const override;

        double graphic_latitude(double lat) const override;

        double mu(double sub_observ_lat,
                  double sub_observ_lon,
                  double lat,
                  double lon,
                  double range) const override;

        double mu0(double sub_solar_lat,
                   double sub_solar_lon,
                   double lat,
                   double lon) const override;

        double cos_phase(double sub_observ_lat,
                         double sub_observ_lon,
                         double sub_solar_lat,
                         double sub_solar_lon,
                         double lat,
                         double lon,
                         double range) const override;
        //@}

        /// Radius of curvature of the meridian.
        /**
         * @param[in] lat Planetocentric latitude.
         *
         * @return Radius of curvature of the meridian.
         */
        double M(double lat);

        /// Radius of curvature in the direction of the prime vertical
        /// perpendicular to the direction of the meridian.
        /**
         * @param[in] lat Planetocentric latitude.
         *
         * @return Radius of curvature in the direction of the prime
         *         vertical perpendicular to the direction of the
         *         meridian.
         */
        double N(double lat);

        /**
         * @brief Intersection of tri-axial ellipsoid with line.
         *
         * \f$\vec{line} = \vec{vec} + k * \vec{dvec}\f$, where @e k
         * will computed.
         *
         * @param[in]  vec  Vector from ellipsoid center to observer.
         * @param[in]  dvec Vector along line.
         * @param[out] lat  Planetocentric latitude in radians.
         * @param[out] lon  Planetocentric east longitude in radians.
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
         * \f$e = \sqrt{1 - (\frac{polar\ radius}{equatorial\ radius})^2}\f$
         *
         * or:
         *
         * \f$e = \sqrt{1 - (1 - flattening)^2}\f$
         */
        double const first_eccentricity_;

  };

}


#endif  /* MARC_OBLATE_SPHEROID_H */
