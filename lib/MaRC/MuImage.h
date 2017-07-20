// -*- C++ -*-

//==========================================================================
/**
 *  @file MuImage.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_MU_IMAGE_H
#define MARC_MU_IMAGE_H

#include "MaRC/VirtualImage.h"
#include "MaRC/OblateSpheroid.h"

namespace MaRC
{
  class OblateSpheroid;

  /**
   * @class MuImage
   *
   * @brief Cosine of emission angle virtual image.
   *
   * This concrete VirtualImage returns the cosine of the emission
   * angle on the body being mapped.  The observer range is taken into
   * account.
   */
  class MuImage : public VirtualImage
  {
  public:

    /// Constructor
    /**
     * @param body           @c OblateSpheroid object representing the
     *                       body being mapped.
     * @param sub_observ_lat Bodycentric sub-observer latitude in degrees.
     * @param sub_observ_lon Sub-observer longitude in degrees.
     * @param range          Observer to target center distance.
     */
    MuImage (const OblateSpheroid & body,
             double sub_observ_lat,
             double sub_observ_lon,
             double range);

    /// Underlying implementation for @c MaRC::MuImage::is_visible()
    /// method.
    static bool is_visible_i (const OblateSpheroid & body,
                              const double & sub_observ_lat,
                              const double & sub_observ_lon,
                              const double & lat,
                              const double & lon,
                              const double & range);

  private:

    /// Compute cosine of emission angle.
    /**
     * @see MaRC::VirtualImage::read_data_i().
     */
    virtual bool read_data_i (const double & lat,
                              const double & lon,
                              double & data) const;

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

    /// Bodycentric sub-observer latitude in radians.
    const double sub_observ_lat_;

    /// Sub-observer longitude in radians.
    const double sub_observ_lon_;

    /// Observer to target center distance.
    /**
     * The range is the line of sight passing through the sub-observer
     * point on the surface of the body.
     *
     * @note The unit used for the range should be consistent with the
     *       unit used for the body radii.
     */
    const double range_;

  };

} // End MaRC namespace

#endif  /* MARC_MU_IMAGE_H */
