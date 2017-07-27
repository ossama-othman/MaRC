// -*- C++ -*-
/**
 * @file SimpleCylindrical.h
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017  Ossama Othman
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


#ifndef MARC_SIMPLE_CYLINDRICAL_H
#define MARC_SIMPLE_CYLINDRICAL_H

#include <MaRC/MapFactory.h>


namespace MaRC
{
  class BodyData;

  /**
   * @class SimpleCylindrical
   *
   * @brief Simple cylindrical concrete map factory.
   *
   * A Simple cylindrical map contains data mapped to a rectangular
   * latitude/longitude array.
   */
  template <typename T>
  class SimpleCylindrical : public MapFactory<T>
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
     * @param lo_lat      Lower latitude  in simple cylindrical map.
     * @param hi_lat      Upper latitude  in simple cylindrical map.
     * @param lo_lon      Lower longitude in simple cylindrical map.
     * @param hi_lon      Upper longitude in simple cylindrical map.
     * @param graphic_lat Map bodygraphic latitudes instead of
     *                    bodycentric latitudes.
     */
    SimpleCylindrical (const ValuePtr<BodyData> & body,
                       double lo_lat,
                       double hi_lat,
                       double lo_lon,
                       double hi_lon,
                       bool graphic_lat);

    /// Destructor
    ~SimpleCylindrical (void);

    /**
     * @name @c MapFactory Methods
     *
     * Factory methods required by the @c MapFactory abstract base
     * class.
     *
     * @see @c MapFactory
     */
    //@{
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

  private:

    /// BodyData object representing the body being mapped.
    ValuePtr<BodyData> body_;

    /// Lower latitude in simple cylindrical map.
    double lo_lat_;

    /// Upper latitude in simple cylindrical map.
    double hi_lat_;

    /// Lower longitude in simple cylindrical map.
    double lo_lon_;

    /// Upper longitude in simple cylindrical map.
    double hi_lon_;

    /// Flag that determines if bodygraphic latitudes are mapped
    /// instead of bodycentric latitudes.
    const bool graphic_lat_;

  };

}


#include "MaRC/SimpleCylindrical.cpp"

#endif
