//   -*- C++ -*-
/**
 * @file Orthographic.h
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

#ifndef MARC_ORTHOGRAPHIC_H
#define MARC_ORTHOGRAPHIC_H

#include <MaRC/MapFactory.h>


namespace MaRC
{
  class OblateSpheroid;
  struct OrthographicCenter;

  enum GeometryType {DEFAULT, CENTER_GIVEN, LAT_LON_GIVEN};

  /**
   * @class Orthographic
   *
   * @brief Orthographic concrete map factory.
   *
   * This class creates orthographic map projections.  Observers in
   * orthographic projections are an infinite distance away (range
   * approaches infinity).
   *
   * @note Only bodies modeled as oblate spheroids are supported by
   *       this implementation.
   */
  template <typename T>
  class Orthographic : public MapFactory<T> {
  public:

    typedef typename MapFactory<T>::map_type  map_type;
    typedef typename MapFactory<T>::grid_type grid_type;

    /// Constructor.
    /**
     * @param body           Pointer to OblateSpheroid object
     *                       representing body.
     * @param sub_observ_lat Bodycentric sub-observer latitude in degrees.
     * @param sub_observ_lon Sub-observer longitude in degrees.
     * @param position_angle Position (north) angle of body in
     *                       orthographic projection.
     * @param km_per_pixel   Number of kilometer per pixel in
     *                       orthographic projection.
     * @param center         Structure containing body center
     *                       information to be used in the
     *                       projection.
     */
    Orthographic (const ValuePtr<OblateSpheroid> & body,
                  double sub_observ_lat,
                  double sub_observ_lon,
                  double position_angle,
                  double km_per_pixel,
                  const OrthographicCenter & center);

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

    /// Initialize all attributes.
    void init (unsigned int samples, unsigned int lines);

  private:

    /// OblateSpheroid object representing the body being mapped.
    ValuePtr<OblateSpheroid> body_;

    /// Sub-observation latitude.
    double sub_observ_lat_;

    /// Sub-observation longitude.
    double sub_observ_lon_;

    /// Map position angle (measured counter-clockwise positive).
    double PA_;

    /// The number of kilometers per pixel in the orthographic
    /// projection.
    double km_per_pixel_;

    /// Body center sample in projection (measured from left edge).
    double sample_center_;

    /// Body center line in projection (measured from bottom edge).
    double line_center_;

    /// Latitude at center of projection (measured from left edge).
    double lat_at_center_;

    /// Line center of projection (measured from bottom edge).
    double lon_at_center_;

    /// True if creating polar projection.
    bool polar_;

  };

  /**
   * @struct OrthographicCenter
   *
   * @brief Center of body in orthographic projection.
   *
   * These values dictate where the center of the body being mapped
   * will be placed in the orthographic projection.
   */
  struct OrthographicCenter
  {
    OrthographicCenter (void);

    /// Type of body center geometry.
    GeometryType geometry;

    /// Sample or latitude at center of body.
    double sample_lat_center;

    /// Line or longitude at center of body.
    double line_lon_center;

  };

}

#include "MaRC/Orthographic.cpp"


#endif  /* MARC_ORTHOGRAPHIC_H */
