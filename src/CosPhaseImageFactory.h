// -*- C++ -*-

//==========================================================================
/**
 *  @file CosPhaseImageFactory.h
 *
 *  $Id: CosPhaseImageFactory.h,v 1.1 2004/07/03 10:59:42 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

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
