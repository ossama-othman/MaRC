// -*- C++ -*-
/**
 * @file BilinearInterpolationStrategy.h
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

#ifndef MARC_BILINEAR_INTERPOLATION_STRATEGY_H
#define MARC_BILINEAR_INTERPOLATION_STRATEGY_H

#include "InterpolationStrategy.h"

#include <cstddef>


namespace MaRC
{

  /**
   * @class BilinearInterpolationStrategy
   *
   * @brief Bilinear interpolation strategy.
   *
   * This strategy performs bilinear interpolation over 2x2 block of
   * data.
   */
  class BilinearInterpolationStrategy : public InterpolationStrategy
  {
  public:

      /// Constructor
      /**
       * @param[in] nibble_left    Left   nibble value.
       * @param[in] nibble_right   Right  nibble value.
       * @param[in] nibble_top     Top    nibble value.
       * @param[in] nibble_bottom  Bottom nibble value.
       */
      BilinearInterpolationStrategy(std::size_t nibble_left,
                                    std::size_t nibble_right,
                                    std::size_t nibble_top,
                                    std::size_t nibble_bottom);

      /// Destructor.
      virtual ~BilinearInterpolationStrategy();

      /// Perform bilinear interpolation over a 2x2 area of pixels on
      /// the given pixel.
      /**
       * @see @c InterpolationStrategy for parameter details.
       */
      virtual bool interpolate(double const * data,
                               std::size_t samples,
                               std::size_t lines,
                               double x,
                               double z,
                               double & datum) const;

  private:

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


#endif  /* MARC_BILINEAR_INTERPOLATION_STRATEGY_H */
