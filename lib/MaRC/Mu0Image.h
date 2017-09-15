// -*- C++ -*-
/**
 * @file Mu0Image.h
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

#ifndef MARC_MU0_IMAGE_H
#define MARC_MU0_IMAGE_H

#include "MaRC/VirtualImage.h"

#include <memory>


namespace MaRC
{
    class BodyData;

    /**
     * @class Mu0Image
     *
     * @brief Cosine of the incidence angle (i.e. &mu;<SUB>0</SUB>)
     *        virtual image.
     *
     * This concrete @c VirtualImage returns the cosine of the
     * sun-local-normal (incidence) angle, &mu;<SUB>0</SUB>, on the
     * body being mapped.  The sun is assumed to be an infinite
     * distance away.
     */
    class Mu0Image : public VirtualImage
    {
    public:

        /// Constructor
        /**
         * @param[in] body          Object representing the body being
         *                          mapped.
         * @param[in] sub_solar_lat Bodycentric sub-solar latitude in
         *                          degrees.
         * @param[in] sub_solar_lon Sub-solar longitude in degrees.
         * @param[in] scale         Linear scaling value by which
         *                          cosines will be multiplied.
         * @param[in] offset        Offset value to be added to
         *                          cosines after the scaling factor
         *                          has been applied.
         */
        Mu0Image(std::shared_ptr<BodyData> body,
                 double sub_solar_lat,
                 double sub_solar_lon,
                 double scale,
                 double offset);

    private:

        /// Compute cosine of the incidence angle, &mu;<SUB>0</SUB>.
        /**
         * @see MaRC::VirtualImage::read_data_i().
         */
        virtual bool read_data_i(double lat,
                                 double lon,
                                 double & Data) const;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Bodycentric sub-solar latitude in radians.
        const double sub_solar_lat_;

        /// Sub-solar longitude in radians.
        const double sub_solar_lon_;

    };

} // End MaRC namespace


#endif  /* MARC_MU0_IMAGE_H */
