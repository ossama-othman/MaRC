//   -*- C++ -*-
/**
 * @file Orthographic.h
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017-2020  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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

    class ortho_map_parameters;
    class ortho_grid_parameters;

    /**
     * @enum GeometryType
     *
     * @brief Body center geometry type.
     *
     * @todo Improve the orthographic center API.
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

        // Disallow copying.
        Orthographic(Orthographic const &) = delete;
        Orthographic & operator=(Orthographic const &) = delete;

        // Disallow moving.
        Orthographic(Orthographic &&) noexcept = delete;
        Orthographic & operator=(Orthographic &&) = delete;

        /// Destructor
        ~Orthographic() override = default;

        /**
         * @name MapFactory Methods
         *
         * Factory methods required by the @c MapFactory abstract base
         * class.
         *
         * @see @c MapFactory
         */
        ///@{
        char const * projection_name() const override;
        ///@}

    private:

        /**
         * @brief Retrieve map size dependent parameters.
         *
         * Retrieve map parameters that may depend on the map
         * dimensions.
         *
         * @param[in]  samples    Number of samples in the map.
         * @param[in]  lines      Number of lines in the map.
         * @param[out] parameters Map parameters corresponding to the
         *                        provided @a samples and @a lines.
         */
        void map_parameters(std::size_t samples,
                            std::size_t lines,
                            ortho_map_parameters & parameters) const;

        /**
         * Create the Orthographic map projection.
         *
         * @see @c MaRC::MapFactory::plot_map().
         */
        void plot_map(std::size_t samples,
                      std::size_t lines,
                      plot_type const & plot) const override;

        /**
         * Create the Orthographic map latitude/longitude grid.
         *
         * @see @c MaRC::MapFactory::plot_grid().
         */
        void plot_grid(std::size_t samples,
                       std::size_t lines,
                       double lat_interval,
                       double lon_interval,
                       grid_type & grid) const override;

        /// Plot the Orthographic map latitude lines.
        void plot_lat_lines(std::size_t samples,
                            std::size_t lines,
                            ortho_grid_parameters const & p,
                            grid_type & grid) const;

        /// Plot the Orthographic map longitude lines.
        void plot_lon_lines(std::size_t samples,
                            std::size_t lines,
                            ortho_grid_parameters const & p,
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

        /// Longitude at center of projection (measured from bottom
        /// edge).
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
        /// Default Constructor.
        OrthographicCenter();

        /**
         * @brief Constructor.
         *
         * @param[in] type       Orthographic projection geometry
         *                       type, e.g. @c LAT_LON_GIVEN.
         * @param[in] sample_lat Sample or latitude at center of
         *                       body.
         * @param[in] line_lon   Line or longitude at center of body.
         */
        constexpr OrthographicCenter(GeometryType type,
                                     double sample_lat,
                                     double line_lon) noexcept
            : geometry(type)
            , sample_lat_center(sample_lat)
            , line_lon_center(line_lon)
        {
        }

        /// Type of body center geometry.
        GeometryType geometry;

        /// Sample or latitude at center of body.
        double sample_lat_center;

        /// Line or longitude at center of body.
        double line_lon_center;
    };

}


#endif  /* MARC_ORTHOGRAPHIC_H */
