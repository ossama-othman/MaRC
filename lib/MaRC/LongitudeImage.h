// -*- C++ -*-
/**
 * @file LongitudeImage.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othmanp
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

#ifndef MARC_LONGITUDE_IMAGE_H
#define MARC_LONGITUDE_IMAGE_H

#include "MaRC/VirtualImage.h"


namespace MaRC
{

  /**
   * @class LongitudeImage
   *
   * @brief Longitude virtual image.
   *
   * This concrete VirtualImage returns the given longitude in
   * degrees.
   */
  class LongitudeImage : public VirtualImage
  {
  private:

    /// Simply return the given longitude in degrees.
    /**
     * @see MaRC::VirtualImage::read_data_i().
     */
    virtual bool read_data_i (double lat,
                              double lon,
                              double & data) const;

  };

} // End MaRC namespace

#endif  /* MARC_LONGITUDE_IMAGE_H */
