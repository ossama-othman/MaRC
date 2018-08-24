//  -*- C++ -*-

//==========================================================================
/**
 * @file GLLGeometricCorrection.h
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


#ifndef MARC_GLL_GEOMETRIC_CORRECTION_H
#define MARC_GLL_GEOMETRIC_CORRECTION_H

#include <marc/GeometricCorrection.h>
#include <marc/Export.h>

#include <cstddef>


//   // Kludge to "automatically" detect summation mode images
//   if (Samples_ > OA_SAMP)
//     this->summation_mode_ = 1; // Full frame
//   else
//     this->summation_mode_ = 2; // Summation mode frame

namespace MaRC
{
    /**
     * @class GLLGeometricCorrection
     *
     * @brief Galileo Spacecraft lens aberration correction strategy.
     *
     * Galileo specific concrete geometric correction strategy.
     */
    class MARC_API GLLGeometricCorrection final
        : public GeometricCorrection
    {
    public:

        /// Constructor
        /**
         * @param[in] samples Number of samples in the PhotoImage.
         *                    Only used to determine whether summation
         *                    mode should be enabled.
         */
        GLLGeometricCorrection(std::size_t samples);

        /**
         * @name GeometricCorrection Methods
         *
         * Virtual methods required by the GeometricCorrection abstract
         * base class.
         */
        //@{
        void image_to_object(double & line, double & sample) const;
        void object_to_image(double & line, double & sample) const;
        //@}

        /// Return current summation mode
        bool summation_mode() const { return this->summation_mode_; }

    private:

        /// true = summation mode, false = full frame
        bool const summation_mode_;

    };

}


#endif  /* MARC_GLL_GEOMETRIC_CORRECTION_H */
