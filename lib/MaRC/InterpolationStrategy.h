// -*- C++ -*-
/**
 * @file InterpolationStrategy.h
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

#ifndef MARC_INTERPOLATION_STRATEGY_H
#define MARC_INTERPOLATION_STRATEGY_H

#include <MaRC/ValuePtr.h>


namespace MaRC
{

  /**
   * @class Interpolation Strategy
   *
   * @brief Abstract base class for all interpolation strategies.
   *
   * Concrete interpolation classes must implement the interface
   * required by this abstract base class.
   */
  class InterpolationStrategy
  {
  public:

    /// Destructor.
    virtual ~InterpolationStrategy (void);

    /// Perform interpolation on given pixel.
    /**
     * The interpolation technique will not include invalid data
     * (e.g. @c NaN) when computing interpolated values.
     *
     * @param image     The image array containing the data to be
     *                  interpolated.
     * @param x         Floating point sample in image.
     * @param z         Floating point line in image.
     * @param data[out] Interpolated data.
     *
     * @return @c true if interpolation succeeded.
     */
    virtual bool interpolate (const double * image,
                              double x,
                              double z,
                              double & data) const = 0;

    /// Clone operation that polymorphically copies the concrete
    /// @c InterpolationStrategy object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<InterpolationStrategy> template specialization to
     * prevent the slicing that occurs when copying through a copy
     * constructor instead.
     */
    virtual InterpolationStrategy * clone (void) const = 0;

  };

  // InterpolationStrategy specialization of the ValuePtr traits template.
  template<>
  struct VP_traits<InterpolationStrategy>
  {
    static InterpolationStrategy * clone (const InterpolationStrategy * p)
    {
      return p->clone ();
    }
  };

}

#endif  /* MARC_INTERPOLATION_STRATEGY_H */
