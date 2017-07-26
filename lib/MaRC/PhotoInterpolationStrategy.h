// -*- C++ -*-
/**
 * @file PhotoInterpolationStrategy.h
 *
 * Copyright (C) 2004-2005, 2017  Ossama Othman
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

#ifndef MARC_PHOTO_INTERPOLATION_STRATEGY_H
#define MARC_PHOTO_INTERPOLATION_STRATEGY_H

#include "InterpolationStrategy.h"


namespace MaRC
{

  /**
   * @class PhotoInterpolationStrategy
   *
   * @brief Bilinear interpolation strategy for PhotoImages.
   *
   * This strategy performs bilinear interpolation over 2x2 area of
   * pixels.
   */
  class PhotoInterpolationStrategy : public InterpolationStrategy
  {
  public:

    /// Destructor.
    virtual ~PhotoInterpolationStrategy (void);

    /// Constructor
    /**
     * @param samples        Number of samples in image.
     * @param lines          Number of lines   in image.
     * @param nibble_left    Left   nibble value.
     * @param nibble_right   Right  nibble value.
     * @param nibble_top     Top    nibble value.
     * @param nibble_bottom  Bottom nibble value.
     */
    PhotoInterpolationStrategy (unsigned int samples,
                                unsigned int lines,
                                unsigned int nibble_left,
                                unsigned int nibble_right,
                                unsigned int nibble_top,
                                unsigned int nibble_bottom);

    /// Perform bilinear interpolation over a 2x2 area of pixels on
    /// the given pixel.
    virtual bool interpolate (const double * image,
                              double x,
                              double z,
                              double & data) const;

    /// Copy this PhotoInterpolationStrategy object.
    virtual InterpolationStrategy * clone (void) const;

  private:

    /// Number of samples in image.
    const unsigned int samples_;

    /// Left most sample in image.
    const unsigned int left_;

    /// Right most sample in image.
    const unsigned int right_;

    /// Top most line in image.
    const unsigned int top_;

    /// Bottom most line in image.
    const unsigned int bottom_;

  };

}


#endif  /* MARC_PHOTO_INTERPOLATION_STRATEGY_H */
