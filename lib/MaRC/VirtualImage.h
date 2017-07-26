// -*- C++ -*-
/**
 * @file VirtualImage.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
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

#ifndef MARC_VIRTUAL_IMAGE_H
#define MARC_VIRTUAL_IMAGE_H

#include "MaRC/SourceImage.h"


namespace MaRC
{

  /**
   * @class VirtualImage
   *
   * @brief Base class for virtual source images.
   *
   * Data from virtual images are computed at run-time rather than
   * retrieved from static sources such as images stored on a
   * filesystem.
   */
  class VirtualImage : public SourceImage
  {
  public:

    /// Constructor.
    /**
     * @param s Linear scaling coefficient applied to computed data.
     * @param o Linear offset value applied to all (scaled) computed
     *          data.
     */
    VirtualImage(double s = 1, double o = 0);

    /// Retrieve data from virtual image.
    /**
     * Retrieve data from virtual image and apply configured data
     * transformations, if any.  Raw data is computed/retrieved from
     * the read_data_i() template method.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude in radians.
     * @param lon  Longitude in radians.
     *
     * @param[out] data Data retrieved from image.
     *
     * @return @c true - Data retrieved, @c false - No data retrieved.
     *
     * @see read_data_i()
     */
    virtual bool read_data(double lat, double lon, double & data) const;

  private:

    /// Compute data specific to a given virtual image.
    /**
     * This template method is the core implementation of the
     * read_data() method.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude  in radians.
     * @param lon  Longitude in radians.
     * @param data Data retrieved from image.
     *
     * @return @c true - Data retrieved, @c false - No data retrieved.
     *
     * @see read_data().
     */
    virtual bool read_data_i(double lat,
			     double lon,
			     double & data) const = 0;

    /// Is point at given latitude and longitude visible to the
    /// observer?
    /**
     * @param lat Bodycentric (e.g. planetocentric) latitude in radians
     * @param lon Longitude in radians
     *
     * @return @c true - Point is visible, @c false - Point is not visible
     *
     * @note The default implementation always returns @c true.
     */
    virtual bool is_visible (double lat, double lon) const;

    /**
     * @name Linear Data Transformation
     *
     * These methods return the values used when linearly transforming
     * the data read from an image.  In particular, data will be
     * transformed according to the following equation:
     *
     * @code
     *    data = data * scale() + offset()
     * @endcode
     *
     * The default implementation returns values that causes no
     * transformation to occur.
     */
    //@{
    /// Data scale.
    /**
     * Linear scaling coefficient applied to computed data.
     *
     * @return Data scale
     */
    double scale(void) const { return this->scale_; }

    /// Data offset
    /**
     * Linear offset value applied to all (scaled) computed data.
     *
     * @return Data offset
     */
    double offset(void) const { return this->offset_; }
    //@}

  private:

    /// Linear scaling coefficient applied to computed data.
    double const scale_;

    /// Linear offset value applied to all (scaled) computed data.
    double const offset_;

  };

} // End MaRC namespace


#endif  /* MARC_VIRTUAL_IMAGE_H */
