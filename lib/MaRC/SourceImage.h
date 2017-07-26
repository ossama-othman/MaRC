// -*- C++ -*-
/**
 * @file SourceImage.h
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

#ifndef MARC_SOURCE_IMAGE_H
#define MARC_SOURCE_IMAGE_H


namespace MaRC
{
  /**
   * @class SourceImage
   *
   * @brief Abstract base class for all images to be mapped.
   *
   * Concrete source image classes must implement the interface
   * required by this abstract base class.
   */
  class SourceImage
  {
  public:

    /// Destructor.
    virtual ~SourceImage (void);

    /// Retrieve data from source image.
    /**
     * Retrieve data from source image.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude in radians.
     * @param lon  Longitude in radians.
     *
     * @param[out] data Data retrieved from image.
     *
     * @return @c true - Data retrieved, @c false - No data retrieved.
     */
    virtual bool read_data (double lat,
                            double lon,
                            double & data) const = 0;

  };

} // End MaRC namespace


#endif  /* MARC_SOURCE_IMAGE_H */
