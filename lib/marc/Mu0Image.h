// -*- C++ -*-
/**
 * @file Mu0Image.h
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MU0_IMAGE_H
#define MARC_MU0_IMAGE_H

#include "marc/VirtualImage.h"
#include "marc/Export.h"

#include <memory>


namespace MaRC
{
    class BodyData;

    /**
     * @class Mu0Image Mu0Image.h <marc/Mu0Image.h>
     *
     * @brief Cosine of the incidence angle (i.e. &mu;<SUB>0</SUB>)
     *        virtual image.
     *
     * This concrete @c VirtualImage returns the cosine of the
     * sun-local-normal (incidence) angle, &mu;<SUB>0</SUB>, on the
     * body being mapped.  The sun is assumed to be an infinite
     * distance away.
     */
    class MARC_API Mu0Image final : public VirtualImage
    {
    public:

        /// Constructor
        /**
         * @param[in] body          Object representing the body being
         *                          mapped.
         * @param[in] sub_solar_lat Planetocentric sub-solar latitude
         *                          in degrees.
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

        /// Destructor.
        ~Mu0Image() override = default;

    private:

        /// Compute cosine of the incidence angle, &mu;<SUB>0</SUB>.
        /**
         * @see MaRC::VirtualImage::read_data_i().
         */
        bool read_data_i(double lat,
                         double lon,
                         double & Data) const override;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Planetocentric sub-solar latitude in radians.
        double const sub_solar_lat_;

        /// Sub-solar longitude in radians.
        double const sub_solar_lon_;

    };

} // End MaRC namespace


#endif  /* MARC_MU0_IMAGE_H */
