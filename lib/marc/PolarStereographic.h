// -*- C++ -*-
/**
 * @file PolarStereographic.h
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_POLAR_STEREOGRAPHIC_H
#define MARC_POLAR_STEREOGRAPHIC_H

#include <marc/MapFactory.h>

#include <memory>


namespace MaRC
{
    class OblateSpheroid;

    /**
     * @class PolarStereographic PolarStereographic.h <marc/PolarStereographic.h>
     *
     * @brief Polar Stereographic concrete map factory.
     *
     * A Polar Stereographic map contains data mapped in a way that
     * retains shape, i.e. the projection is conformal.  One of the
     * poles will be at the center of the map.
     *
     * @note This implementation can only map oblate spheroids or
     *       spheres.
     */
    class MARC_API PolarStereographic final : public MapFactory
    {
    public:

        /// Type returned from @c make_grid() method.
        using typename MapFactory::grid_type;

        /// Type of functor passed to @c plot_map() method.
        using typename MapFactory::plot_type;

        /// Constructor.
        /**
         * @param[in] body       Pointer to @c OblateSpheroid object
         *                       representing body being mapped.
         * @param[in] max_lat    Maximum planetoCENTRIC latitude to
         *                       map in degrees.  For example, given a
         *                       map with 50 samples and 25 lines,
         *                       @a max_lat will be at the lower edge
         *                       of line 1 and the upper edge of line
         *                       25.
         * @param[in] north_pole Place north_pole if @c true.  South
         *                       pole otherwise.
         */
        PolarStereographic(std::shared_ptr<OblateSpheroid> body,
                           double max_lat,
                           bool north_pole);

        // Disallow coping.
        PolarStereographic(PolarStereographic const &) = delete;
        PolarStereographic & operator=(PolarStereographic const &) = delete;

        // Disallow moving.
        PolarStereographic(PolarStereographic &&) noexcept = delete;
        PolarStereographic & operator=(PolarStereographic &&) = delete;

        /// Destructor
        ~PolarStereographic() override = default;

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

        ///
        /**
         * @brief Scale distortion at given planetographic latitude
         *        @a latg on map.
         * @param[in] latg Planetographic latitude.
         */
        double distortion(double latg) const;

    private:

        /**
         * Create the Polar Stereographic map projection.
         *
         * @see @c MaRC::MapFactory::plot_map().
         */
        void plot_map(std::size_t samples,
                      std::size_t lines,
                      plot_type const & plot) const override;

        /**
         * Create the Polar Stereographic map latitude/longitude
         * grid.
         *
         * @see @c MaRC::MapFactory::plot_grid().
         */
        void plot_grid(std::size_t samples,
                       std::size_t lines,
                       double lat_interval,
                       double lon_interval,
                       grid_type & grid) const override;

    private:

        /// The underlying Polar Stereographic projection equation.
        /**
         * @param[in] latg Planetographic latitude.
         *
         * @return Value of point on projection along a radial line
         *         (e.g. along a longitude line).
         */
        double stereo_rho(double latg) const;

    private:

        /// @c OblateSpheroid object representing the body being
        /// mapped.
        std::shared_ptr<OblateSpheroid> const body_;

        /// Maximum planetoCENTRIC latitude to map in radians.
        double const max_lat_;

        /// Coefficient used in map equation.
        double const rho_coeff_;

        /// Coefficient used in scale distortion equation.
        double const distortion_coeff_;

        /// @c true if north pole is at center of map.  @c false if
        /// south pole is at center.
        bool const north_pole_;

    };

}


#endif
