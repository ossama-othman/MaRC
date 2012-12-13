// -*- C++ -*-

//==========================================================================
/**
 *  @file LatitudeImage.h
 *
 *  $Id: LatitudeImage.h,v 1.2 2004/07/03 10:59:42 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_LATITUDE_IMAGE_H
#define MARC_LATITUDE_IMAGE_H

#include "MaRC/VirtualImage.h"
#include "MaRC/BodyData.h"
#include "MaRC/ValuePtr.h"


namespace MaRC
{

  /**
   * @class LatitudeImage
   *
   * @brief Latitude virtual image.
   *
   * This concrete VirtualImage returns the given latitude in
   * degrees.  This class may be configured to return bodygraphic
   * latitudes instead of bodycentric latitudes.
   */
  class LatitudeImage : public VirtualImage
  {
  public:

    /// Constructor
    /**
     * @param body              Pointer to BodyData object
     *                          representing body being mapped.
     *
     * @param graphic_latitudes Return bodygraphic latitudes instead
     *                          of bodycentric latitudes.
     */
    LatitudeImage (const ValuePtr<BodyData> & body,
                   bool graphic_latitudes);

  private:

    /// Simply return the given latitude in degrees.
    /**
     * @see MaRC::VirtualImage::read_data_i().
     */
    virtual bool read_data_i (const double & lat,
                              const double & lon,
                              double & data) const;

  private:

    /// Object representing the body being mapped.
    const ValuePtr<BodyData> body_;

    /// Flag that determines if bodygraphic latitudes are returned
    /// instead of bodycentric latitudes.
    const bool graphic_latitudes_;

  };

} // End MaRC namespace

#endif  /* MARC_LATITUDE_IMAGE_H */
