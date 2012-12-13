// -*- C++ -*-

//==========================================================================
/**
 *  @file Mu0Image.h
 *
 *  $Id: Mu0Image.h,v 1.3 2004/07/06 00:01:41 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_MU0_IMAGE_H
#define MARC_MU0_IMAGE_H

#include "MaRC/VirtualImage.h"
#include "MaRC/OblateSpheroid.h"

namespace MaRC
{

  class OblateSpheroid;

  /**
   * @class Mu0Image
   *
   * @brief Cosine of the incidence angle virtual image.
   *
   * This concrete VirtualImage returns the cosine of the
   * sun-local-normal (incidence) angle on the body being mapped.  The
   * sun is assumed to be an infinite distance away.
   *
   * @note This implementation requires that the body under
   *       observation is modeled as an oblate spheroid.
   */
  class Mu0Image : public VirtualImage
  {
  public:

    /// Constructor
    /**
     * @param body          @c OblateSpheroid object representing the
     *                      body being mapped.
     * @param sub_solar_lat Bodycentric sub-solar latitude in degrees.
     * @param sub_solar_lon Sub-solar longitude in degrees.
     */
    Mu0Image (const OblateSpheroid & body,
              double sub_solar_lat,
              double sub_solar_lon);

  private:

    /// Compute cosine of the incidence angle.
    /**
     * @see MaRC::VirtualImage::read_data_i().
     */
    virtual bool read_data_i (const double & Latitude,
                              const double & Longitude,
                              double & Data) const;


    /// Is point at given latitude and longitude visible to the
    /// observer?
    /**
     * @see MaRC::VirtualImage::is_visible().
     */
    virtual bool is_visible (const double & lat,
                             const double & lon) const;

  private:

    /// Object representing the body being mapped.
    /**
     * @note OblateSpheroid is used instead of BodyData since some
     *       code in this implementation assumes that the body is
     *       modeled as an oblate spheroid.
     */
    const OblateSpheroid body_;

    /// Bodycentric sub-solar latitude in radians.
    const double sub_solar_lat_;

    /// Sub-solar longitude in radians.
    const double sub_solar_lon_;

  };

} // End MaRC namespace

#endif  /* MARC_MU0_IMAGE_H */
