// -*- C++ -*-
/**
 * @file PolarStereographic.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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

#ifndef MARC_POLAR_STEREOGRAPHIC_H
#define MARC_POLAR_STEREOGRAPHIC_H

#include <MaRC/MapFactory.h>
#include <MaRC/OblateSpheroid.h>


namespace MaRC
{
  class BodyData;

  /**
   * @class PolarStereographic
   *
   * @brief Polar Stereographic concrete map factory.
   *
   * A Polar Stereographic map contains data mapped in a way that
   * retains shape, i.e. the projection is conformal.  One of the
   * poles will be at the center of the map.
   *
   * @note This implementation can only map oblate spheroids or
   *       spheres.
   */
  template <typename T>
  class PolarStereographic : public MapFactory<T>
  {
  public:

    /// @typedef Type returned from @c make_map() method.
    typedef typename MapFactory<T>::map_type  map_type;

    /// @typedef Type returned from @c make_grid() method.
    typedef typename MapFactory<T>::grid_type grid_type;

    /// Constructor.
    /**
     * @param body       Pointer to BodyData object representing body
     *                   being mapped.
     * @param max_lat    Maximum bodyCENTRIC latitude to map in degrees.
     * @param north_pole Place north_pole if @c true.  South pole
     *                   otherwise.
     */
    PolarStereographic (const ValuePtr<OblateSpheroid> & body,
                        double max_lat,
                        bool north_pole);

    /// Destructor
    ~PolarStereographic (void);

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

    /// The underlying Polar Stereographic projection equation.
    /**
     * @param latg Bodygraphic latitude.
     *
     * @return Value of point on projection along a radial line
     *         (e.g. along a longitude line).
     */
    double stereo_rho (double latg) const;

    /// Scale distortion at given bodygraphic latitude @a latg on map.
    /**
     * @param latg Bodygraphic latitude.
     */
    double distortion (double latg) const;

  private:

    /// BodyData object representing the body being mapped.
    ValuePtr<OblateSpheroid> body_;

    /// Maximum bodyCENTRIC latitude to map in radians.
    double const max_lat_;

    /// Coefficient used in map equation.
    double const rho_coeff_;

    /// Coefficient used in scale distortion equation.
    double const distortion_coeff_;

    /// @c true if north pole is at center of map.  @c false if south
    /// pole is at center.
    bool const north_pole_;

  };

}


#include "MaRC/PolarStereographic.cpp"

#endif
