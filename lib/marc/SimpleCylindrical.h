// -*- C++ -*-
/**
 * @file SimpleCylindrical.h
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */


#ifndef MARC_SIMPLE_CYLINDRICAL_H
#define MARC_SIMPLE_CYLINDRICAL_H

#include <marc/MapFactory.h>
#include <marc/BodyData.h>

#include <memory>


namespace MaRC
{

    /**
     * @class SimpleCylindrical SimpleCylindrical.h <marc/SimpleCylindrical.h>
     *
     * @brief Simple cylindrical concrete map factory.
     *
     * A Simple cylindrical map contains data mapped to a rectangular
     * latitude/longitude array, where the spacing the between a given
     * number of degrees of latitude and longitude is equal.  This
     * projection is more formally known as the Plate Carrée
     * projection, as well as rectangular, equirectangular and
     * equidistant cylindrical.
     */
    class MARC_API SimpleCylindrical final : public MapFactory
    {
    public:

        /// Type of grid passed to @c plot_grid() method.
        using typename MapFactory::grid_type;

        /// Type of functor passed to @c plot_map() method.
        using typename MapFactory::plot_type;

        /// Constructor.
        /**
         * @param[in] body        Pointer to @c BodyData object
         *                        representing body being mapped.
         * @param[in] lo_lat      Planetocentric lower latitude in
         *                        degrees in simple cylindrical map.
         * @param[in] hi_lat      Planetocentric upper latitude in
         *                        degrees in simple cylindrical map.
         * @param[in] lo_lon      Lower longitude in degrees in simple
         *                        cylindrical map.
         * @param[in] hi_lon      Upper longitude in degrees in simple
         *                        cylindrical map.
         * @param[in] graphic_lat Map planetographic latitudes instead
         *                        of planetocentric latitudes.
         */
        SimpleCylindrical(std::shared_ptr<BodyData> body,
                          double lo_lat,
                          double hi_lat,
                          double lo_lon,
                          double hi_lon,
                          bool graphic_lat);

        // Disallow coping.
        SimpleCylindrical(SimpleCylindrical const &) = delete;
        SimpleCylindrical & operator=(SimpleCylindrical const &) = delete;

        // Disallow moving.
        SimpleCylindrical(SimpleCylindrical &&) noexcept = delete;
        SimpleCylindrical & operator=(SimpleCylindrical &&) = delete;

        /// Destructor
        ~SimpleCylindrical() override = default;

        /**
         * @name MapFactory Methods
         *
         * Methods required by the @c MapFactory abstract base class.
         *
         * @see @c MapFactory
         */
        ///@{
        char const * projection_name() const override;
        ///@}

    private:

        /**
         * Create the Simple Cylindrical map projection.
         *
         * @see @c MaRC::MapFactory::plot_map().
         */
        void plot_map(std::size_t samples,
                      std::size_t lines,
                      plot_type const & plot) const override;

        /**
         * Create the Simple Cylindrical map latitude/longitude grid.
         *
         * @see @c MaRC::MapFactory::plot_grid().
         */
        void plot_grid(std::size_t samples,
                       std::size_t lines,
                       double lat_interval,
                       double lon_interval,
                       grid_type & grid) const override;

        /**
         * @brief Orient longitude according to rotation direction
         *        (prograde/retrograde).
         *
         * @param[in] i  Sample in map being mapped.
         * @param[in] cf Longitudes (radians) per sample.
         *
         * return Longitude in radians.
         */
        inline double get_longitude(std::size_t i, double cf) const
        {
            // Compute longitude at center of pixel.
            double lon = (i + 0.5) * cf;

            // PROGRADE:   West longitudes (increasing to the left)
            // RETROGRADE: East longitudes (increasing to the right)

            if (this->body_->prograde ())
                lon = this->hi_lon_ - lon;
            else
                lon += this->lo_lon_;

            return lon;
        }

    private:

        /// @c BodyData object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Lower latitude in simple cylindrical map.
        double const lo_lat_;

        /// Upper latitude in simple cylindrical map.
        double const hi_lat_;

        /// Lower longitude in simple cylindrical map.
        double lo_lon_;

        /// Upper longitude in simple cylindrical map.
        double hi_lon_;

        /// Flag that determines if planetographic latitudes are
        /// mapped instead of planetocentric latitudes.
        bool const graphic_lat_;

    };

}


#endif
