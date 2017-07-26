//   -*- C++ -*-
/**
 * @file PhotometricCorrection.h
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

#ifndef MARC_PHOTOMETRIC_CORRECTION_H
#define MARC_PHOTOMETRIC_CORRECTION_H

#include "MaRC/ValuePtr.h"


namespace MaRC
{
  class BodyData;

  /**
   * @class PhotometricCorrection
   *
   * @brief Photometric correction strategy abstract base class.
   *
   * The @c PhotometricCorrection base class defines the interface
   * used by the @c PhotoImage class when performing photometric
   * correction in an image, such as compensating for limb
   * darkening).  All such photometric metric correction strategies
   * should inherit from this base class.
   */
  class PhotometricCorrection
  {
  public:

    /// Destructor
    virtual ~PhotometricCorrection (void);

    /// Perform photometric correction.
    /**
     * @param body           Pointer to BodyData object representing
     *                       body being mapped.
     * @param sub_observ_lat Bodycentric subobservation latitude.
     * @param sub_observ_lon Subobservation longitude.
     * @param sub_solar_lat  Bodycentric subsolar latitude.
     * @param sub_solar_lon  Subsolar longitude.
     * @param lat            Bodycentric latitude.
     * @param lon            Longitude.
     * @param range          Observer range to subobservation point.
     */
    virtual int correct (const BodyData & body,
                         double sub_observ_lat,
                         double sub_observ_lon,
                         double sub_solar_lat,
                         double sub_solar_lon,
                         double lat,
                         double lon,
                         double range,
                         double & data) = 0;

    /// Clone operation that polymorphically copies the concrete
    /// @c PhotometricCorrection object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<PhotometricCorrection> template specialization to
     * prevent the slicing that occurs when copying through a copy
     * constructor instead.
     */
    virtual PhotometricCorrection * clone (void) const = 0;

  };

  // PhotometricCorrection specialization of the ValuePtr traits
  // template.
  template<>
  struct VP_traits<PhotometricCorrection>
  {
    static PhotometricCorrection * clone (const PhotometricCorrection * p)
    {
      return p->clone ();
    }
  };

}


#endif  /* MARC_PHOTOMETRIC_CORRECTION_H */
