// -*- C++ -*-

//==========================================================================
/**
 *  @file SourceImage.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_SOURCE_IMAGE_H
#define MARC_SOURCE_IMAGE_H

namespace MaRC
{
  /**
   * @class SourceImage
   *
   * @brief Abstract base class for all images to be mapped.
   *
   * Concrete source image classes must implement the interface
   * required by this abstract base class.
   */
  class SourceImage
  {
  public:

    /// Destructor.
    virtual ~SourceImage (void);

    /// Retrieve data from source image.
    /**
     * Retrieve data from source image.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude in radians.
     * @param lon  Longitude in radians.
     *
     * @param[out] data Data retrieved from image.
     *
     * @return @c true - Data retrieved, @c false - No data retrieved.
     */
    virtual bool read_data (double lat,
                            double lon,
                            double & data) const = 0;

  };

} // End MaRC namespace

#endif  /* MARC_SOURCE_IMAGE_H */
