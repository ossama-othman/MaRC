// -*- C++ -*-
/**
 * @file GeometricCorrection.h
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_GEOMETRIC_CORRECTION_H
#define MARC_GEOMETRIC_CORRECTION_H

#include <marc/Export.h>


namespace MaRC
{

    /**
     * @class GeometricCorrection GeometricCorrection.h <marc/GeometricCorrection.h>
     *
     * @brief Abstract base class for all geometric correction
     *        strategies.
     *
     * Concrete geometric correction classes must implement the interface
     * required by this abstract base class.
     */
    class MARC_API GeometricCorrection
    {
    public:

        GeometricCorrection() = default;

        // Disallow copying.
        GeometricCorrection(GeometricCorrection const &) = delete;
        GeometricCorrection & operator=(
            GeometricCorrection const &) = delete;

        /// Destructor.
        virtual ~GeometricCorrection() = default;

        /// Convert from image space to object space.
        /**
         * Apply lens aberration correction.
         *
         * @param[in,out] line   Image-space line coordinate converted
         *                       to object space.
         * @param[in,out] sample Image-space sample coordinate
         *                       converted to object space.
         */
        virtual void image_to_object(double & line,
                                     double & sample) const = 0;

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
        virtual void object_to_image(double & line,
                                     double & sample) const = 0;

    };

} // End MaRC namespace


#endif  /* MARC_GEOMETRIC_CORRECTION_H */
