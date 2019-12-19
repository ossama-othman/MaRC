// -*- C++ -*-
/**
 * @file BilinearInterpolation.h
 *
 * Copyright (C) 2004-2005, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_BILINEAR_INTERPOLATION_H
#define MARC_BILINEAR_INTERPOLATION_H

#include "marc/InterpolationStrategy.h"
#include "marc/Export.h"

#include <cstddef>


namespace MaRC
{

  /**
   * @class BilinearInterpolation BilinearInterpolation.h <marc/BilinearInterpolation.h>
   *
   * @brief Bilinear interpolation strategy.
   *
   * This strategy performs bilinear interpolation over 2x2 block of
   * data.
   */
  class MARC_API BilinearInterpolation final
      : public InterpolationStrategy
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
      BilinearInterpolation(std::size_t samples,
                            std::size_t lines,
                            std::size_t nibble_left,
                            std::size_t nibble_right,
                            std::size_t nibble_top,
                            std::size_t nibble_bottom);

      /// Destructor.
      ~BilinearInterpolation() override = default;

      /// Perform bilinear interpolation over a 2x2 area of pixels on
      /// the given pixel.
      /**
       * @see @c InterpolationStrategy for parameter details.
       */
      bool interpolate(double const * data,
                       double x,
                       double z,
                       double & datum) const override;

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


#endif  /* MARC_BILINEAR_INTERPOLATION_H */
