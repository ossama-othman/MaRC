// -*- C++ -*-
/**
 * @file PhotoImageFactory.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#ifndef MARC_PHOTO_IMAGE_FACTORY_H
#define MARC_PHOTO_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include "MaRC/PhotoImage.h"  /* Need complete type for covariant
                                 return type in make() method. */

#include <vector>
#include <string>
#include <cstddef>


namespace MaRC
{
    class GeometricCorrection;
    class PhotometricCorrection;

    /**
     * @class PhotoImageFactory
     *
     * @brief Factory class that create PhotoImage objects.
     *
     * This class creates PhotoImage objects.  It is designed to
     * decouple FITS (for example) file and image operations from the
     * PhotoImage class.  It also exists to decouple the MaRC parser
     * grammar from the PhotoImage class.  This allows PhotoImage
     * object creation to be delayed until it is time for the data in
     * the PhotoImage to be mapped, which reduces run-time memory
     * requirements.
     */
    class PhotoImageFactory : public ImageFactory
    {
    public:

        /// Constructor.
        /**
         * @param[in] filename Name of file containing image.
         * @param[in] body     Body being mapped (currently must be
         *                     represented as an oblate spheroid).
         */
        PhotoImageFactory(char const * filename,
                          std::shared_ptr<OblateSpheroid> body);

        /// Create a @c PhotoImage.
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so);

        /// Set the flat field image filename.
        void flat_field(char const * name);

        /// Set the nibbling values.
        void nibbling(std::size_t left,
                      std::size_t right,
                      std::size_t top,
                      std::size_t bottom);

        /// Set the image inversion flags.
        void invert(bool vertical, bool horizontal);

        /// Set image interpolation flag.
        void interpolate(bool enable);

        /// Set image sky removal flag.
        void remove_sky(bool enable);

        /// Set emission angle beyond which no data will be read.
        void emi_ang_limit(double angle);

        /// Set sample and line of body center.
        void body_center(double sample, double line);

        /// Set latitude and longitude at center of image.
        void lat_lon_center(double lat, double lon);

        /// Set the optical axis.
        /**
         * @param[in] sample Optical axis sample.
         * @param[in] line   Optical axis line.
         */
        void optical_axis(double sample, double line);

        /// Enable the geometric correction strategy during
        /// lat/lon to pixel conversion, and vice-versa.
        void geometric_correction(bool enable);

        /// Enable the photometric correction strategy.
        void photometric_correction(bool enable);

        /// Set sub-observation latitude and longitude.
        /**
         * @param[in] lat Sub-observation latitude  (degrees)
         * @param[in] lon Sub-observation longitude (degrees)
         */
        void sub_observ(double lat, double lon);

        /// Set Sub-Solar latitude and longitude.
        /**
         * @param[in] lat Sub-solar latitude  (degrees)
         * @param[in] lon Sub-solar longitude (degrees)
         */
        void sub_solar(double lat, double lon);

        /// Set observer to body distance (KM).
        /**
         * @param[in] r Distance from observer to body in kilometers.
         */
        void range(double r);

        /// Set camera focal length (millimeters).
        /**
         * @param[in] len Focal length in millimeters.
         */
        void focal_length (double len);

        /// Set input image scale (pixels / mm).
        /**
         * @param[in] s Image scale in pixels per millimeter.
         */
        void scale(double s);

        /// Set position angle (a.k.a. North Angle in degrees) found
        /// in image.
        void position_angle(double north);

        /// Arcseconds per pixel in image.
        void arcsec_per_pixel(double a);

        /// Kilometers per pixel in image.
        void km_per_pixel(double k);

        /// Set flag that determines whether or not terminator is
        /// taken into account when determining if data point on body
        /// is visible.
        void use_terminator (bool u);

        /// Invert image from left to right.
        /**
         * @param[in,out] img     Image to be inverted.
         * @param[in]     samples Number of samples in image.
         * @param[in]     lines   Number of lines   in image.
         */
        static void invert_h(std::vector<double> & image,
                             std::size_t samples,
                             std::size_t lines);

        /// Invert image from top to bottom.
        /**
         * @param[in,out] img     Image to be inverted.
         * @param[in]     samples Number of samples in image.
         * @param[in]     lines   Number of lines   in image.
         */
        static void invert_v(std::vector<double> & image,
                             std::size_t samples,
                             std::size_t lines);

    private:

        /// Name of photo/image to be mapped.
        std::string const filename_;

        /// Name of flat field image to be substracted from the
        /// photo/image containing the actual data.
        std::string flat_field_;

        /// Object representing the body being mapped.
        /**
         * @note OblateSpheroid is used instead of BodyData since some
         *       code in this implementation assumes that the body is
         *       modeled as an oblate spheroid.
         */
        std::shared_ptr<OblateSpheroid> const body_;

        /// Enable/disable geometric correction.
        /**
         * @note Only GLL spacecraft geometric lens abberration
         *       correct is currently supported.
         */
        bool geometric_correction_;

        /// Enable/disable photometric correction.
        bool photometric_correction_;

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

        /// pixels/mm at focal plane.
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

        /// Emission angle beyond which no data will be mapped.
        double emi_ang_limit_;

        /// Perform sky removal.
        bool remove_sky_;

        /// Perform pixel interpolation.
        bool interpolate_;

        /// Enable/disable mapping beyond terminator.
        bool use_terminator_;

        /// Invert image top to bottom.
        bool invert_v_;

        /// Invert image left to right.
        bool invert_h_;

    };

}

#endif  /* MARC_PHOTO_IMAGE_FACTORY_H */
