// -*- C++ -*-

//==========================================================================
/**
 *  @file PhotoInterpolationStrategy.h
 *
 *  $Id: PhotoInterpolationStrategy.h,v 1.3 2005/11/09 18:58:57 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================


#ifndef MARC_PHOTO_INTERPOLATION_STRATEGY_H
#define MARC_PHOTO_INTERPOLATION_STRATEGY_H

#include "InterpolationStrategy.h"

namespace MaRC
{

  /**
   * @class PhotoInterpolationStrategy
   *
   * @brief Bilinear interpolation strategy for PhotoImages.
   *
   * This strategy performs bilinear interpolation over 2x2 area of
   * pixels.
   */
  class PhotoInterpolationStrategy : public InterpolationStrategy
  {
  public:

    /// Destructor.
    virtual ~PhotoInterpolationStrategy (void);

    /// Constructor
    /**
     * @param samples        Number of samples in image.
     * @param lines          Number of lines   in image.
     * @param nibble_left    Left   nibble value.
     * @param nibble_right   Right  nibble value.
     * @param nibble_top     Top    nibble value.
     * @param nibble_bottom  Bottom nibble value.
     */
    PhotoInterpolationStrategy (unsigned int samples,
                                unsigned int lines,
                                unsigned int nibble_left,
                                unsigned int nibble_right,
                                unsigned int nibble_top,
                                unsigned int nibble_bottom);

    /// Perform bilinear interpolation over a 2x2 area of pixels on
    /// the given pixel.
    virtual bool interpolate (const double * image,
                              double x,
                              double z,
                              double & data) const;

    /// Copy this PhotoInterpolationStrategy object.
    virtual InterpolationStrategy * clone (void) const;

  private:

    /// Number of samples in image.
    const unsigned int samples_;

    /// Left most sample in image.
    const unsigned int left_;

    /// Right most sample in image.
    const unsigned int right_;

    /// Top most line in image.
    const unsigned int top_;

    /// Bottom most line in image.
    const unsigned int bottom_;

  };

}

#endif  /* MARC_PHOTO_INTERPOLATION_STRATEGY_H */
