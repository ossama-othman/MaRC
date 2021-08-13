// -*- C++ -*-
/**
 * @file Mercator.h
 *
 * Copyright (C) 1999, 2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MERCATOR_H
#define MARC_MERCATOR_H

#include <marc/MapFactory.h>

#include <memory>


namespace MaRC
{
    class OblateSpheroid;

    /**
     * @class Mercator Mercator.h <marc/Mercator.h>
     *
     * @brief Mercator concrete map factory.
     *
     * A Mercator map contains data mapped in a way that retains
     * shape, i.e. the projection is conformal.  It also retains true
     * direction (unlike its transverse variant).
     *
     * @note A maximum latitude is currently not accepted as an option
     *       since the full 360 longitude range is mapped across the
     *       entire width of the generated map.  The maximum latitude
     *       is determined by the number of lines in the map.  The
     *       greater the number of lines in the map, the greater the
     *       latitude range in the map.
     *
     * @note This implementation can only map oblate spheroids or
     *       spheres.
     */
    class MARC_API Mercator final : public MapFactory
    {
    public:

        /// Type returned from @c make_grid() method.
        using typename MapFactory::grid_type;

        /// Type of functor passed to @c plot_map() method.
        using typename MapFactory::plot_type;

        /// Constructor.
        /**
         * @param[in] body    Pointer to @c OblateSpheroid object
         *                    representing body being mapped.
         */
        Mercator(std::shared_ptr<OblateSpheroid> body);

        // Disallow copying.
        Mercator(Mercator const &) = delete;
        Mercator & operator=(Mercator const &) = delete;

        // Disallow moving.
        Mercator(Mercator &&) noexcept = delete;
        Mercator & operator=(Mercator &&) = delete;

        /// Destructor
        ~Mercator() override = default;

        /**
         * @name @c MapFactory Methods
         *
         * Factory methods required by the @c MapFactory abstract base
         * class.
         *
         * @see @c MapFactory
         */
        //@{
        char const * projection_name() const override;
        //@}

        /**
         * @brief Scale distortion at given planetographic latitude
         *        @a latg on map.
         *
         * @param[in] latg Planetographic latitude.
         */
        double distortion(double latg) const;

    private:

        /**
         * Create the Mercator map projection.
         *
         * @see @c MaRC::MapFactory::plot_map().
         */
        void plot_map(std::size_t samples,
                      std::size_t lines,
                      plot_type const & plot) const override;

        /**
         * Create the Mercator map latitude/longitude grid.
         *
         * @see @c MaRC::MapFactory::plot_grid().
         */
        void plot_grid(std::size_t samples,
                       std::size_t lines,
                       double lat_interval,
                       double lon_interval,
                       grid_type & grid) const override;

        /// Orient longitude according to rotation direction
        /// (prograde/retrograde).
        /**
         * @param[in] i       Sample in map being mapped.
         * @param[in] samples Number of samples in image.
         *
         * @return Longitude at given sample @a i.
         */
        double get_longitude(std::size_t i, std::size_t samples) const;

    private:

        /// @c OblateSpheroid object representing the body being
        /// mapped.
        std::shared_ptr<OblateSpheroid> const body_;

    };

}


#endif
