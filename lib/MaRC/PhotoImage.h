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
#include <MaRC/PhotoImageParameters.h>
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
    class PhotoImage : public SourceImage, PhotoImageFlagsWrapper
    {
    public:

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
        PhotoImage(std::shared_ptr<OblateSpheroid> body,
                   std::vector<double> && image, // moved, not copied!
                   std::size_t samples,
                   std::size_t lines,
                   std::unique_ptr<GeometricCorrection> gc);

        /// Destructor.
        virtual ~PhotoImage();

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
         * @retval true  Data retrieved,
         * @retval false No data retrieved.
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
         * @param[in]  lat Bodycentric (e.g. planetocentric) latitude
         *                 in radians.
         * @param[in]  lon Longitude in radians.
         * @param[out] x   Sample at given latitude and longitude.
         * @param[out] z   Line at given latitude and longitude.
         *
         * @retval true  Conversion succeeded.
         * @retval false Conversion failed.
         *
         * @note Since @a x and @a z potentially include fractional
         *       pixel components, they are more accurate than their
         *       integer counterparts.
         */
        bool latlon2pix(double lat,
                        double lon,
                        double & x,
                        double & z) const;

    private:

        /// Object representing the body being mapped.
        /**
         * @note OblateSpheroid is used instead of BodyData since some
         *       code in this implementation assumes that the body is
         *       modeled as an oblate spheroid.
         */
        std::shared_ptr<OblateSpheroid> const body_;

        /// Pointer to the image array.
        std::vector<double> const image_;

        /// Number of samples in the image.
        std::size_t const samples_;

        /// Number of lines in the image.
        std::size_t const lines_;

        /// Geometric/optical correction strategy used during
        /// latitude/longitude to pixel conversion, and vice versa.
        std::unique_ptr<GeometricCorrection> const geometric_correction_;

        /// Pointer to the photometric correction strategy.
        std::unique_ptr<PhotometricCorrection> const photometric_correction_;

        /// Pointer to the photometric correction strategy.
        std::unique_ptr<InterpolationStrategy> const interpolation_strategy_;

        /// Sub-Observer Latitude -- BodyCENTRIC (radians).
        double const sub_observ_lat_;

        /// Sub-Observer Longitude -- Central Meridian (radians).
        double const sub_observ_lon_;

        /// Sub-Solar Latitude -- BodyCENTRIC (radians)
        double const sub_solar_lat_;

        /// Sub-Solar Longitude (radians)
        double const sub_solar_lon_;

        /// Center of body distance to observer (Kilometers)
        double const range_;

        /// Position angle in the image NOT in the sky.
        double const position_angle_;

        /// Kilometers per pixel at plane that passes through body
        /// center.
        double const km_per_pixel_;

        /// Focal length in millimeters.
        double const focal_length_;

        /// Focal length in pixels
        /**
         * @note Any distance unit may be used for the focal length
         *       and the scale as long as BOTH use the SAME distance
         *       unit (e.g. cm and pixels/cm).
         */
        double const focal_length_pixels_;

        /// pixels/mm at focal plane.
        /**
         * @see @c focal_length_pixels_
         */
        double const scale_;

        /**
         * @name Optical Axis
         *
         * The location of the optical axis (boresight) on the photo.
         */
        //@{
        /// Sample component of optical axis.
        double const OA_s_;

        /// Line component of optical axis.
        double const OA_l_;
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
        std::vector<bool> const sky_mask_;

        // Mask tracking which pixels in the image were read.
        std::vector<bool> image_unread_mask_;

        /// Range vector in body coordinates, measured from the center
        /// of the body to the observer.
        DVector const range_b_;

        /// Transformation matrix to go from observer to body
        /// coordinates.
        DMatrix const observ2body_;

        /// Transformation matrix to go from body to observer
        /// coordinates.
        DMatrix const body2observ_;

        /// Amount of pixels to ignore from left side of input image
        /// (photo).
        std::size_t const nibble_left_;

        /// Amount of pixels to ignore from right side of input image
        /// (photo).
        std::size_t const nibble_right_;

        /// Amount of pixels to ignore from top side of input image
        /// (photo).
        std::size_t const nibble_top_;

        /// Amount of pixels to ignore from bottom side of input image
        /// (photo).
        std::size_t const nibble_bottom_;

        /**
         * @name Object-Space Body Center
         *
         * Object space (e.g. corrected for lens aberration) center of
         * body in the input image/photo.
         */
        //@{
        /// Horizontal center of body in photo.
        double const sample_center_;

        /// Vertical center of body in photo.
        double const line_center_;
        //@}

        /// BODYcentric latitude at picture center.
        double const lat_at_center_;

        /// Longitude at picture center
        double const lon_at_center_;

        /**
         * The cosine, &mu; of the emission angle outside of which no
         * data will be plotted / retrieved.
         *
         * This is used to avoid mapping data close to the limb.
         */
        double const mu_limit_;

        /// bit set used to keep track of which internal flags are
        /// set.
        /**
         * @todo Should we replace the "flags" implementation with
         *       @c std::bitset<> instead?
         */
        unsigned long const flags_;
  };

}


#endif  /* MARC_PHOTO_IMAGE_H */
