//   -*- C++ -*-
/**
 * @file BodyData.h
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

#ifndef MARC_BODY_DATA_H
#define MARC_BODY_DATA_H

#include <MaRC/Export.h>


namespace MaRC
{

    /**
     * @class BodyData
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
        BodyData(bool prograde);

        // Disallow copying.
        BodyData(BodyData const &) = delete;
        BodyData & operator=(BodyData const &) = delete;

        /// Destructor
        virtual ~BodyData();

        /// Return the direction of rotation.
        /**
         * @return @c true if the current rotation is prograde.
         */
        bool prograde() const { return this->prograde_; }

        /// Find radius as a function of bodycentric latitude.
        /**
         * Calculate and return the radius, i.e. distance from the
         * center of the body to the point on the surface at given
         * latitude.
         *
         * @param[in] lat Bodycentric (e.g. planetocentric) latitude
         *                in radians.
         *
         * @return Radius, i.e. distance from the center of the body
         *         to point on the surface at given latitude.
         *
         * @todo We really should add a longitude parameter since not
         *       all bodies are symmetrical about their polar axis.
         *
         */
        virtual double centric_radius(double lat) const = 0;

        /// Convert from GRAPHIC to CENTRIC latitude
        /**
         * @param[in] latg Graphic (e.g. planetographic) latitude in
         *                 radians.
         *
         * @return Bodycentric latitude in radians.
         *
         * @todo We really should add a longitude parameter since not
         *       all bodies are symmetrical about their polar axis.
         *
         */
        virtual double centric_latitude(double latg) const = 0;

        /// Convert from CENTRIC to GRAPHIC latitude
        /**
         * @param[in] lat Centric (e.g. planetocentric) latitude in
         *                radians.
         *
         * @return Bodygraphic latitude in radians.
         *
         * @todo We really should add a longitude parameter since not
         *       all bodies are symmetrical about their polar axis.
         */
        virtual double graphic_latitude(double lat) const = 0;

        /// Return cosine of emission angle, i.e. &mu;
        /**
         * @param[in] sub_observ_lat Bodycentric subobservation
         *                           latitude
         * @param[in] sub_observ_lon Subobservation longitude
         * @param[in] lat            Bodycentric latitude
         * @param[in] lon            Longitude
         * @param[in] range          Observer range to subobservation
         *                           point
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
         * @param[in] sub_solar_lat Bodycentric subsolar latitude
         * @param[in] sub_solar_lon Subsolar longitude
         * @param[in] lat           Bodycentric latitude
         * @param[in] lon           Longitude
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
         * @param[in] sub_observ_lat Bodycentric subobservation
         *                           latitude
         * @param[in] sub_observ_lon Subobservation longitude
         * @param[in] sub_solar_lat  Bodycentric subsolar latitude
         * @param[in] sub_solar_lon  Subsolar longitude
         * @param[in] lat            Bodycentric latitude
         * @param[in] lon            Longitude
         * @param[in] range          Observer range to subobservation
         *                           point
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

        /// Flag that states whether the body rotation is prograde or
        /// retrograde.
        /**
         * prograde == @c true, retrograde == @c false
         */
        bool const prograde_;

  };

}

#endif  /* MARC_BODY_DATA_H */
