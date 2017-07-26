// -*- C++ -*-
/**
 * @file CosPhaseImageFactory.h
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

#ifndef MARC_COS_PHASE_IMAGE_FACTORY_H
#define MARC_COS_PHASE_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include "MaRC/OblateSpheroid.h"

#include <string>


namespace MaRC
{
  /**
   * @class CosPhaseImageFactory
   *
   * @brief Factory class that creates CosPhaseImage objects.
   *
   * This class creates CosPhaseImage objects.
   */
  class CosPhaseImageFactory : public ImageFactory
  {
  public:

    /// Constructor.
    /**
     * @param body Reference to body being mapped (currently must be
     *             represented as an oblate spheroid).
     */
    CosPhaseImageFactory (const OblateSpheroid & body,
                          double sub_observ_lat,
                          double sub_observ_lon,
                          double sub_solar_lat,
                          double sub_solar_lon,
                          double range);

    /// Create a @c CosPhaseImage.
    virtual SourceImage * make (void);

    /// Clone operation that polymorphically copies this
    /// @c CosPhaseImageFactory object.
    virtual ImageFactory * clone (void) const;

  private:

    /// Object representing the body being mapped.
    /**
     * @note OblateSpheroid is used instead of BodyData since some
     *       code in this implementation assumes that the body is
     *       modeled as an oblate spheroid.
     */
    OblateSpheroid body_;

    /// Sub-Observer Latitude -- BodyCENTRIC (radians).
    double sub_observ_lat_;

    /// Sub-Observer Longitude -- Central Meridian (radians).
    double sub_observ_lon_;

    /// Sub-Solar Latitude -- BodyCENTRIC (radians)
    double sub_solar_lat_;

    /// Sub-Solar Longitude (radians)
    double sub_solar_lon_;

    /// Center of body distance to observer (Kilometers)
    double range_;

  };

}


#endif  /* MARC_COS_PHASE_IMAGE_FACTORY_H */
