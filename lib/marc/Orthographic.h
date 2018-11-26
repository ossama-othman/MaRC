//   -*- C++ -*-
/**
 * @file Orthographic.h
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017-2018  Ossama Othman
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

#ifndef MARC_ORTHOGRAPHIC_H
#define MARC_ORTHOGRAPHIC_H

#include <marc/MapFactory.h>

#include <memory>


namespace MaRC
{
    class OblateSpheroid;
    struct OrthographicCenter;

    /**
     * @enum GeometryType
     *
     * @brief Body center geometry type..
     */
    enum GeometryType { DEFAULT, CENTER_GIVEN, LAT_LON_GIVEN };

    /**
     * @class Orthographic Orthographic.h <marc/Orthographic.h>
     *
     * @brief Orthographic concrete map factory.
     *
     * This class creates orthographic map projections.  Observers in
     * orthographic projections are an infinite distance away (range
     * approaches infinity).
     *
     * @note Only bodies modeled as oblate spheroids are supported by
     *       this implementation.
     */
    class MARC_API Orthographic final : public MapFactory
    {
    public:

        /// Type of grid passed to @c plot_grid() method.
        using typename MapFactory::grid_type;

        /// Type of functor passed to @c plot_map() method.
        using typename MapFactory::plot_type;

        /// Constructor.
        /**
         * @param[in] body           Pointer to @c OblateSpheroid
         *                           object representing body.
         * @param[in] sub_observ_lat Planetocentric sub-observer latitude
         *                           in degrees.
         * @param[in] sub_observ_lon Sub-observer longitude in
         *                           degrees.
         * @param[in] position_angle Position (north) angle of body in
         *                           orthographic projection.
         * @param[in] km_per_pixel   Number of kilometer per pixel in
         *                           orthographic projection.
         * @param[in] center         Structure containing body center
         *                           information to be used in the
         *                           projection.
         */
        Orthographic(std::shared_ptr<OblateSpheroid> body,
                     double sub_observ_lat,
                     double sub_observ_lon,
                     double position_angle,
                     double km_per_pixel,
                     OrthographicCenter const & center);

        /// Destructor
        virtual ~Orthographic() = default;

        /**
         * @name @c MapFactory Methods
         *
         * Factory methods required by the @c MapFactory abstract base
         * class.
         *
         * @see @c MapFactory
         */
        //@{
        virtual char const * projection_name() const;
        //@}

    private:

        /// Retrieve map size dependent parameters.
        /**
         * Retrieve map parameters that may depend on the map
         * dimensions.
         *
         * @param[in]  samples       Number of samples in the map.
         * @param[in]  lines         Number of lines in the map.
         * @param[out] km_per_pixel  The number of kilometers per
         *                           pixel in the orthographic
         *                           projection.
         * @param[out] sample_center Body center sample in projection
         *                           (measured from left edge).
         * @param[out] line_center   Body center line in projection
         *                           (measured from bottom edge).
         */
        void map_parameters(std::size_t samples,
                            std::size_t lines,
                            double & km_per_pixel,
                            double & sample_center,
                            double & line_center) const;

        /**
         * Create the Orthographic map projection.
         *
         * @see @c MaRC::MapFactory::plot_map().
         */
        virtual void plot_map(std::size_t samples,
                              std::size_t lines,
                              plot_type plot) const;

        /**
         * Create the Orthographic map latitude/longitude grid.
         *
         * @see @c MaRC::MapFactory::plot_grid().
         */
        virtual void plot_grid(std::size_t samples,
                               std::size_t lines,
                               double lat_interval,
                               double lon_interval,
                               grid_type & grid) const;

    private:

        /// OblateSpheroid object representing the body being mapped.
        std::shared_ptr<OblateSpheroid> const body_;

        /// Sub-observation latitude.
        double sub_observ_lat_;

        /// Sub-observation longitude.
        double sub_observ_lon_;

        /// Map position angle (measured counter-clockwise positive).
        double PA_;

        /// The number of kilometers per pixel in the orthographic
        /// projection.
        double km_per_pixel_;

        /// Body center sample in projection (measured from left
        /// edge).
        double sample_center_;

        /// Body center line in projection (measured from bottom
        /// edge).
        double line_center_;

        /// Latitude at center of projection (measured from left
        /// edge).
        double lat_at_center_;

        /// Line center of projection (measured from bottom edge).
        double lon_at_center_;

        /// True if creating polar projection.
        bool polar_;

    };

    /**
     * @struct OrthographicCenter Orthographic.h <marc/Orthographic.h>
     *
     * @brief Center of body in orthographic projection.
     *
     * These values dictate where the center of the body being mapped
     * will be placed in the orthographic projection.
     */
    struct MARC_API OrthographicCenter
    {
        OrthographicCenter();

        /// Type of body center geometry.
        GeometryType geometry;

        /// Sample or latitude at center of body.
        double sample_lat_center;

        /// Line or longitude at center of body.
        double line_lon_center;
    };

}


#endif  /* MARC_ORTHOGRAPHIC_H */
