//   -*- C++ -*-
/**
 * @file BodyData.h
 *
 * Copyright (C) 1999, 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_BODY_DATA_H
#define MARC_BODY_DATA_H

#include <marc/Export.h>


namespace MaRC
{

    /**
     * @class BodyData BodyData.h <marc/BodyData.h>
     *
     * @brief Class representing the body under observation.
     *
     * Concrete @c BodyData implementations must subclass this
     * abstract base class.
     *
     * @todo @c BodyData class methods that only have a latitude
     *       should really have a longitude parameter as well.
     *       Without a longitude parameter only bodies that are
     *       symmetrical about their polar axis can be implemented as
     *       subclasses.
     */
    class MARC_API BodyData
    {
    public:

        /// Constructor
        /**
         * @param[in] prograde Flag that states whether body rotation
         *                     is prograde or retrograde.
         */
        constexpr explicit BodyData(bool prograde) noexcept
            : prograde_(prograde)
        {
        }

        // Disallow copying.
        BodyData(BodyData const &) = delete;
        BodyData & operator=(BodyData const &) = delete;

        // Disallow moving.
        BodyData(BodyData &&) noexcept = delete;
        BodyData & operator=(BodyData &&) = delete;

        /// Destructor
        virtual ~BodyData() = default;

        /// Return the direction of rotation.
        /**
         * @return @c true if the current rotation is prograde.
         */
        bool prograde() const { return this->prograde_; }

        /// Find radius as a function of planetocentric latitude.
        /**
         * Calculate and return the radius, i.e. distance from the
         * center of the body to the point on the surface at given
         * latitude.
         *
         * @param[in] lat Planetocentric latitude in radians.
         *
         * @return Radius, i.e. distance from the center of the body
         *         to point on the surface at given latitude.
         *
         * @todo We really should add a longitude parameter since not
         *       all bodies are symmetrical about their polar axis.
         *
         */
        virtual double centric_radius(double lat) const = 0;

        /// Convert from planeteographic to planetocentric latitude.
        /**
         * @param[in] latg Planetographic latitude in radians.
         *
         * @return Planetocentric latitude in radians.
         *
         * @todo We really should add a longitude parameter since not
         *       all bodies are symmetrical about their polar axis.
         *
         */
        virtual double centric_latitude(double latg) const = 0;

        /// Convert from planetocentric to planetographic latitude.
        /**
         * @param[in] lat Planetocentric latitude in radians.
         *
         * @return Planetographic latitude in radians.
         *
         * @todo We really should add a longitude parameter since not
         *       all bodies are symmetrical about their polar axis.
         */
        virtual double graphic_latitude(double lat) const = 0;

        /// Return cosine of emission angle, i.e. &mu;
        /**
         * @param[in] sub_observ_lat Planetocentric sub-observation
         *                           latitude.
         * @param[in] sub_observ_lon Sub-Observation longitude.
         * @param[in] lat            Planetocentric latitude.
         * @param[in] lon            Longitude.
         * @param[in] range          Observer range to sub-observation
         *                           point.
         *
         * @return Cosine of emission angle, i.e. &mu;.
         */
        virtual double mu(double sub_observ_lat,
                          double sub_observ_lon,
                          double lat,
                          double lon,
                          double range) const = 0;

        /// Return cosine of incidence angle, i.e. &mu;<SUB>0</SUB>
        /**
         * @param[in] sub_solar_lat Planetocentric sub-solar
         *                          latitude.
         * @param[in] sub_solar_lon Sub-Solar longitude.
         * @param[in] lat           Planetocentric latitude.
         * @param[in] lon           Longitude.
         *
         * @return Cosine of incidence angle, i.e. &mu;<SUB>0</SUB>.
         *
         * @note Sun is assumed to be an infinite distance away.
         */
        virtual double mu0(double sub_solar_lat,
                           double sub_solar_lon,
                           double lat,
                           double lon) const = 0;

        /// Return cosine of phase angle, i.e. @c cos(&phi;)
        /**
         * @param[in] sub_observ_lat Planetocentric sub-observation
         *                           latitude.
         * @param[in] sub_observ_lon Sub-observation longitude.
         * @param[in] sub_solar_lat  Planetocentric sub-solar
         *                           latitude.
         * @param[in] sub_solar_lon  Sub-solar longitude.
         * @param[in] lat            Planetocentric latitude.
         * @param[in] lon            Longitude.
         * @param[in] range          Observer range to sub-observation.
         *                           point.
         *
         * @return Cosine of phase angle, i.e. @c cos(&phi;).
         */
        virtual double cos_phase(double sub_observ_lat,
                                 double sub_observ_lon,
                                 double sub_solar_lat,
                                 double sub_solar_lon,
                                 double lat,
                                 double lon,
                                 double range) const = 0;

    private:

        /**
         * @brief Flag that states whether the body rotation is
         *        prograde or retrograde.
         *
         * prograde == @c true, retrograde == @c false
         */
        bool const prograde_;

  };

}

#endif  /* MARC_BODY_DATA_H */
