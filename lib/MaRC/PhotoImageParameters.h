//   -*- C++ -*-

//==========================================================================
/**
 *  @file PhotoImageParameters.h
 *
 *  @author Nathanael Nerode <neroden@gcc.gnu.org>
 */
//==========================================================================


#ifndef MARC_PHOTO_IMAGE_PARAMETERS_H
#define MARC_PHOTO_IMAGE_PARAMETERS_H

#include <MaRC/SourceImage.h>
#include <MaRC/Geometry.h>
#include <MaRC/OblateSpheroid.h>
#include <MaRC/GeometricCorrection.h>
#include <MaRC/PhotometricCorrection.h>
#include <MaRC/InterpolationStrategy.h>
#include <MaRC/PhotoInterpolationStrategy.h>
#include <MaRC/NullInterpolationStrategy.h>
#include <MaRC/Matrix.h>
#include <MaRC/misc.h>
#include <MaRC/Constants.h>

#include <cmath>
#include <limits>
#include <stdexcept>
#include <numeric>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>


namespace MaRC {

  class PhotoImage; // forward declare, avoid circular inclusion

  class PhotoImageFlagsWrapper {
  public:
    /**
     * @enum sflags
     *
     * Flags that state which PhotoImage attributes have been set.
     */
    enum sflags
      {
        LATLON_AT_CENTER = 1 << 0,
        SS_VERIFY        = 1 << 1,
        EXTREMA_SET      = 1 << 2,
        USE_TERMINATOR   = 1 << 3,
        REMOVE_SKY       = 1 << 4
      };
  };

  /**
   * @class PhotoImageParameters
   *
   * @brief Auxilliary class using the Named Parameter Idiom to
   *        set the options for PhotoImage.
   */
  class PhotoImageParameters: PhotoImageFlagsWrapper
  {
    friend class PhotoImage;
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
    PhotoImageParameters (OblateSpheroid const & body,
                double * image,
                unsigned int samples,
                unsigned int lines,
                GeometricCorrection * gc);

    /// Destructor.
    virtual ~PhotoImageParameters (void);

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

    /// Set the geometric correction strategy used during lat/lon to
    /// pixel conversion, and vice-versa.
    void geometric_correction (GeometricCorrection * g);

    /// Set the photometric correction strategy.
    void photometric_correction (PhotometricCorrection * p);

    /// Set sub-observer latitude and longitude.
    /**
     * @param lat Sub-observer latitude  (degrees)
     * @param lon Sub-observer longitude (degrees)
     */
    void sub_observ (double lat, double lon);

    /// Set Sub-Observer latitude.
    /**
     * @param lat Sub-observer latitude  (degrees)
     */
    void sub_observ_lat (double lat);

    /// Set Sub-Observer longitude.
    /**
     * @param lon Sub-observer longitude (degrees)
     */
    void sub_observ_lon (double lon);

    /// Set Sub-Solar latitude and longitude.
    /**
     * @param lat Sub-solar latitude  (degrees)
     * @param lon Sub-solar longitude (degrees)
     */
    void sub_solar (double lat, double lon);

    /// Set Sub-Solar latitude.
    /**
     * @param lat Sub-solar latitude  (degrees)
     */
    void sub_solar_lat (double lat);

    /// Set Sub-Solar longitude.
    /**
     * @param lon Sub-solar longitude (degrees)
     */
    void sub_solar_lon (double lon);

    /// Set observer to body distance (KM).
    /**
     * @param r Distance from observer to body in kilometers.
     */
    void range (double r);

    /// Set camera focal length (millimeters).
    /**
     * @param len Focal length in millimeters.
     */
    void focal_length (double len);

    /// Set input image scale (pixels / mm).
    /**
     * @param s Image scale in pixels per millimeter.
     */
    void scale (double s);

    /// Set position angle (a.k.a. North Angle in degrees) found in
    /// image.
    void position_angle (double north);

    /// Arcseconds per pixel in image.
    void arcsec_per_pixel (double a);

    /// Kilometers per pixel in image.
    void km_per_pixel (double k);

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

    /// Enable/disable pixel interpolation when reading data.
    void interpolate (bool enable);

    /// Set emission angle beyond which no data will be read.
    void emi_ang_limit (double angle);

    /// Set sample and line of body center.
    void body_center (double sample, double line);

    /// Set sample of body center.
    void body_center_sample (double);

    /// Set line of body center.
    void body_center_line (double);

    /// Set latitude and longitude at center of image.
    void lat_lon_center (double lat, double lon);

    /// Set latitude at center of image.
    void lat_at_center (double lat);

    /// Set longitude at center of image.
    void lon_at_center (double lon);

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

    // Turn supersampling verification on (or off).
    void supersample_verify (bool verify);

    /**
     * Name of the file this came from.
     */
    void set_filename (const std::string &);

  private:

    /// Make sure all pre-processing is done.
    void finalize_setup (void);

    /*
     * These are used to automagically invoke finalize_setup once
     * when an unready element is needed by PhotoImage's
     * constructor.
     * They're trivial and should remain inlined.
     */
    double get_km_per_pixel() {
      if (!this->finalized) {
        this->finalize_setup();
      }
      return this->km_per_pixel_;
    }
    double get_focal_length_pixels() {
      if (!this->finalized) {
        this->finalize_setup();
      }
      return this->focal_length_pixels_;
    }
    DVector get_range_b() {
      if (!this->finalized) {
        this->finalize_setup();
      }
      return this->range_b_;
    }
    DMatrix get_observ2body() {
      if (!this->finalized) {
        this->finalize_setup();
      }
      return this->observ2body_;
    }
    DMatrix get_body2observ() {
      if (!this->finalized) {
        this->finalize_setup();
      }
      return this->body2observ_;
    }
    Image<bool> get_sky_mask() {
      if (!this->finalized) {
        this->finalize_setup();
      }
      return this->sky_mask_;
    }
    Image<bool> get_image_unread_mask() {
      if (!this->finalized) {
        this->finalize_setup();
      }
      // Initialize image_unread_mask to sky_mask, because
      // we want to treat all sky pixels as "already read"
      // for verification purposes.
      return this->sky_mask_;
    }

    /**
     * Run sanity checks on the parameters; throw if this is an 
     * invalid combination of parameters.
     * Use only after the parameters are all set.
     */
    void verify_values (void) const;

    /// Use range, focal length and scale to compute
    void set_km_per_pixel (void);

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

    /// Create sky removal mask.
    void remove_sky (void);

  private:

    /**
     * This is set to true when the final parameters have been
     * computed (possibly a rather complicated process).
     */
    bool finalized;

    /// filename
    std::string filename_;

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
    std::unique_ptr<GeometricCorrection> geometric_correction_;

    /// Pointer to the photometric correction strategy.
    std::unique_ptr<PhotometricCorrection> photometric_correction_;

    /// Pointer to the photometric correction strategy.
    std::unique_ptr<InterpolationStrategy> interpolation_strategy_;

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

    /// bit set used to keep track of which internal flags are set.
    unsigned long flags_;

  };

/******************
  Inline functions
 ******************/

  inline void
  PhotoImageParameters::geometric_correction (GeometricCorrection * strategy)
  {
    if (strategy != 0)
      {
        this->geometric_correction_.reset(strategy);
      }
    else
      {
        throw std::invalid_argument(
          "Null geometric correction strategy pointer.");
      }
  }
  
  inline void
  PhotoImageParameters::photometric_correction (PhotometricCorrection * strategy)
  {
    if (strategy != 0)
      {
        this->photometric_correction_.reset(strategy);
      }
    else
      {
        throw std::invalid_argument(
          "Null photometric correction strategy pointer.");
      }
  }
  
  inline void
  PhotoImageParameters::sub_observ_lat (double lat)
  {
    if (::fabs (lat) <= 90)
      this->sub_observ_lat_ = lat * C::degree;
    else
      {
        std::ostringstream s;
        s << "Incorrect value for Sub-Observer Latitude: " << lat;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::sub_observ_lon (double lon)
  {
    if (::fabs (lon) <= 360)
      {
        if (lon < 0)
          lon += 360;
        this->sub_observ_lon_ = lon * C::degree;
      }
    else
      {
        std::ostringstream s;
        s << "Incorrect value for Sub-Observer Longitude: " << lon;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::sub_observ (double lat, double lon)
  {
    this->sub_observ_lat(lat);
    this->sub_observ_lon(lon);
  }
  
  inline void
  PhotoImageParameters::sub_solar_lat (double lat)
  {
    if (::fabs (lat) <= 90)
      this->sub_solar_lat_ = lat * C::degree;
    else
      {
        std::ostringstream s;
        s << "Incorrect value for Sub-Solar Latitude: " << lat;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::sub_solar_lon (double lon)
  {
    if (::fabs (lon) <= 360)
      {
        if (lon < 0)
          lon += 360;
        this->sub_solar_lon_ = lon * C::degree;
      }
    else
      {
        std::ostringstream s;
        s << "Incorrect value for Sub-Solar Longitude: " << lon;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::sub_solar (double lat, double lon)
  {
    this->sub_solar_lat(lat);
    this->sub_solar_lon(lon);
  }
  
  inline void
  PhotoImageParameters::position_angle (double north)
  {
    if (::fabs (north) <= 360)
      this->position_angle_ = north * C::degree;
    else
      {
        std::ostringstream s;
        s << "Incorrect value for position angle: " << north;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::remove_sky (bool r)
  {
    if ( r ) {
      flags::set (this->flags_, REMOVE_SKY);
    } else {
      flags::unset (this->flags_, REMOVE_SKY);
    }
  }
  
  inline void
  PhotoImageParameters::arcsec_per_pixel (double a)
  {
    // FIXME: Error out if range is not set!
    // NOTE: range_ should be in units of kilometers at this point.
    if (a > 0)
      {
        this->arcsec_per_pixel_ = a;
        this->km_per_pixel_ =
          C::pi / 648e3 * this->range_ * this->arcsec_per_pixel_;
      }
    else
      {
        std::ostringstream s;
        s << "Incorrect number of arcseconds per pixel entered: " << a;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::km_per_pixel (double value)
  {
    if (value > 0)
      this->km_per_pixel_ =  value;
    else
      {
        std::ostringstream s;
        s << "Incorrect number of kilometers per pixel entered: " << value;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::focal_length (double len)
  {
    if (len > 0)
      this->focal_length_ = len;
    else
      {
        std::ostringstream s;
        s << "Incorrect focal length entered: " << len;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::scale (double sc)
  {
    if (sc > 0)
      this->scale_ = sc;
    else
      {
        std::ostringstream s;
        s << "Incorrect scale entered: " << sc;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::body_center_sample (double sample)
  {
    this->sample_center_ = sample;
  }
  
  inline void
  PhotoImageParameters::body_center_line (double line)
  {
    this->line_center_ = line;
  }
  
  inline void
  PhotoImageParameters::body_center (double sample, double line)
  {
    this->body_center_sample(sample);
    this->body_center_line(line);
  }
  
  inline void
  PhotoImageParameters::lat_at_center (double lat)
  {
    if (::fabs (lat) <= 90)
      {
        this->lat_at_center_ = lat * C::degree;
        flags::set (this->flags_, LATLON_AT_CENTER);
      }
    else
      {
        std::ostringstream s;
        s << "Incorrect latitude at picture center: " << lat;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::lon_at_center (double lon)
  {
    if (::fabs (lon) <= 360)
      {
        this->lon_at_center_ = lon * C::degree;
        flags::set (this->flags_, LATLON_AT_CENTER);
      }
    else
      {
        std::ostringstream s;
        s << "Incorrect longitude at picture center: " << lon;
        throw std::invalid_argument(s.str());
      }
  }
  
  inline void
  PhotoImageParameters::lat_lon_center (double lat, double lon)
  {
    this->lat_at_center(lat);
    this->lon_at_center(lon);
  }
  
  inline void
  PhotoImageParameters::optical_axis_sample (double sample)
  {
    this->OA_s_ = sample;
  }
  
  inline void
  PhotoImageParameters::optical_axis_line (double line)
  {
    this->OA_l_ = line;
  }
  
  inline void
  PhotoImageParameters::optical_axis (double sample, double line)
  {
    this->optical_axis_sample(sample);
    this->optical_axis_line(line);
  }
  
  inline void
  PhotoImageParameters::range (double r)
  {
    // value should be in kilometers!
    const double mr =
      std::min (this->body_.eq_rad (), this->body_.pol_rad ());
  
    if (r > mr && r < ::sqrt (std::numeric_limits<double>::max ()) - 1)
      this->range_ =  r;
    else
      {
        std::ostringstream s;
        s << "Invalid range entered: range must be greater than "
          << mr << " and less than "
          << ::sqrt (std::numeric_limits<double>::max ()) - 1;
        throw std::invalid_argument (s.str ());
      }
  }
  
  inline void
  PhotoImageParameters::nibble (unsigned int n)
  {
    const unsigned int minimum_dimension =
      (this->samples_ < this->lines_ ? this->samples_ : this->lines_);
  
    if (n < (minimum_dimension / 2))
      {
        this->nibble_left_   = n;
        this->nibble_right_  = n;
        this->nibble_top_    = n;
        this->nibble_bottom_ = n;
      }
    else
      {
        std::ostringstream s;
        s << "Invalid overall nibble value (" << n << ")";
        throw std::invalid_argument (s.str ());
      }
  }
  
  inline void
  PhotoImageParameters::nibble_left (unsigned int n)
  {
    if (n < (this->samples_ - this->nibble_right_))
      this->nibble_left_ = n;
    else
      {
        std::ostringstream s;
        s << "Invalid nibble left value (" << n << ")";
        throw std::invalid_argument (s.str ());
      }
  }
  
  inline void
  PhotoImageParameters::nibble_right (unsigned int n)
  {
    if (n < (this->samples_ - this->nibble_left_))
      this->nibble_right_ = n;
    else
      {
        std::ostringstream s;
        s << "Invalid nibble right value (" << n << ")";
        throw std::invalid_argument (s.str ());
      }
  }
  
  inline void
  PhotoImageParameters::nibble_top (unsigned int n)
  {
    if (n < (this->lines_ - this->nibble_bottom_))
      this->nibble_top_ = n;
    else
      {
        std::ostringstream s;
        s << "Invalid nibble top value (" << n << ")";
        throw std::invalid_argument (s.str ());
      }
  }
  
  inline void
  PhotoImageParameters::nibble_bottom (unsigned int n)
  {
    if (n < (this->lines_ - this->nibble_top_))
      this->nibble_bottom_ = n;
    else
      {
        std::ostringstream s;
        s << "Invalid nibble bottom value (" << n << ")";
        throw std::invalid_argument (s.str ());
      }
  }
  
  inline void
  PhotoImageParameters::interpolate (bool i)
  {
    // FIXME: Make sure this runs after nibble values are set
    if (i)
      {
        this->interpolation_strategy_ =
            std::make_unique<PhotoInterpolationStrategy>(
                this->samples_,
                this->lines_,
                this->nibble_left_,
                this->nibble_right_,
                this->nibble_top_,
                this->nibble_bottom_));
      }
    else
      this->interpolation_strategy_ =
          std::make_unique<NullInterpolationStrategy>();
  }
  
  inline void
  PhotoImageParameters::emi_ang_limit (double angle)
  {
    if (angle > 0 && angle <= 90)
      {
        this->mu_limit_ = ::cos (angle * C::degree);
      }
    else
      {
        std::ostringstream s;
        s << "Invalid value for emission angle limit: " << angle;
        throw std::invalid_argument (s.str ());
      }
  }
  
  inline void
  PhotoImageParameters::use_terminator (bool u)
  {
    if (u)
      flags::set (this->flags_, USE_TERMINATOR);
    else
      flags::unset (this->flags_, USE_TERMINATOR);
  }
  
  inline void
  PhotoImageParameters::supersample_verify (bool verify)
  {
    if (verify) {
      flags::set (this->flags_, SS_VERIFY);
    } else {
      flags::unset (this->flags_, SS_VERIFY);
    }
  }

  inline void
  PhotoImageParameters::set_filename (const std::string & filename)
  {
    this->filename_ = filename;
  }

} /* namespace MaRC */

#endif  /* MARC_PHOTO_IMAGE_PARAMETERS_H */
  
