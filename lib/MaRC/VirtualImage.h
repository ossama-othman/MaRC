// -*- C++ -*-

//==========================================================================
/**
 *  @file VirtualImage.h
 *
 *  $Id: VirtualImage.h,v 1.3 2004/07/06 00:01:41 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_VIRTUAL_IMAGE_H
#define MARC_VIRTUAL_IMAGE_H

#include "MaRC/SourceImage.h"


namespace MaRC
{

  /**
   * @class VirtualImage
   *
   * @brief Base class for virtual source images.
   *
   * Data from virtual images are computed at run-time rather than
   * retrieved from static sources such as images stored on a
   * filesystem.
   */
  class VirtualImage : public SourceImage
  {
  public:

    /// Constructor.
    /**
     * @param s Linear scaling coefficient applied to computed data.
     * @param o Linear offset value applied to all (scaled) computed
     *          data.
     */
    VirtualImage (double s = 1, double o = 0);

    /// Retrieve data from virtual image.
    /**
     * Retrieve data from virtual image and apply configured data
     * transformations, if any.  Raw data is computed/retrieved from
     * the read_data_i() template method.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude in radians.
     * @param lon  Longitude in radians.
     *
     * @param[out] data Data retrieved from image.
     *
     * @return @c true - Data retrieved, @c false - No data retrieved.
     *
     * @see read_data_i()
     */
    virtual bool read_data (const double & lat,
                            const double & lon,
                            double & data);

  private:

    /// Compute data specific to a given virtual image.
    /**
     * This template method is the core implementation of the
     * read_data() method.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude  in radians.
     * @param lon  Longitude in radians.
     * @param data Data retrieved from image.
     *
     * @return @c true - Data retrieved, @c false - No data retrieved.
     *
     * @see read_data().
     */
    virtual bool read_data_i (const double & lat,
                              const double & lon,
                              double & data) const = 0;


    /// Is point at given latitude and longitude visible to the
    /// observer?
    /**
     * @param lat Bodycentric (e.g. planetocentric) latitude in radians
     * @param lon Longitude in radians
     *
     * @return @c true - Point is visible, @c false - Point is not visible
     *
     * @note The default implementation always returns @c true.
     */
    virtual bool is_visible (const double & lat,
                             const double & lon) const;


    /**
     * @name Linear Data Transformation
     *
     * These methods return the values used when linearly transforming
     * thedata read from an image.  In particular, data will be
     * transformed according to the following equation:
     *
     * @code
     *    data = data * scale() + offset()
     * @endcode
     *
     * The default implementation returns values that causes no
     * transformation to occur.
     */
    //@{
    /// Data scale.
    /**
     * Linear scaling coefficient applied to computed data.
     *
     * @return Data scale
     */
    virtual double scale (void) const;

    /// Data offset
    /**
     * Linear offset value applied to all (scaled) computed data.
     *
     * @return Data offset
     */
    virtual double offset (void) const;
    //@}

  private:

    /// Linear scaling coefficient applied to computed data.
    double scale_;

    /// Linear offset value applied to all (scaled) computed data.
    double offset_;

  };

} // End MaRC namespace

#endif  /* MARC_VIRTUAL_IMAGE_H */
