// -*- C++ -*-
/**
 *  @file CosPhaseImage.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
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

#ifndef MARC_COS_PHASE_IMAGE_H
#define MARC_COS_PHASE_IMAGE_H

#include "MaRC/VirtualImage.h"

#include <memory>


namespace MaRC
{
    class OblateSpheroid;

    /**
     * @class CosPhaseImage
     *
     * @brief Cosine of phase angle virtual image.
     *
     * This concrete VirtualImage returns the cosine of the Sun-point
     * on surface of body-Observer (phase) angle on the body being
     * mapped.  The observer range is taken into account.
     */
    class CosPhaseImage : public VirtualImage
    {
    public:

        /// Constructor
        /**
         * @param[in] body           @c OblateSpheroid object
         *                           representing the body being
         *                           mapped.
         * @param[in] sub_observ_lat Bodycentric sub-observer latitude
         *                           in degrees.
         * @param[in] sub_observ_lon Sub-observer longitude in
         *                           degrees.
         * @param[in] sub_solar_lat  Sub-solar latitude in degrees.
         * @param[in] sub_solar_lon  Sub-solar longitude in degrees.
         * @param[in] range          Observer to target center
         *                           distance.
         */
        CosPhaseImage(std::shared_ptr<OblateSpheroid> body,
                      double sub_observ_lat,
                      double sub_observ_lon,
                      double sub_solar_lat,
                      double sub_solar_lon,
                      double range);

    private:

        /// Compute cosine of phase angle.
        /**
         * @see MaRC::VirtualImage::read_data_i().
         */
        virtual bool read_data_i(double lat,
                                 double lon,
                                 double & data) const;

        /// Is point at given latitude and longitude visible to the
        /// observer?
        /**
         * @see MaRC::VirtualImage::is_visible().
         */
        virtual bool is_visible(double lat, double lon) const;

    private:

        /// Object representing the body being mapped.
        /**
         * @note OblateSpheroid is used instead of BodyData since some
         *       code in this implementation assumes that the body is
         *       modeled as an oblate spheroid.
         */
        std::shared_ptr<OblateSpheroid> const body_;

        /// Bodycentric sub-observer latitude in radians.
        double const sub_observ_lat_;

        /// Sub-observer longitude in radians.
        double const sub_observ_lon_;

        /// Sub-solar latitude in radians.
        double const sub_solar_lat_;

        /// Sub-solar longitude in radians.
        double const sub_solar_lon_;

        /// Observer to target center distance.
        /**
         * The range is the line of sight passing through the
         * sub-observer point on the surface of the body.
         *
         * @note The unit used for the range should be consistent with
         *       the unit used for the body radii.
         */
        double const range_;

    };

} // End MaRC namespace


#endif  /* MARC_COS_PHASE_IMAGE_H */
