// -*- C++ -*-
/**
 * @file LatitudeImage.h
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_LATITUDE_IMAGE_H
#define MARC_LATITUDE_IMAGE_H

#include "marc/VirtualImage.h"
#include "marc/Export.h"

#include <memory>


namespace MaRC
{
    class BodyData;

    /**
     * @class LatitudeImage LatitudeImage.h <marc/LatitudeImage.h>
     *
     * @brief Latitude virtual image.
     *
     * This concrete VirtualImage returns the given latitude in
     * degrees.  This class may be configured to return planetographic
     * latitudes instead of planetocentric latitudes.
     */
    class MARC_API LatitudeImage final : public VirtualImage
    {
    public:

        /// Constructor
        /**
         * @param[in] body              Pointer to BodyData object
         *                              representing body being
         *                              mapped.
         * @param[in] graphic_latitudes Return planetographic
         *                              latitudes instead of
         *                              planetocentric latitudes.
         * @param[in] scale             Linear scaling value by which
         *                              latitudes should be
         *                              multiplied.
         * @param[in] offset            Offset value to be added to
         *                              latitudes after the scaling
         *                              factor has been applied.
         */
        LatitudeImage(std::shared_ptr<BodyData> body,
                      bool graphic_latitudes,
                      double scale,
                      double offset);

        // Disallow copying.
        LatitudeImage(LatitudeImage const &) = delete;
        LatitudeImage & operator=(LatitudeImage const &) = delete;

        // Disallow moving.
        LatitudeImage(LatitudeImage &&) = delete;
        LatitudeImage & operator=(LatitudeImage &&) = delete;

        /// Destructor.
        ~LatitudeImage() override = default;

    private:

        /// Simply return the given latitude in degrees.
        /**
         * @see MaRC::VirtualImage::read_data_i().
         */
        bool read_data_i(double lat,
                         double lon,
                         double & data) const override;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> body_;

        /// Flag that determines if planetographic latitudes are
        /// returned instead of planetocentric latitudes.
        bool const graphic_latitudes_;

    };

} // End MaRC namespace


#endif  /* MARC_LATITUDE_IMAGE_H */
