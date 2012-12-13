// -*- C++ -*-

//==========================================================================
/**
 *  @file MosaicImageFactory.h
 *
 *  $Id: MosaicImageFactory.h,v 1.1 2004/07/03 10:59:42 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_MOSAIC_IMAGE_FACTORY_H
#define MARC_MOSAIC_IMAGE_FACTORY_H

#include "PhotoImageFactory.h"

#include "MaRC/MosaicImage.h"

#include <list>

namespace MaRC
{
  /**
   * @class MosaicImageFactory
   *
   * @brief Factory class that create MosaicImage objects.
   *
   * This class creates MosaicImage objects.  It is designed to
   * decouple FITS (for example) file and image operations from the
   * MosaicImage class.  It also exists to decouple the MaRC parser
   * grammar from the MosaicImage class.  This allows MosaicImage
   * object creation to be delayed until it is time for the data in
   * the MosaicImage to be mapped, which reduces run-time memory
   * requirements.
   */
  class MosaicImageFactory : public ImageFactory
  {
  public:

    typedef std::list<PhotoImageFactory> list_type;

    /// Constructor.
    MosaicImageFactory (const list_type & factories,
                        MosaicImage::average_type type);

    /// Create a @c MosaicImage.
    virtual SourceImage * make (void);

    /// Clone operation that polymorphically copies this
    /// @c MosaicImageFactory object.
    virtual ImageFactory * clone (void) const;

  private:

    /// List of PhotoImageFactorys
    list_type factories_;

    /// The type of averaging to be performed when multiple images
    /// overlap.
    const MosaicImage::average_type average_type_;

  };

}

#endif  /* MARC_MOSAIC_IMAGE_FACTORY_H */
