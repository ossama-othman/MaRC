// -*- C++ -*-
/**
 * @file LatitudeImage.h
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

#ifndef MARC_LATITUDE_IMAGE_H
#define MARC_LATITUDE_IMAGE_H

#include "MaRC/VirtualImage.h"

#include <memory>


namespace MaRC
{
    class BodyData;

    /**
     * @class LatitudeImage
     *
     * @brief Latitude virtual image.
     *
     * This concrete VirtualImage returns the given latitude in
     * degrees.  This class may be configured to return bodygraphic
     * latitudes instead of bodycentric latitudes.
     */
    class LatitudeImage : public VirtualImage
    {
    public:

        /// Constructor
        /**
         * @param[in] body              Pointer to BodyData object
         *                              representing body being
         *                              mapped.
         * @param[in] graphic_latitudes Return bodygraphic latitudes
         *                              instead of bodycentric
         *                              latitudes.
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

    private:

        /// Simply return the given latitude in degrees.
        /**
         * @see MaRC::VirtualImage::read_data_i().
         */
        virtual bool read_data_i(double lat,
                                 double lon,
                                 double & data) const;

    private:

        /// Object representing the body being mapped.
        std::shared_ptr<BodyData> body_;

        /// Flag that determines if bodygraphic latitudes are returned
        /// instead of bodycentric latitudes.
        bool const graphic_latitudes_;

  };

} // End MaRC namespace


#endif  /* MARC_LATITUDE_IMAGE_H */
