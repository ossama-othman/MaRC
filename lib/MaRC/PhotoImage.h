//   -*- C++ -*-

//==========================================================================
/**
 *  @file PhotoImage.h
 *
 *  $Id: PhotoImage.h,v 1.12 2005/11/14 08:53:44 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================


#ifndef MARC_PHOTO_IMAGE_H
#define MARC_PHOTO_IMAGE_H

#include <MaRC/SourceImage.h>
#include <MaRC/PhotoImageParameters.h>
#include <MaRC/Geometry.h>
#include <MaRC/OblateSpheroid.h>
#include <MaRC/Image.h>
#include <MaRC/GeometricCorrection.h>
#include <MaRC/PhotometricCorrection.h>
#include <MaRC/InterpolationStrategy.h>
#include <MaRC/Matrix.h>

#include <string>

namespace MaRC
{
  /**
   * @class PhotoImage
   *
   * @brief Concrete SourceImage strategy for mapping bodies in
   *        photos.
   *
   * PhotoImages have viewing geometries that may differ from other
   * photos of the same body being mapped.  For example, photos from
   * telescope observations fit into this category.
   */
  class PhotoImage : public SourceImage, PhotoImageFlagsWrapper
  {
  public:
    /// Constructor
    /**
     * @param body    Pointer to OblateSpheroid object representing
     *                body being mapped.
     * @param image   Array containing the image data.
     * @param samples Number of samples in the image.
     * @param lines   Number of lines   in the image.
     * @param gc      Geometric correction strategy.  PhotoImage
     *                assumes ownership.
     */
    PhotoImage (PhotoImageParameters & params);

    /// Destructor.
    virtual ~PhotoImage (void);

    /// Supersampling verification.
    virtual void check_image_unread_mask (void) const;

    /// Less than operator.
    bool operator< (const PhotoImage & /* img */) { return false; }

    /// Equality operator.
    bool operator== (const PhotoImage & img);

    /// Return left nibble value.
    unsigned int nibble_left   (void) const { return this->nibble_left_;   }

    /// Return right nibble value.
    unsigned int nibble_right  (void) const { return this->nibble_right_;  }

    /// Return top nibble value.
    unsigned int nibble_top    (void) const { return this->nibble_top_;    }

    /// Return bottom nibble value.
    unsigned int nibble_bottom (void) const { return this->nibble_bottom_; }

    /// Retrieve data from source image.
    /**
     * Retrieve data from source image. The configured data
     * interpolation strategy will be applied.
     *
     * @param lat  Bodycentric (e.g. planetocentric) latitude in radians.
     * @param lon  Longitude in radians.
     *
     * @param[out] data Data retrieved from image.
     *
     * @return 0 - Data retrieved, 1 - No data retrieved.
     */
    virtual bool read_data (const double & lat,
                            const double & lon,
                            double & data);

    /// Retrieve data and weight from source image.
    /**
     * Retrieve data and weight from source image.  The configured
     * data interpolation strategy will be applied.
     *
     * @param lat    Bodycentric (e.g. planetocentric) latitude in radians.
     * @param lon    Longitude in radians.
     * @param data   Data retrieved from image.
     * @param weight Distance from pixel to closest edge or blank pixel.
     * @param scan   Flag that determines if a data weight scan is
     *               performed.  It is generally only used by the
     *               version of read_data() that does not return a
     *               weight.
     *
     * @return 0 - Data retrieved, 1 - No data retrieved.
     */
    virtual bool read_data (const double & lat,
                            const double & lon,
                            double & data,
                            unsigned int & weight,
                            bool scan = true);

    /// Returns "true" if latitude and longitude are visible
    bool is_visible (const double & lat, const double & lon) const;


    /// Convert (Latitude, Longitude) to (Sample, Line)
    /**
     * @param lat Bodycentric (e.g. planetocentric) latitude  in radians.
     * @param lon Longitude in radians.
     * @param x   Floating point value corresponding to @c i.
     * @param z   Floating point value corresponding to @c k.
     *
     * @note Since @c x and @c z potentially include fractional pixel
     *       components, they are more accurate than their integer
     *       counterparts.
     */
    void latlon2pix (const double & lat,
                     const double & lon,
                     double & x,
                     double & z) const;

  private:

    // Filename for the source of the image.
    const std::string filename_;

    /// Object representing the body being mapped.
    /**
     * @note OblateSpheroid is used instead of BodyData since some
     *       code in this implementation assumes that the body is
     *       modeled as an oblate spheroid.
     */
    const OblateSpheroid body_;

    /// Pointer to the image array.
    const Image<double> image_;

    /// Number of samples in the image.
    const unsigned int samples_;

    /// Number of lines in the image.
    const unsigned int lines_;

    /// Geometric/optical correction strategy used during
    /// latitude/longitude to pixel conversion, and vice versa.
    const ValuePtr<GeometricCorrection> geometric_correction_;

    /// Pointer to the photometric correction strategy.
    const ValuePtr<PhotometricCorrection> photometric_correction_;

    /// Pointer to the photometric correction strategy.
    const ValuePtr<InterpolationStrategy> interpolation_strategy_;

    /// Sub-Observer Latitude -- BodyCENTRIC (radians).
    const double sub_observ_lat_;

    /// Sub-Observer Longitude -- Central Meridian (radians).
    const double sub_observ_lon_;

    /// Sub-Solar Latitude -- BodyCENTRIC (radians)
    const double sub_solar_lat_;

    /// Sub-Solar Longitude (radians)
    const double sub_solar_lon_;

    /// Center of body distance to observer (Kilometers)
    const double range_;

    /// Position angle in the image NOT in the sky.
    const double position_angle_;

    /// Kilometers per pixel at plane that passes through body
    /// center.
    double km_per_pixel_;

    /// Focal length in millimeters.
    const double focal_length_;

    /// Focal length in pixels
    /**
     * @note Any distance unit may be used for the focal length and
     *       the scale as long as BOTH use the SAME distance unit.
     *       (e.g. cm and pixels/cm).
     */
    const double focal_length_pixels_;

    /// pixels/mm at focal plane.
    /**
     * @see @c focal_length_pixels_
     */
    const double scale_;

    /**
     * @name Optical Axis
     *
     * The location of the optical axis (boresight) on the photo.
     */
    //@{
    /// Sample component of optical axis.
    const double OA_s_;

    /// Line component of optical axis.
    const double OA_l_;
    //@}

    /// Mask used when "removing" sky from source image.
    /**
     * A mask is used to mark which pixels in the photo are in the
     * body and which are in the sky without actually modifying the
     * original source photo.
     *
     * @note The sky mask is generally only useful when performing
     *       weighted averaging in a mosaiced image.
     *
     * @see MosaicImage
     */
    const Image<bool> sky_mask_;

    // Mask tracking which pixels in the image were read.
    Image<bool> image_unread_mask_;

    /// Range vector in body coordinates, measured from the center of
    /// the body to the observer.
    const DVector range_b_;

    /// Transformation matrix to go from observer to body
    /// coordinates.
    const DMatrix observ2body_;

    /// Transformation matrix to go from body to observer
    /// coordinates.
    const DMatrix body2observ_;

    /// Amount of pixels to ignore from left side of input image
    /// (photo).
    const unsigned int nibble_left_;

    /// Amount of pixels to ignore from right side of input image
    /// (photo).
    const unsigned int nibble_right_;

    /// Amount of pixels to ignore from top side of input image
    /// (photo).
    const unsigned int nibble_top_;

    /// Amount of pixels to ignore from bottom side of input image
    /// (photo).
    const unsigned int nibble_bottom_;

    /**
     * @name Object-Space Body Center
     *
     * Object space (e.g. corrected for lens aberration) center of
     * body in the input image/photo.
     */
    //@{
    /// Horizontal center of body in photo.
    const double sample_center_;

    /// Vertical center of body in photo.
    const double line_center_;
    //@}

    /// BODYcentric latitude at picture center.
    const double lat_at_center_;
	
    /// Longitude at picture center
    const double lon_at_center_;

    /// Cosine of emission angle outside of which no data will be
    /// plotted/retrieved.
    const double mu_limit_;

//     /// Minimum latitude visible on body.
//     double min_lat_;

//     /// Maximum latitude visible on body.
//     double max_lat_;

//     /// Minimum longitude visible on body
//     double min_lon_;

//     /// Maximum longitude visible on body
//     double max_lon_;

    /// bit set used to keep track of which internal flags are set.
    const unsigned long flags_;
  };

}


#endif  /* MARC_PHOTO_IMAGE_H */
