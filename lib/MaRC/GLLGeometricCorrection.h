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

#include <MaRC/GeometricCorrection.h>


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
  class GLLGeometricCorrection : public GeometricCorrection
  {
  public:

    /// Constructor
    /**
     * @param samples Number of samples in the PhotoImage.  Only used
     *                to determine whether summation mode should be
     *                enabled.
     */
    GLLGeometricCorrection (unsigned int samples);

    /**
     * @name GeometricCorrection Methods
     *
     * Virtual methods required by the GeometricCorrection abstract
     * base class.
     */
    //@{
    virtual void image_to_object (double & line, double & sample);
    virtual void object_to_image (double & line, double & sample);
    virtual GeometricCorrection * clone (void) const;
    //@}

    /// Return current summation mode
    bool summation_mode (void) const { return this->summation_mode_; }

  private:

    /// Compute the cubic root of a given value
    static double cube_root (double x);

  private:

    /// true = summation mode, false = full frame
    const bool summation_mode_;

  };

}

#endif  /* MARC_GLL_GEOMETRIC_CORRECTION_H */
