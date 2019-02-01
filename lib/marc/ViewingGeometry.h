// -*- C++ -*-
/**
 * @file ViewingGeometry.h
 *
 * Copyright (C) 1999, 2003-2005, 2017-2018  Ossama Othman
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

#ifndef MARC_VIEWING_GEOMETRY_H
#define MARC_VIEWING_GEOMETRY_H

#include "marc/Geometry.h"
#include "marc/GeometricCorrection.h"
#include "marc/Export.h"

#include <vector>
#include <cstddef>
#include <memory>


namespace MaRC
{
    class OblateSpheroid;
    class GeometricCorrection;

    /**
     * @class ViewingGeometry ViewingGeometry.h <marc/ViewingGeometry.h>
     *
     * @brief Viewing geometry necessary to go from latitude/longitude
     *        to image pixel.
     *
     * This class encapsulates parameters and operations needed to
     * convert a point (latitude and longitude) on an observed body to
     * an image pixel.
     *
     * @note The observed body is currently required to be modeled as
     *       an oblate spheroid due to the way the rotation matrices
     *       are generated.
     */
    class MARC_API ViewingGeometry
    {
    public:

        /// Constructor.
        ViewingGeometry(std::shared_ptr<OblateSpheroid> body);

        /// Destructor.
        ~ViewingGeometry() = default;

        // Disallow copying.
        ViewingGeometry(ViewingGeometry const &) = delete;
        ViewingGeometry & operator=(ViewingGeometry const &) = delete;

        /// Make sure all pre-processing is done.
        /**
         * @param[in] samples  Number of samples in the image.
         * @param[in] lines    Number of lines   in the image.
         *
         * @todo Automate finalization of PhotoImage setup.
         */
        void finalize_setup(std::size_t samples,
                            std::size_t lines);

        /// Set the geometric correction strategy used during lat/lon
        /// to pixel conversion, and vice-versa.
        /**
         * @throw std::invalid_argument @a strategy is a nullptr.
         */
        void geometric_correction(
            std::unique_ptr<GeometricCorrection> strategy);

        /// Set sub-observation latitude and longitude.
        /**
         * @param[in] lat Sub-observation latitude  (degrees)
         * @param[in] lon Sub-observation longitude (degrees)
	 *
         * @throw std::invalid_argument Invalid @a lat or @a lon.
         */
        void sub_observ(double lat, double lon);

        /// Set Sub-Observation latitude.
        /**
         * @param[in] lat Sub-observation latitude  (degrees)
	 *
         * @throw std::invalid_argument Invalid @a lat.
         */
        void sub_observ_lat(double lat);

        /// Set Sub-Observation longitude.
        /**
         * @param[in] lon Sub-observation longitude (degrees)
	 *
         * @throw std::invalid_argument Invalid @a lon.
         */
        void sub_observ_lon(double lon);

        /// Set Sub-Solar latitude and longitude.
        /**
         * @param[in] lat Sub-solar latitude  (degrees)
         * @param[in] lon Sub-solar longitude (degrees)
	 *
         * @throw std::invalid_argument Invalid @a lat or @a lon.
         */
        void sub_solar(double lat, double lon);

        /// Set Sub-Solar latitude.
        /**
         * @param[in] lat Sub-solar latitude  (degrees)
	 *
         * @throw std::invalid_argument Invalid @a lat.
         */
        void sub_solar_lat(double lat);

        /// Set Sub-Solar longitude.
        /**
         * @param[in] lon Sub-solar longitude (degrees)
	 *
         * @throw std::invalid_argument Invalid @a lon.
         */
        void sub_solar_lon(double lon);

        /// Set observer to body distance (KM).
        /**
         * @param[in] r Distance from observer to body in kilometers.
	 *
         * @throw std::invalid_argument Invalid @a r.
         */
        void range(double r);

        /// Set camera focal length (millimeters).
        /**
         * @param len Focal length in millimeters.
	 *
         * @throw std::invalid_argument Invalid @a len.
         */
        void focal_length(double len);

        /// Set input image scale (pixels / mm).
        /**
         * @param[in] s Image scale in pixels per millimeter.
	 *
         * @throw std::invalid_argument Invalid @a s.
         */
        void scale(double s);

        /**
         * @brief Set position angle in degrees found in image.
         *
         * Set the position (i.e. North) angle in degrees found in the
         * image, measured counter-clockwise positive from the
         * vertical "up" direction of the image.
         *
         * @param[in] north Position angle (degrees).
         *
         * @throw std::invalid_argument Invalid @a north.
         */
        void position_angle(double north);

        /// Arcseconds per pixel in image.
        /**
         * @param[in] arcseconds Arcseconds per pixel in the image.
         *
         * @throw std::invalid_argument Invalid @a arcseconds.
         *
         * @attention Set the range before calling this method.
         *
         * @todo Forcing the caller to set the range beforehand is
         *       rather brittle.  Try to do away with that
         *       requirement.
         */
        void arcsec_per_pixel(double arcseconds);

        /// Kilometers per pixel in image.
        /**
         * @param[in] k Kilometers per pixel in the image.
         *
         * @throw std::invalid_argument Invalid @a k.
         */
        void km_per_pixel(double k);

        /// Set sample and line of body center.
        void body_center(double sample, double line);

        /// Set sample of body center.
        void body_center_sample(double);

        /// Set line of body center.
        void body_center_line(double);

        /// Set latitude and longitude at center of image.
        /**
         * @param[in] lat Latitude in degrees at center of image.
         * @param[in] lon Longitude in degrees at center of image.
         *
         * @throw std::invalid_argument Invalid @a lat or @a lon.
         */
        void lat_lon_center(double lat, double lon);

        /// Set latitude at center of image.
        /**
         * @param[in] lat Latitude in degrees at center of image.
         *
         * @throw std::invalid_argument Invalid @a lat.
         */
        void lat_at_center(double lat);

        /// Set longitude at center of image.
        /**
         * @param[in] lon Longitude in degrees at center of image.
         *
         * @throw std::invalid_argument Invalid @a lon.
         */
        void lon_at_center(double lon);

        /// Set the optical axis.
        /**
         * @param[in] sample Optical axis sample.
         * @param[in] line   Optical axis line.
         *
         * @throw std::invalid_argument Invalid @a sample or @a line.
         */
        void optical_axis(double sample, double line);

        /// Set the optical axis sample.
        /**
         * @param[in] sample Optical axis sample.
         *
         * @throw std::invalid_argument Invalid @a sample.
         */
        void optical_axis_sample(double sample);

        /// Set the optical axis line.
        /**
         * @param[in] line Optical axis line.
         *
         * @throw std::invalid_argument Invalid @a line.
         */
        void optical_axis_line(double line);


        /// Set emission angle beyond which no data will be read.
        /**
         * @param[in] angle Emission angle in degrees.
         *
         * @throw std::invalid_argument Invalid @a angle.
         */
        void emi_ang_limit(double angle);

        /**
         * Set flag that determines whether or not terminator is taken
         * into account when determining if data point on body is
         * visible.
         */
        void use_terminator(bool u);

        /// Is given latitude and longitude visible to the observer?
        /**
	 * @param[in] lat Latitude in radians
	 * @param[in] lon Longitude in radians
	 *
	 * @return @c true if latitude and longitude are visible.
         *
         * @note The point on the surface at the given @a lat and
         *       @a lon may still be considered "visible" even if it
         *       is off-image since it is on the same side of the body
         *       as the observer.  For example, the point on the
         *       surface may be on the side of the body facing a
         *       spacecraft, but the spacecraft camera may be directed
         *       far enough away that the point on the surface doesn't
         *       show up in the image.
	 */
        bool is_visible(double lat, double lon) const;

        /// Convert (latitude, longitude) to (sample, line).
        /**
         * @param[in]  lat Planetocentric latitude in radians.
         * @param[in]  lon Longitude in radians.
         * @param[out] x   Sample at given latitude and longitude.
         * @param[out] z   Line at given latitude and longitude.
         *
         * @retval true  Conversion succeeded.
         * @retval false Conversion failed.
         *
         * @bug @a x and @a z may be outside the bounds of the image
         *      even if they are visible, i.e. on the side of the body
         *      facing the observer.  Verify that they fall within
         *      image bounds.
         *
         * @note Since @a x and @a z potentially include fractional
         *       pixel components, they are more accurate than their
         *       integer counterparts.
         *
         */
        bool latlon2pix(double lat,
                        double lon,
                        double & x,
                        double & z) const;

        /// Convert (sample, line) to (latitude, longitude).
        /**
         * @param[in]  sample Sample at given latitude and longitude.
         * @param[in]  line   Line at given latitude and longitude.
         * @param[out] lat    Planetocentric latitude in radians.
         * @param[out] lon    Longitude in radians.
         *
         * @retval true  Conversion succeeded.
         * @retval false Conversion failed.
         */
        bool pix2latlon(double sample,
                        double line,
                        double & lat,
                        double & lon) const;

        /**
         * @brief Mask that marks where the observed body is in the
         *        image.
         *
         * Mark where the observed body with the encapsulated viewing
         * geometry lies in an image with @a samples and @a lines.
         *
         * @param[in] samples Samples in the image.
         * @param[in] lines   Lines   in the image.
         *
         * @return Vector of boolean values, where @c true refers to a
         *         point on the body.
         */
        std::vector<bool> body_mask(std::size_t samples,
                                    std::size_t lines) const;

    private:

        /// Finalize kilometers per pixel value.
        /**
         * Use range, focal length and scale to compute the kilometers
         * per pixel in the image.
         */
        void set_km_per_pixel();

        /// Get rotation matrices for case when body centers are given.
        /**
         * @param[in]  range_o     Range vector in observer coordinates.
         * @param[out] observ2body Observer to body coordinates
         *                         transformation matrix.
         * @param[out] body2observ Body to observer coordinates
         *                         transformation matrix.
         *
         * @return @c true on success.
         */
        bool rot_matrices(DVector const & range_o,
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
        std::shared_ptr<OblateSpheroid> const body_;

        /// Sub-Observer Latitude -- Planetocentric (radians).
        double sub_observ_lat_;

        /// Sub-Observer Longitude -- Central Meridian (radians).
        double sub_observ_lon_;

        /// Sub-Solar Latitude -- Planetocentric (radians)
        double sub_solar_lat_;

        /// Sub-Solar Longitude (radians)
        double sub_solar_lon_;

        /// Center of body distance to observer (Kilometers)
        double range_;

        /**
         * @brief Position angle in the image NOT in the sky.
         *
         * Position angle of the body in the image measured
         * counter-clockwise positive from the vertical "up" direction
         * of the image.
         */
        double position_angle_;

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

        /// Range vector in body coordinates, measured from the center
        /// of the body to the observer.
        DVector range_b_;

        /// Transformation matrix to go from observer to body
        /// coordinates.
        DMatrix observ2body_;

        /// Transformation matrix to go from body to observer
        /// coordinates.
        DMatrix body2observ_;

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

        /// Planetocentric latitude at picture center.
        double lat_at_center_;

        /// Longitude at picture center
        double lon_at_center_;

        /**
         * The cosine, &mu;, of the emission angle outside of which no
         * data will be plotted / retrieved.
         *
         * This is used to avoid mapping data close to the limb.
         */
        double mu_limit_;

        /// Take into account terminator when determing if lat/lon is
        /// visible.
        bool use_terminator_;

        /// Geometric/optical correction strategy used during
        /// latitude/longitude to pixel conversion, and vice versa.
        std::unique_ptr<GeometricCorrection> geometric_correction_;

    };

} // End MaRC namespace


#endif  /* MARC_VIEWING_GEOMETRY_H */
