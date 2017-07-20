// -*- C++ -*-

//==========================================================================
/**
 *  @file LongitudeImageFactory.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_LONGITUDE_IMAGE_FACTORY_H
#define MARC_LONGITUDE_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include "MaRC/OblateSpheroid.h"


namespace MaRC
{
  /**
   * @class LongitudeImageFactory
   *
   * @brief Factory class that creates LongitudeImage objects.
   *
   * This class creates LongitudeImage objects.
   */
  class LongitudeImageFactory : public ImageFactory
  {
  public:

    /// Create a @c LongitudeImage.
    virtual SourceImage * make (void);

    /// Clone operation that polymorphically copies this
    /// @c LongitudeImageFactory object.
    virtual ImageFactory * clone (void) const;

  };

}

#endif  /* MARC_LONGITUDE_IMAGE_FACTORY_H */
