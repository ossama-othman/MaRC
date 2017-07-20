// -*- C++ -*-

//==========================================================================
/**
 *  @file LatitudeImageFactory.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_LATITUDE_IMAGE_FACTORY_H
#define MARC_LATITUDE_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include "MaRC/OblateSpheroid.h"


namespace MaRC
{
  /**
   * @class LatitudeImageFactory
   *
   * @brief Factory class that creates LatitudeImage objects.
   *
   * This class creates LatitudeImage objects.
   */
  class LatitudeImageFactory : public ImageFactory
  {
  public:

    /// Constructor.
    /**
     * @param body              Pointer to BodyData object
     *                          representing body being mapped.
     *
     * @param graphic_latitudes Return bodygraphic latitudes instead
     *                          of bodycentric latitudes.
     */
    LatitudeImageFactory (const ValuePtr<BodyData> & body,
                          bool graphic_latitudes);

    /// Create a @c LatitudeImage.
    virtual SourceImage * make (void);

    /// Clone operation that polymorphically copies this
    /// @c LatitudeImageFactory object.
    virtual ImageFactory * clone (void) const;

  private:

    /// Object representing the body being mapped.
    const ValuePtr<BodyData> body_;

    /// Flag that determines if bodygraphic latitudes are returned
    /// instead of bodycentric latitudes.
    const bool graphic_latitudes_;

  };

}

#endif  /* MARC_LATITUDE_IMAGE_FACTORY_H */
