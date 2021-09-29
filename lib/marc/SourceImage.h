// -*- C++ -*-
/**
 * @file SourceImage.h
 *
 * Copyright (C) 1999, 2003-2004, 2017-2018, 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_SOURCE_IMAGE_H
#define MARC_SOURCE_IMAGE_H

#include <marc/Export.h>

#include <cstddef>


namespace MaRC
{

    /**
     * @class SourceImage SourceImage.h <marc/SourceImage.h>
     *
     * @brief Abstract base class for all images to be mapped.
     *
     * Concrete source image classes must implement the interface
     * required by this abstract base class.
     */
    class MARC_API SourceImage
    {
    public:

        SourceImage() = default;

        // Disallow copying.
        SourceImage(SourceImage const &) = delete;
        SourceImage & operator=(SourceImage const &) = delete;

        // Disallow moving.
        SourceImage(SourceImage &&) = delete;
        SourceImage & operator=(SourceImage &&) = delete;

        /// Destructor.
        virtual ~SourceImage() = default;

        /**
         * @brief Retrieve physical data from source image.
         *
         * @param[in]  lat  Planetocentric latitude in radians.
         * @param[in]  lon  Longitude in radians.
         * @param[out] data Physical data retrieved from image.
         *
         * @retval true  Physical data retrieved.
         * @retval false No physical data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const = 0;

        /**
         * @brief Retrieve data and weight from source image.
         *
         * Retrieve physical data and weight from source image.  The
         * default implementation merely ignores the @a weight and
         * @a scan arguments, and forwards the call to the concrete
         * implementation of @c read_data().  Subclasses should
         * override this method if they will provide a @a weight along
         * with @a data.
         *
         * @param[in]     lat    Planetocentric latitude in radians.
         * @param[in]     lon    Longitude in radians.
         * @param[out]    data   Physical data retrieved from image.
         * @param[in,out] weight Physical data weight.
         * @param[in]     scan   Flag that determines if a data weight
         *                       scan is performed..
         *
         * @retval true  Physical data retrieved.
         * @retval false No physical data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data,
                               double & weight,
                               bool scan) const;

    };

} // End MaRC namespace


#endif  /* MARC_SOURCE_IMAGE_H */
