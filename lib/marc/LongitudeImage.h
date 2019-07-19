// -*- C++ -*-
/**
 * @file LongitudeImage.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_LONGITUDE_IMAGE_H
#define MARC_LONGITUDE_IMAGE_H

#include "marc/VirtualImage.h"
#include "marc/Export.h"


namespace MaRC
{

    /**
     * @class LongitudeImage LongitudeImage.h <marc/LongitudeImage.h>
     *
     * @brief Longitude virtual image.
     *
     * This concrete VirtualImage returns the given longitude in
     * degrees.
     */
    class MARC_API LongitudeImage final : public VirtualImage
    {
    public:

        /// Constructor
        /**
         * @param[in] scale             Linear scaling value by which
         *                              longitudes should be
         *                              multiplied.
         * @param[in] offset            Offset value to be added to
         *                              longitudes after the scaling
         *                              factor has been applied.
         */
        LongitudeImage(double scale, double offset);

        // Disallow copying.
        LongitudeImage(LongitudeImage const &) = delete;
        LongitudeImage & operator=(LongitudeImage const &) = delete;

        // Disallow moving.
        LongitudeImage(LongitudeImage &&) = delete;
        LongitudeImage & operator=(LongitudeImage &&) = delete;

        /// Destructor.
        ~LongitudeImage() override = default;

    private:

        /// Simply return the given longitude in degrees.
        /**
         * @see MaRC::VirtualImage::read_data_i().
         */
        bool read_data_i(double lat,
                         double lon,
                         double & data) const override;

    };

} // End MaRC namespace


#endif  /* MARC_LONGITUDE_IMAGE_H */
