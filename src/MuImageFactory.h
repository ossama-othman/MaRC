// -*- C++ -*-

//==========================================================================
/**
 *  @file MuImageFactory.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_MU_IMAGE_FACTORY_H
#define MARC_MU_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include "MaRC/OblateSpheroid.h"


namespace MaRC
{
  /**
   * @class MuImageFactory
   *
   * @brief Factory class that creates MuImage objects.
   *
   * This class creates MuImage objects.
   */
  class MuImageFactory : public ImageFactory
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
    MuImageFactory (const OblateSpheroid & body,
                    double sub_observ_lat,
                    double sub_observ_lon,
                    double range);

    /// Create a @c MuImage.
    virtual SourceImage * make (void);

    /// Clone operation that polymorphically copies this
    /// @c MuImageFactory object.
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

    /// Center of body distance to observer (Kilometers)
    double range_;

  };

}

#endif  /* MARC_MU_IMAGE_FACTORY_H */
