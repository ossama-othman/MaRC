// -*- C++ -*-
/**
 *  @file CosPhaseImage.h
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_COS_PHASE_IMAGE_H
#define MARC_COS_PHASE_IMAGE_H

#include "marc/VirtualImage.h"
#include "marc/Export.h"

#include <memory>


namespace MaRC
{
    class BodyData;

    /**
     * @class CosPhaseImage CosPhaseImage.h <marc/CosPhaseImage.h>
     *
     * @brief Cosine of phase angle (i.e. cos(&phi;)) virtual
     *        image.
     *
     * This concrete VirtualImage returns the cosine of the Sun-point
     * on surface of body-Observer (phase) angle, &phi;, on the body
     * being mapped.  The observer range is taken into account.
     */
    class MARC_API CosPhaseImage final : public VirtualImage
    {
    public:

        /// Constructor
        /**
         * @param[in] body           Object representing the body
         *                           being mapped.
         * @param[in] sub_observ_lat Planetocentric sub-observer
         *                           latitude in degrees.
         * @param[in] sub_observ_lon Sub-observer longitude in
         *                           degrees.
         * @param[in] sub_solar_lat  Planetocentric sub-solar latitude
         *                           in degrees.
         * @param[in] sub_solar_lon  Sub-solar longitude in degrees.
         * @param[in] range          Observer to target center
         *                           distance.
         * @param[in] scale          Linear scaling value by which
         *                           cosines will be multiplied.
         * @param[in] offset         Offset value to be added to
         *                           cosines after the scaling factor
         *                           has been applied.
         */
        CosPhaseImage(std::shared_ptr<BodyData> body,
                      double sub_observ_lat,
                      double sub_observ_lon,
                      double sub_solar_lat,
                      double sub_solar_lon,
                      double range,
                      double scale,
                      double offset);

        // Disallow copying.
        CosPhaseImage(CosPhaseImage const &) = delete;
        CosPhaseImage & operator=(CosPhaseImage const &) = delete;

        // Disallow moving.
        CosPhaseImage(CosPhaseImage &&) = delete;
        CosPhaseImage & operator=(CosPhaseImage &&) = delete;

        /// Destructor.
        ~CosPhaseImage() override = default;

    private:

        /// Compute cosine of phase angle, @c cos(&phi;).
        /**
         * @see MaRC::VirtualImage::read_data_i().
         */
        bool read_data_i(double lat,
                         double lon,
                         double & data) const override;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Planetocentric sub-observer latitude in radians.
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
