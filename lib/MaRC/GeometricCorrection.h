// -*- C++ -*-
/**
 * @file GeometricCorrection.h
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

#ifndef MARC_GEOMETRIC_CORRECTION_H
#define MARC_GEOMETRIC_CORRECTION_H


namespace MaRC
{

    /**
     * @class GeometricCorrection
     *
     * @brief Abstract base class for all geometric correction
     *        strategies.
     *
     * Concrete geometric correction classes must implement the interface
     * required by this abstract base class.
     */
    class GeometricCorrection
    {
    public:

        // Disallow copying.
        GeometricCorrection(GeometricCorrection const &) = delete;
        GeometricCorrection & operator=(
            GeometricCorrection const &) = delete;

        /// Destructor.
        virtual ~GeometricCorrection();

        /// Convert from image space to object space.
        /**
         * Apply lens aberration correction.
         *
         * @param[in,out] line   Image-space line coordinate converted
         *                       to object space.
         * @param[in,out] sample Image-space sample coordinate
         *                       converted to object space.
         */
        virtual void image_to_object(double & line, double & sample) = 0;

        /// Convert from object space to image space.
        /**
         * Introduce lens aberration to determine actual image space
         * pixel values.
         *
         * @param[in,out] line   Object-space line coordinate
         *                       converted to image space.
         * @param[in,out] sample Object-space sample coordinate
         *                       converted to image space.
         */
        virtual void object_to_image(double & line, double & sample) = 0;
    };

} // End MaRC namespace


#endif  /* MARC_GEOMETRIC_CORRECTION_H */
