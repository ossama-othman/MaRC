// -*- C++ -*-

//==========================================================================
/**
 *  @file MosaicImage.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_MOSAIC_IMAGE_H
#define MARC_MOSAIC_IMAGE_H

#include <MaRC/SourceImage.h>
#include <MaRC/PhotoImage.h>

#include <vector>


namespace MaRC
{
  /**
   * @class MosaicImage
   *
   * @brief Source image comprised of multiple @c PhotoImages.
   *
   * Mosaics may be comprised of multiple photographs, each taken at
   * different viewing geometries.
   */
  class MosaicImage : public SourceImage
  {
  public:

    typedef std::vector<PhotoImage> list_type;

    /**
     * @enum AverageType
     *
     * The type of averaging to be performed on data retrieved from
     * multiple images that contain data at the given latitude and
     * longitude.
     */
    enum average_type { AVG_NONE, AVG_UNWEIGHTED, AVG_WEIGHTED };

    /// Constructor.
    /**
     * @param images The set of images to be mosaiced.
     * @param type   The type of averaging to be performed.
     */
    MosaicImage (const list_type & images, average_type type);

    /// Retrieve data from mosaic images.
    /**
     * Retrieve data from all mosaic images that have data at the
     * given latitude and longitude.  The configured data averaging
     * strategy will be applied in cases where multiple images have
     * data at the given longitude and latitude.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude in radians.
     * @param lon  Longitude in radians.
     *
     * @param[out] data Data retrieved from image.
     *
     * @return @c true - Data retrieved, @c false - No data retrieved.
     */
    virtual bool read_data (const double & lat,
                            const double & lon,
                            double & data) const;

  private:

    /// Should the given @a data value be considered zero?
    /**
     * Since a zero floating point value is not always represented as
     * exactly zero (e.g. as the result of a computation), interpret
     * values less than some threshold to be zero.
     *
     * @return @c true if @a data is considered to be zero.
     */
    static bool is_zero (double data);

  private:

    /// Set of images
    const list_type images_;

    /// The type of averaging to perform on data retrieved from
    /// multiple images.
    const average_type average_type_;

  };

} // End MaRC namespace

#endif  /* MARC_SOURCE_IMAGE_H */
