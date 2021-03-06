// -*- C++ -*-
/**
 * @file MuImage.h
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MU_IMAGE_H
#define MARC_MU_IMAGE_H

#include "marc/VirtualImage.h"
#include "marc/Export.h"

#include <memory>


namespace MaRC
{
    class BodyData;

    /**
     * @class MuImage MuImage.h <marc/MuImage.h>
     *
     * @brief Cosine of emission angle (i.e. &mu;) virtual image.
     *
     * This concrete @c VirtualImage returns the cosine of the
     * emission angle on the body being mapped.  The observer range is
     * taken into account.
     */
    class MARC_API MuImage final : public VirtualImage
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
         * @param[in] range          Observer to target center
         *                           distance.
         * @param[in] scale          Linear scaling value by which
         *                           cosines will be multiplied.
         * @param[in] offset         Offset value to be added to
         *                           cosines after the scaling factor
         *                           has been applied.
         */
        MuImage(std::shared_ptr<BodyData> body,
                double sub_observ_lat,
                double sub_observ_lon,
                double range,
                double scale,
                double offset);

        // Disallow copying.
        MuImage(MuImage const &) = delete;
        MuImage & operator=(MuImage const &) = delete;

        // Disallow moving.
        MuImage(MuImage &&) = delete;
        MuImage & operator=(MuImage &&) = delete;

        /// Destructor.
        ~MuImage() override = default;

    private:

        /// Compute cosine of emission angle, &mu;.
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


#endif  /* MARC_MU_IMAGE_H */
