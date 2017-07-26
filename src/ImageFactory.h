// -*- C++ -*-
/**
 * @file ImageFactory.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#ifndef MARC_IMAGE_FACTORY_H
#define MARC_IMAGE_FACTORY_H

#include "MaRC/ValuePtr.h"


namespace MaRC
{
  class SourceImage;

  /**
   * @class ImageFactory
   *
   * @brief Abstract factory class containing interface for image
   *        factories.
   *
   * @note An @c ImageFactory can be considered a map plane factory as
   *       well.
   */
  class ImageFactory
  {
  public:

    /// Constructor.
    ImageFactory (void);

    /// Destructor.
    virtual ~ImageFactory (void);

    /// Create an Image.
    virtual SourceImage * make (void) = 0;

    /// Clone operation that polymorphically copies the concrete
    /// @c ImageFactory object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<ImageFactory> template specialization to prevent the
     * slicing that occurs when copying through a copy constructor
     * instead.
     */
    virtual ImageFactory * clone (void) const = 0;

    /// Set minimum allowed data value in map plane.  (data > minimum)
    void minimum (double m) { this->minimum_ = m; }

    /// Set maximum allowed data value in map plane.  (data < maximum)
    void maximum (double m) { this->maximum_ = m; }

    /// Return minimum allowed data value in map plane.  (data > minimum)
    double minimum (void) const { return this->minimum_; }

    /// Return maximum allowed data value in map plane.  (data < maximum)
    double maximum (void) const { return this->maximum_; }

  private:

    /// Minimum allowed data value in map plane.  (data > minimum)
    double minimum_;

    /// Maximum allowed data value in map plane.  (data < maximum)
    double maximum_;

  };

  // ImageFactory specialization of the ValuePtr traits template.
  template<>
  struct VP_traits<ImageFactory>
  {
    static ImageFactory * clone (const ImageFactory * p)
    {
      return p->clone ();
    }
  };

}

#endif  /* MARC_IMAGE_FACTORY_H */
