//   -*- C++ -*-
/**
 * @file PhotometricCorrection.h
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

#ifndef MARC_PHOTOMETRIC_CORRECTION_H
#define MARC_PHOTOMETRIC_CORRECTION_H


namespace MaRC
{
    class ViewingGeometry;

    /**
     * @class PhotometricCorrection
     *
     * @brief Photometric correction strategy abstract base class.
     *
     * The @c PhotometricCorrection base class defines the interface
     * used by the @c PhotoImage class when performing photometric
     * correction in an image, such as compensating for limb
     * darkening).  All such photometric metric correction strategies
     * should inherit from this base class.
     */
    class PhotometricCorrection
    {
    public:

        PhotometricCorrection() = default;

        // Disallow copying.
        PhotometricCorrection(PhotometricCorrection const &) = delete;
        PhotometricCorrection & operator=(
            PhotometricCorrection const &) = delete;

        /// Destructor
        virtual ~PhotometricCorrection();

        /// Perform photometric correction.
        /**
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
