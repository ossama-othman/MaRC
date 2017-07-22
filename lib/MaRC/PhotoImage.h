//   -*- C++ -*-

//==========================================================================
/**
 *  @file PhotoImage.h
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_PHOTO_IMAGE_H
#define MARC_PHOTO_IMAGE_H

#include <MaRC/SourceImage.h>
#include <MaRC/Geometry.h>
#include <MaRC/OblateSpheroid.h>
#include <MaRC/Image.h>
#include <MaRC/GeometricCorrection.h>
#include <MaRC/PhotometricCorrection.h>
#include <MaRC/InterpolationStrategy.h>
#include <MaRC/Matrix.h>

namespace MaRC
{
  class OblateSpheroid;

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
  class PhotoImage : public SourceImage
  {
  public:

    /**
     * @enum sflags
     *
     * Flags that state which PhotoImage attributes have been set.
     */
    enum sflags
      {
        EMI_ANG_LIMIT    = 1 << 0,
        LATLON_AT_CENTER = 1 << 1,
        OA_SET           = 1 << 2,
        EXTREMA_SET      = 1 << 3,
        USE_TERMINATOR   = 1 << 4
      };

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
    PhotoImage (OblateSpheroid const & body,
                double * image,
                unsigned int samples,
                unsigned int lines,
                GeometricCorrection * gc);

    /// Destructor.
    virtual ~PhotoImage (void);

    /// Less than operator.
    bool operator< (const PhotoImage & /* img */) { return false; }

    /// Equality operator.
    bool operator== (const PhotoImage & img);

    /// Set sky removal variable
    /**
     * Enabling sky removal prevents data believed (i.e. computed) to
     * be in the sky rather than on the body from being mapped.
     *
     * @note The source image array will not be modified.
     *
     * @param remove @c true  == create sky removal mask,
     *               @c false == do not create sky removal mask.
     */
    void remove_sky (bool remove);

    /// Create sky removal mask.
    void remove_sky (void);

    /// Set the geometric correction strategy used during lat/lon to
    /// pixel conversion, and vice-versa.
    int geometric_correction (GeometricCorrection * g);

    /// Set the photometric correction strategy.
    int photometric_correction (PhotometricCorrection * p);

    /// Set sub-observation latitude and longitude.
    /**
     * @param lat Sub-observation latitude  (degrees)
     * @param lon Sub-observation longitude (degrees)
     */
    int sub_observ (double lat, double lon);

    /// Set Sub-Observation latitude.
    /**
     * @param lat Sub-observation latitude  (degrees)
     */
    int sub_observ_lat (double lat);

    /// Set Sub-Observation longitude.
    /**
     * @param lon Sub-observation longitude (degrees)
     */
    int sub_observ_lon (double lon);

    /// Set Sub-Solar latitude and longitude.
    /**
     * @param lat Sub-solar latitude  (degrees)
     * @param lon Sub-solar longitude (degrees)
     */
    int sub_solar (double lat, double lon);

    /// Set Sub-Solar latitude.
    /**
     * @param lat Sub-solar latitude  (degrees)
     */
    int sub_solar_lat (double lat);

    /// Set Sub-Solar longitude.
    /**
     * @param lon Sub-solar longitude (degrees)
     */
    int sub_solar_lon (double lon);

    /// Set observer to body distance (KM).
    /**
     * @param r Distance from observer to body in kilometers.
     */
    int range (double r);

    /// Set camera focal length (millimeters).
    /**
     * @param len Focal length in millimeters.
     */
    int focal_length (double len);

    /// Set input image scale (pixels / mm).
    /**
     * @param s Image scale in pixels per millimeter.
     */
    int scale (double s);

    /// Set position angle (a.k.a. North Angle in degrees) found in
    /// image.
    int position_angle (double north);

    /// Arcseconds per pixel in image.
    int arcsec_per_pixel (double a);

    /// Kilometers per pixel in image.
    int km_per_pixel (double k);

    /// Set all nibble values to @a n.
    /**
     * @param n Nibble value for all image sides.
     */
    void nibble        (unsigned int n);

    /// Set left nibble value to @a n.
    /**
     * @param n Nibble value for left image side.
     */
    void nibble_left   (unsigned int n);

    /// Set right nibble value to @a n.
    /**
     * @param n Nibble value for right image side.
     */
    void nibble_right  (unsigned int n);

    /// Set top nibble value to @a n.
    /**
     * @param n Nibble value for top image side.
     */
    void nibble_top    (unsigned int n);

    /// Set bottom nibble value to @a n.
    /**
     * @param n Nibble value for bottom image side.
     */
    void nibble_bottom (unsigned int n);

    /// Return left nibble value.
    unsigned int nibble_left   (void) const { return this->nibble_left_;   }

    /// Return right nibble value.
    unsigned int nibble_right  (void) const { return this->nibble_right_;  }

    /// Return top nibble value.
    unsigned int nibble_top    (void) const { return this->nibble_top_;    }

    /// Return bottom nibble value.
    unsigned int nibble_bottom (void) const { return this->nibble_bottom_; }

    /// Enable/disable pixel interpolation when reading data.
    void interpolate (bool enable);

    /// Set emission angle beyond which no data will be read.
    int emi_ang_limit (double angle);

    /// Set sample and line of body center.
    void body_center (double sample, double line);

    /// Set sample of body center.
    void body_center_sample (double);

    /// Set line of body center.
    void body_center_line (double);

    /// Set latitude and longitude at center of image.
    int lat_lon_center (double lat, double lon);

    /// Set latitude at center of image.
    int lat_at_center (double lat);

    /// Set longitude at center of image.
    int lon_at_center (double lon);

    /// Set the optical axis.
    /**
     * @param sample Optical axis sample.
     * @param line   Optical axis line.
     */
    void optical_axis (double sample, double line);

    /// Set the optical axis sample.
    /**
     * @param sample Optical axis sample.
     */
    void optical_axis_sample (double sample);

    /// Set the optical axis line.
    /**
     * @param line Optical axis line.
     */
    void optical_axis_line (double line);

    /// Set flag that determines whether or not terminator is taken
    /// into account when determining if data point on body is
    /// visible.
    void use_terminator (bool u);

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
    virtual bool read_data (double lat,
                            double lon,
                            double & data) const;

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
    virtual bool read_data (double lat,
                            double lon,
                            double & data,
                            unsigned int & weight,
                            bool scan = true) const;

    /// Returns "true" if latitude and longitude are visible
    bool is_visible (double lat, double lon) const;


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
    bool latlon2pix (double lat,
                     double lon,
                     double & x,
                     double & z) const;

    /// Make sure all pre-processing is done.
    /**
     * @todo Automate finalization of PhotoImage setup.
     */
    void finalize_setup (void);

  private:

    /// Use range, focal length and scale to compute
    int set_km_per_pixel (void);

    /// Get rotation matrices for case when body centers are given.
    /**
     * @param range_o     Range vector in observer coordinates.
     * @param observ2body Observer to body coordinates transformation
     *                    matrix.
     * @param body2observ Body to observer coordinates transformation
     *                    matrix.
     */
    int rot_matrices (const DVector & range_o,
                      DMatrix & observ2body,
                      DMatrix & body2observ);

    /// Get rotation matrices for case when lat/lon at optical axis
    /// were given.
    /**
     * @param range_b     The range vector in body coordinates.
     * @param OA          The optical axis vector in body coordinates.
     * @param observ2body Observer to body coordinates transformation
     *                    matrix.
     * @param body2observ Body to observer coordinates transformation
     *                    matrix.
     */
    void rot_matrices (const DVector & range_b,
                       const DVector & OA,
                       DMatrix & observ2body,
                       DMatrix & body2observ);

  private:

    /// Object representing the body being mapped.
    /**
     * @note OblateSpheroid is used instead of BodyData since some
     *       code in this implementation assumes that the body is
     *       modeled as an oblate spheroid.
     */
    /* const */ OblateSpheroid body_;

    /// Pointer to the image array.
    Image<double> image_;

    /// Number of samples in the image.
    /* const */ unsigned int samples_;

    /// Number of lines in the image.
    /* const */ unsigned int lines_;

    /// Geometric/optical correction strategy used during
    /// latitude/longitude to pixel conversion, and vice versa.
    ValuePtr<GeometricCorrection> geometric_correction_;

    /// Pointer to the photometric correction strategy.
    ValuePtr<PhotometricCorrection> photometric_correction_;

    /// Pointer to the photometric correction strategy.
    ValuePtr<InterpolationStrategy> interpolation_strategy_;

    /// Sub-Observer Latitude -- BodyCENTRIC (radians).
    double sub_observ_lat_;

    /// Sub-Observer Longitude -- Central Meridian (radians).
    double sub_observ_lon_;

    /// Sub-Solar Latitude -- BodyCENTRIC (radians)
    double sub_solar_lat_;

    /// Sub-Solar Longitude (radians)
    double sub_solar_lon_;

    /// Center of body distance to observer (Kilometers)
    double range_;

    /// Position angle in the image NOT in the sky.
    double position_angle_;

    /// Arcseconds per pixel.
    double arcsec_per_pixel_;

    /// Kilometers per pixel at plane that passes through body
    /// center.
    double km_per_pixel_;

    /// Focal length in millimeters.
    double focal_length_;

    /// Focal length in pixels
    /**
     * @note Any distance unit may be used for the focal length and
     *       the scale as long as BOTH use the SAME distance unit.
     *       (e.g. cm and pixels/cm).
     */
    double focal_length_pixels_;

    /// pixels/mm at focal plane.
    /**
     * @see @c focal_length_pixels_
     */
    double scale_;

    /// Perpendicular distance from observer to image plane.
    double normal_range_;

    /**
     * @name Optical Axis
     *
     * The location of the optical axis (boresight) on the photo.
     */
    //@{
    /// Sample component of optical axis.
    double OA_s_;

    /// Line component of optical axis.
    double OA_l_;
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
    Image<bool> sky_mask_;

    /// Range vector in body coordinates, measured from the center of
    /// the body to the observer.
    DVector range_b_;

    /// Transformation matrix to go from observer to body
    /// coordinates.
    DMatrix observ2body_;

    /// Transformation matrix to go from body to observer
    /// coordinates.
    DMatrix body2observ_;

    /// Amount of pixels to ignore from left side of input image
    /// (photo).
    unsigned int nibble_left_;

    /// Amount of pixels to ignore from right side of input image
    /// (photo).
    unsigned int nibble_right_;

    /// Amount of pixels to ignore from top side of input image
    /// (photo).
    unsigned int nibble_top_;

    /// Amount of pixels to ignore from bottom side of input image
    /// (photo).
    unsigned int nibble_bottom_;

    /**
     * @name Object-Space Body Center
     *
     * Object space (e.g. corrected for lens aberration) center of
     * body in the input image/photo.
     */
    //@{
    /// Horizontal center of body in photo.
    double sample_center_;

    /// Vertical center of body in photo.
    double line_center_;
    //@}

    /// BODYcentric latitude at picture center.
    double lat_at_center_;

    /// Longitude at picture center
    double lon_at_center_;

    /// Cosine of emission angle outside of which no data will be
    /// plotted/retrieved.
    double mu_limit_;

//     /// Minimum latitude visible on body.
//     double min_lat_;

//     /// Maximum latitude visible on body.
//     double max_lat_;

//     /// Minimum longitude visible on body
//     double min_lon_;

//     /// Maximum longitude visible on body
//     double max_lon_;

    /// bit set used to keep track of which internal flags are set.
    unsigned long flags_;

  };

}


#endif  /* MARC_PHOTO_IMAGE_H */
