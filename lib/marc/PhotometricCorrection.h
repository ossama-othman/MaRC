//   -*- C++ -*-
/**
 * @file PhotometricCorrection.h
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PHOTOMETRIC_CORRECTION_H
#define MARC_PHOTOMETRIC_CORRECTION_H

#include <marc/Export.h>


namespace MaRC
{
    class ViewingGeometry;

    /**
     * @class PhotometricCorrection PhotometricCorrection.h <marc/PhotometricCorrection.h>
     *
     * @brief Photometric correction strategy abstract base class.
     *
     * The @c PhotometricCorrection base class defines the interface
     * used by the @c PhotoImage class when performing photometric
     * correction in an image, such as compensating for limb
     * darkening).  All such photometric metric correction strategies
     * should inherit from this base class.
     */
    class MARC_API PhotometricCorrection
    {
    public:

        PhotometricCorrection() = default;

        // Disallow copying.
        PhotometricCorrection(PhotometricCorrection const &) = delete;
        PhotometricCorrection & operator=(
            PhotometricCorrection const &) = delete;

        // Disallow moving.
        PhotometricCorrection(PhotometricCorrection &&) = delete;
        PhotometricCorrection & operator=(
            PhotometricCorrection &&) = delete;

        /// Destructor
        virtual ~PhotometricCorrection() = default;

        /**
         * @brief Perform photometric correction.
         *
         * @param[in]     geometry Viewing geometry.
         * @param[in,out] data     Data to be photometrically
         *                         corrected.
         *
         * @return @c true on successful correction.
         */
        virtual bool correct(ViewingGeometry const & geometry,
                             double & data) = 0;

    };

}


#endif  /* MARC_PHOTOMETRIC_CORRECTION_H */
