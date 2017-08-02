// -*- C++ -*-
/**
 * @file SourceImage.h
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
//==========================================================================

#ifndef MARC_SOURCE_IMAGE_H
#define MARC_SOURCE_IMAGE_H

#include <cstddef>


namespace MaRC
{
    /**
     * @class SourceImage
     *
     * @brief Abstract base class for all images to be mapped.
     *
     * Concrete source image classes must implement the interface
     * required by this abstract base class.
     */
    class SourceImage
    {
    public:

        SourceImage() = default;

        // Disallow copying.
        SourceImage(SourceImage const &) = delete;
        SourceImage & operator=(SourceImage const &) = delete;

        /// Destructor.
        virtual ~SourceImage();

        /// Retrieve data from source image.
        /**
         * Retrieve data from source image.
         *
         * @param[in]  lat  Bodycentric (e.g. planetocentric) latitude
         *                  in radians.
         * @param[in]  lon  Longitude in radians.
         * @param[out] data Data retrieved from image.
         *
         * @retval @c true  Data retrieved,
         * @retval @c false No data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const = 0;

        /// Retrieve data and weight from source image.
        /**
         * Retrieve data and weight from source image.  The default
         * implementation merely ignores the @a weight and @a scan
         * arguments, and forwards the call to the concrete
         * implementation of @c read_data().  Subclasses should
         * override this method if they will provide a @a weight along
         * with @a data.
         *
         * @param[in]     lat    Bodycentric (e.g. planetocentric)
         *                       latitude in radians.
         * @param[in]     lon    Longitude in radians.
         * @param[out]    data   Data retrieved from image.
         * @param[in,out] weight Distance from pixel to closest edge
         *                       or blank pixel.
         * @param[in]     scan   Flag that determines if a data weight
         *                       scan is performed..
         *
         * @retval @c true  Data retrieved,
         * @retval @c false No data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data,
                               std::size_t & weight,
                               bool scan) const;
    };

} // End MaRC namespace


#endif  /* MARC_SOURCE_IMAGE_H */
