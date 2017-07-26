// -*- C++ -*-
/**
 * @file Mercator.h
 *
 * Copyright (C) 1999, 2004, 2017  Ossama Othman
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

#ifndef MARC_MERCATOR_H
#define MARC_MERCATOR_H

#include <MaRC/MapFactory.h>
#include <MaRC/OblateSpheroid.h>


namespace MaRC
{
  class BodyData;

  /**
   * @class Mercator
   *
   * @brief Tranverse Mercator concrete map factory.
   *
   * A Transverse Mercator map contains data mapped in a way that
   * retains shape, i.e. the projection is conformal.  The
   * "transverse" basically means that latitudes and longitudes are
   * vertical and horizontal lines, respectively.
   *
   * @note This implementation can only map oblate spheroids or
   *       spheres.
   */
  template <typename T>
  class Mercator : public MapFactory<T>
  {
  public:

    /// @typedef Type returned from @c make_map() method.
    typedef typename MapFactory<T>::map_type  map_type;

    /// @typedef Type returned from @c make_grid() method.
    typedef typename MapFactory<T>::grid_type grid_type;

    /// Constructor.
    /**
     * @param body        Pointer to BodyData object representing body
     *                    being mapped.
     */
    Mercator (const ValuePtr<OblateSpheroid> & body);

    /// Destructor
    ~Mercator (void);

    /**
     * @name @c MapFactory Methods
     *
     * Factory methods required by the @c MapFactory abstract base
     * class.
     *
     * @see @c MapFactory
     */
    //@{
    virtual MapFactory<T> * clone (void) const;
    virtual const char * projection_name (void) const;
    virtual map_type * make_map (const SourceImage & source,
                                 unsigned int samples,
                                 unsigned int lines,
                                 double minimum,
                                 double maximum);
    virtual grid_type * make_grid (unsigned int samples,
                                   unsigned int lines,
                                   float lat_interval,
                                   float lon_interval);
    //@}

  private:

    /// Orient longitude according to rotation direction
    /// (prograde/retrograde).
    /**
     * @param i       Sample in map being mapped.
     * @param samples Number of samples in image.
     */
    inline double get_longitude (unsigned int i,
                                 unsigned int samples) const;

    /// The underlying Transverse Mercator projection equation.
    /**
     * @param latg Bodygraphic latitude.
     *
     * @return Value of point on projection along a vertical axis
     *         (e.g. along a longitude line).
     */
    double mercator_x (double latg) const;

    /// Scale distortion at given bodygraphic latitude @a latg on map.
    /**
     * @param latg Bodygraphic latitude.
     */
    double distortion (double latg) const;

  private:

    /// BodyData object representing the body being mapped.
    ValuePtr<OblateSpheroid> body_;

  };

}


#include "MaRC/Mercator.cpp"

#endif
