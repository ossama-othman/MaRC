//   -*- C++ -*-
/**
 * @file PhotoImage.h
 *
 * Copyright (C) 1999, 2003-2005, 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#ifndef MARC_PHOTO_IMAGE_H
#define MARC_PHOTO_IMAGE_H

#include <MaRC/SourceImage.h>
#include <MaRC/Geometry.h>

#include <memory>
#include <vector>


namespace MaRC
{
    class OblateSpheroid;
    class GeometricCorrection;
    class PhotometricCorrection;
    class InterpolationStrategy;

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
         * @param[in]     body    Pointer to OblateSpheroid object
         *                        representing body being mapped.
         * @param[in,out] image   Array containing the image data.
         *                        Ownership is transferred through a
         *                        move operation.
         * @param[in]     samples Number of samples in the image.
         * @param[in]     lines   Number of lines   in the image.
         * @param[in,out] gc      Geometric correction strategy.
         *                        (@c PhotoImage assumes ownership).
         */
        PhotoImage(OblateSpheroid const & body,
                   std::vector<double> image, // moved, not copied!
                   std::size_t samples,
                   std::size_t lines,
                   std::unique_ptr<GeometricCorrection> gc);

        /// Destructor.
        virtual ~PhotoImage();

        /// Less than operator.
        bool operator<(PhotoImage const & /* img */) { return false; }

        /// Equality operator.
        bool operator==(PhotoImage const & img);

        /// Set sky removal variable
        /**
         * Enabling sky removal prevents data believed (i.e. computed)
         * to be in the sky rather than on the body from being
         * mapped.
         *
         * @param[in] remove @c true  == create sky removal mask,
         *                   @c false == do not create sky removal
         *                               mask.
         *
         * @note The source image array will not be modified.
         */
        void remove_sky(bool remove);

        /// Create sky removal mask.
        void remove_sky();

        /// Set the geometric correction strategy used during lat/lon
        /// to pixel conversion, and vice-versa.
        int geometric_correction(
            std::unique_ptr<GeometricCorrection> strategy);
        

        /// Set the photometric correction strategy.
        int photometric_correction(
            std::unique_ptr<PhotometricCorrection> strategy);

        /// Set sub-observation latitude and longitude.
        /**
         * @param[in] lat Sub-observation latitude  (degrees)
         * @param[in] lon Sub-observation longitude (degrees)
         */
        int sub_observ(double lat, double lon);

        /// Set Sub-Observation latitude.
        /**
         * @param[in] lat Sub-observation latitude  (degrees)
         */
        int sub_observ_lat (double lat);

        /// Set Sub-Observation longitude.
        /**
         * @param[in] lon Sub-observation longitude (degrees)
         */
        int sub_observ_lon (double lon);

        /// Set Sub-Solar latitude and longitude.
        /**
         * @param[in] lat Sub-solar latitude  (degrees)
         * @param[in] lon Sub-solar longitude (degrees)
         */
        int sub_solar(double lat, double lon);

        /// Set Sub-Solar latitude.
        /**
         * @param[in] lat Sub-solar latitude  (degrees)
         */
        int sub_solar_lat(double lat);

        /// Set Sub-Solar longitude.
        /**
         * @param[in] lon Sub-solar longitude (degrees)
         */
        int sub_solar_lon(double lon);

        /// Set observer to body distance (KM).
        /**
         * @param[in] r Distance from observer to body in kilometers.
         */
        int range(double r);

        /// Set camera focal length (millimeters).
        /**
         * @param len Focal length in millimeters.
         */
        int focal_length(double len);

        /// Set input image scale (pixels / mm).
        /**
         * @param[in] s Image scale in pixels per millimeter.
         */
        int scale(double s);

        /// Set position angle (a.k.a. North Angle in degrees) found
        /// in image.
        int position_angle(double north);

        /// Arcseconds per pixel in image.
        int arcsec_per_pixel(double a);

        /// Kilometers per pixel in image.
        int km_per_pixel(double k);

        /// Set all nibble values to @a n.
        /**
         * @param[in] n Nibble value for all image sides.
         */
        void nibble(std::size_t n);

        /// Set left nibble value to @a n.
        /**
         * @param[in] n Nibble value for left image side.
         */
        void nibble_left(std::size_t n);

        /// Set right nibble value to @a n.
        /**
         * @param[in] n Nibble value for right image side.
         */
        void nibble_right(std::size_t n);

        /// Set top nibble value to @a n.
        /**
         * @param[in] n Nibble value for top image side.
         */
        void nibble_top(std::size_t n);

        /// Set bottom nibble value to @a n.
        /**
         * @param[in] n Nibble value for bottom image side.
         */
        void nibble_bottom(std::size_t n);

        /// Return left nibble value.
        std::size_t nibble_left() const   { return this->nibble_left_;   }

        /// Return right nibble value.
        std::size_t nibble_right() const  { return this->nibble_right_;  }

        /// Return top nibble value.
        std::size_t nibble_top() const    { return this->nibble_top_;    }

        /// Return bottom nibble value.
        std::size_t nibble_bottom() const { return this->nibble_bottom_; }

        /// Enable/disable pixel interpolation when reading data.
        void interpolate(bool enable);

        /// Set emission angle beyond which no data will be read.
        int emi_ang_limit(double angle);

        /// Set sample and line of body center.
        void body_center(double sample, double line);

        /// Set sample of body center.
        void body_center_sample(double);

        /// Set line of body center.
        void body_center_line(double);

        /// Set latitude and longitude at center of image.
        int lat_lon_center(double lat, double lon);

        /// Set latitude at center of image.
        int lat_at_center(double lat);

        /// Set longitude at center of image.
        int lon_at_center(double lon);

        /// Set the optical axis.
        /**
         * @param[in] sample Optical axis sample.
         * @param[in] line   Optical axis line.
         */
        void optical_axis(double sample, double line);

        /// Set the optical axis sample.
        /**
         * @param[in] sample Optical axis sample.
         */
        void optical_axis_sample(double sample);

        /// Set the optical axis line.
        /**
         * @param[in] line Optical axis line.
         */
        void optical_axis_line(double line);

        /**
         * Set flag that determines whether or not terminator is taken
         * into account when determining if data point on body is
         * visible.
         */
        void use_terminator(bool u);

        /// Retrieve data from source image.
        /**
         * Retrieve data from source image. The configured data
         * interpolation strategy will be applied.
         *
         * @see MaRC::SourceImage::read_data().
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const;

        /// Retrieve data and weight from source image.
        /**
         * Retrieve data and weight from source image.  The configured
         * data interpolation strategy will be applied.
         *
         * @param[in]     lat    Bodycentric (e.g. planetocentric)
         *                       latitude in radians.
         * @param[in]     lon    Longitude in radians.
         * @param[out]    data   Data retrieved from image.
         * @param[in,out] weight Distance from pixel to closest edge
         *                       or blank pixel. 
         * @param[in]     scan   Flag that determines if a data weight
         *                       scan is performed.  It is generally
         *                       only used by the version of
         *                       @c read_data() that does not return a
         *                       weight.
         *
         * @retval @c true  Data retrieved,
         * @retval @c false No data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data,
                               std::size_t & weight,
                               bool scan = true) const;

        /// Returns "true" if latitude and longitude are visible.
        bool is_visible(double lat, double lon) const;

        /// Convert (latitude, longitude) to (sample, sine)
        /**
         * @param[in] lat  Bodycentric (e.g. planetocentric) latitude
         *                 in radians.
         * @param[in] lon  Longitude in radians.
         * @param[out] x   Floating point value corresponding to @c i.
         * @param[out] z   Floating point value corresponding to @c k.
         *
         * @retval @c true  Conversion succeeded.
         * @retval @c false Conversion failed.
         * 
         * @note Since @a x and @a z potentially include fractional
         *       pixel components, they are more accurate than their
         *       integer counterparts.
         */
        bool latlon2pix(double lat,
                        double lon,
                        double & x,
                        double & z) const;

        /// Make sure all pre-processing is done.
        /**
         * @todo Automate finalization of PhotoImage setup.
         */
        void finalize_setup();

    private:

        /// Use range, focal length and scale to compute
        int set_km_per_pixel();

        /// Get rotation matrices for case when body centers are given.
        /**
         * @param[in]  range_o     Range vector in observer coordinates.
         * @param[out] observ2body Observer to body coordinates
         *                         transformation matrix.
         * @param[out] body2observ Body to observer coordinates
         *                         transformation matrix.
         */
        int rot_matrices(DVector const & range_o,
                         DMatrix & observ2body,
                         DMatrix & body2observ);

        /// Get rotation matrices for case when lat/lon at optical axis
        /// were given.
        /**
         * @param[in]  range_b     The range vector in body
         *                         coordinates.
         * @param[in]  OA          The optical axis vector in body
         *                         coordinates.
         * @param[out] observ2body Observer to body coordinates
         *                         transformation matrix.
         * @param[out] body2observ Body to observer coordinates
         *                         transformation matrix.
         */
        void rot_matrices(DVector const & range_b,
                          DVector const & OA,
                          DMatrix & observ2body,
                          DMatrix & body2observ);

    private:

        /// Object representing the body being mapped.
        /**
         * @note OblateSpheroid is used instead of BodyData since some
         *       code in this implementation assumes that the body is
         *       modeled as an oblate spheroid.
         */
        OblateSpheroid const & body_;

        /// Pointer to the image array.
        std::vector<double> const image_;

        /// Number of samples in the image.
        std::size_t const samples_;

        /// Number of lines in the image.
        std::size_t const lines_;

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
         * @note Any distance unit may be used for the focal length
         *       and the scale as long as BOTH use the SAME distance
         *       unit (e.g. cm and pixels/cm).
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
         * body and which are in the sky without actually modifying
         * the original source photo.
         *
         * @note The sky mask is generally only useful when performing
         *       weighted averaging in a mosaiced image.
         *
         * @see MosaicImage
         */
        std::vector<bool> sky_mask_;

        /// Range vector in body coordinates, measured from the center
        /// of the body to the observer.
        DVector range_b_;

        /// Transformation matrix to go from observer to body
        /// coordinates.
        DMatrix observ2body_;

        /// Transformation matrix to go from body to observer
        /// coordinates.
        DMatrix body2observ_;

        /// Amount of pixels to ignore from left side of input image
        /// (photo).
        std::size_t nibble_left_;

        /// Amount of pixels to ignore from right side of input image
        /// (photo).
        std::size_t nibble_right_;

        /// Amount of pixels to ignore from top side of input image
        /// (photo).
        std::size_t nibble_top_;

        /// Amount of pixels to ignore from bottom side of input image
        /// (photo).
        std::size_t nibble_bottom_;

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
        /// plotted / retrieved.
        double mu_limit_;

        /// Minimum latitude visible on body.
        // double min_lat_;

        /// Maximum latitude visible on body.
        // double max_lat_;

        /// Minimum longitude visible on body
        // double min_lon_;

        /// Maximum longitude visible on body
        // double max_lon_;

        /// bit set used to keep track of which internal flags are
        /// set.
        /**
         * @todo Should we replace the "flags" implementation with
         *       @c std::bitset<> instead?
         */
        unsigned long flags_;

  };

}


#endif  /* MARC_PHOTO_IMAGE_H */
