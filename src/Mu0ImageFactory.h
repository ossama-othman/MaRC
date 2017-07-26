// -*- C++ -*-
/**
 * @file Mu0ImageFactory.h
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

#ifndef MARC_MU0_IMAGE_FACTORY_H
#define MARC_MU0_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include "MaRC/OblateSpheroid.h"


namespace MaRC
{
  /**
   * @class Mu0ImageFactory
   *
   * @brief Factory class that creates Mu0Image objects.
   *
   * This class creates Mu0Image objects.
   */
  class Mu0ImageFactory : public ImageFactory
  {
  public:

    /// Constructor.
    /**
     * @param body Reference to body being mapped (currently must be
     *             represented as an oblate spheroid).
     * @param sub_observ_lat Bodycentric sub-observer latitude in radians
     * @param sub_observ_lon Sub-observer longitude in radians
     * @param range          Observer to target center distance
     */
    Mu0ImageFactory (const OblateSpheroid & body,
                     double sub_solar_lat,
                     double sub_solar_lon);

    /// Create a @c Mu0Image.
    virtual SourceImage * make (void);

    /// Clone operation that polymorphically copies this
    /// @c Mu0ImageFactory object.
    virtual ImageFactory * clone (void) const;

  private:

    /// Object representing the body being mapped.
    /**
     * @note OblateSpheroid is used instead of BodyData since some
     *       code in this implementation assumes that the body is
     *       modeled as an oblate spheroid.
     */
    OblateSpheroid body_;

    /// Sub-Solar Latitude -- BodyCENTRIC (radians).
    double sub_solar_lat_;

    /// Sub-Solar Longitude -- Central Meridian (radians).
    double sub_solar_lon_;

  };

}


#endif  /* MARC_MU0_IMAGE_FACTORY_H */
