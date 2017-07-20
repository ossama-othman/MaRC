// -*- C++ -*-

//==========================================================================
/**
 *  @file LongitudeImage.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_LONGITUDE_IMAGE_H
#define MARC_LONGITUDE_IMAGE_H

#include "MaRC/VirtualImage.h"


namespace MaRC
{

  /**
   * @class LongitudeImage
   *
   * @brief Longitude virtual image.
   *
   * This concrete VirtualImage returns the given longitude in
   * degrees.
   */
  class LongitudeImage : public VirtualImage
  {
  private:

    /// Simply return the given longitude in degrees.
    /**
     * @see MaRC::VirtualImage::read_data_i().
     */
    virtual bool read_data_i (const double & lat,
                              const double & lon,
                              double & data) const;

  };

} // End MaRC namespace

#endif  /* MARC_LONGITUDE_IMAGE_H */
