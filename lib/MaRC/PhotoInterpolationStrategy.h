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

#include <cstddef>


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

      /// Constructor
      /**
       * @param[in] samples        Number of samples in image.
       * @param[in] lines          Number of lines   in image.
       * @param[in] nibble_left    Left   nibble value.
       * @param[in] nibble_right   Right  nibble value.
       * @param[in] nibble_top     Top    nibble value.
       * @param[in] nibble_bottom  Bottom nibble value.
       */
      PhotoInterpolationStrategy(std::size_t samples,
                                 std::size_t lines,
                                 std::size_t nibble_left,
                                 std::size_t nibble_right,
                                 std::size_t nibble_top,
                                 std::size_t nibble_bottom);

      /// Destructor.
      virtual ~PhotoInterpolationStrategy();

      /// Perform bilinear interpolation over a 2x2 area of pixels on
      /// the given pixel.
      /**
       * @todo Document parameters.
       */
      virtual bool interpolate(double const * image,
                               double x,
                               double z,
                               double & data) const;

  private:

      /// Number of samples in image.
      std::size_t const samples_;

      /// Left most sample in image.
      std::size_t const left_;

      /// Right most sample in image.
      std::size_t const right_;

      /// Top most line in image.
      std::size_t const top_;

      /// Bottom most line in image.
      std::size_t const bottom_;

  };

}


#endif  /* MARC_PHOTO_INTERPOLATION_STRATEGY_H */
