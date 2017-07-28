// -*- C++ -*-
/**
 * @file SimpleCylindrical.h
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017  Ossama Othman
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


#ifndef MARC_SIMPLE_CYLINDRICAL_H
#define MARC_SIMPLE_CYLINDRICAL_H

#include <MaRC/MapFactory.h>

#include <memory>


namespace MaRC
{
    class BodyData;

    /**
     * @class SimpleCylindrical
     *
     * @brief Simple cylindrical concrete map factory.
     *
     * A Simple cylindrical map contains data mapped to a rectangular
     * latitude/longitude array.
     */
    template <typename T>
    class SimpleCylindrical : public MapFactory<T>
    {
    public:

        /// @typedef Type of map passed to @c plot_map() method.
        using MapFactory<T>::map_type;

        /// @typedef Type of grid passed to @c plot_grid() method.
        using MapFactory<T>::grid_type;

        /// Constructor.
        /**
         * @param[in] body        Pointer to BodyData object
         *                        representing body being mapped.
         * @param[in] lo_lat      Lower latitude  in simple
         *                        cylindrical map.
         * @param[in] hi_lat      Upper latitude  in simple
         *                        cylindrical map.
         * @param[in] lo_lon      Lower longitude in simple
         *                        cylindrical map.
         * @param[in] hi_lon      Upper longitude in simple
         *                        cylindrical map.
         * @param[in] graphic_lat Map bodygraphic latitudes instead of
         *                        bodycentric latitudes.
         */
        SimpleCylindrical (BodyData const & body,
                           double lo_lat,
                           double hi_lat,
                           double lo_lon,
                           double hi_lon,
                           bool graphic_lat);

        /// Destructor
        virtual ~SimpleCylindrical();

        /**
         * @name @c MapFactory Methods
         *
         * Methods required by the @c MapFactory abstract base class.
         *
         * @see @c MapFactory
         */
        //@{
        virtual const char * projection_name() const;
        //@}

  private:

        /**
         * @name Private @c MapFactory Methods
         *
         * Methods required by the @c MapFactory abstract base class.
         *
         * @see @c MapFactory and @c MapFactoryBase
         */
        //@{
        virtual const char * projection_name() const;
        virtual void plot_map(SourceImage const & source,
                              unsigned int samples,
                              unsigned int lines,
                              double minimum,
                              double maximum,
                              map_type & map);
        virtual void plot_grid(unsigned int samples,
                               unsigned int lines,
                               float lat_interval,
                               float lon_interval,
                               grid_type & grid);
        //@}

        /// Orient longitude according to rotation direction
        /// (prograde/retrograde).
        /**
         * @param[in] i       Sample in map being mapped.
         * @param[in] samples Number of samples in image.
         */
        inline double get_longitude (unsigned int i,
                                     unsigned int samples) const;

    private:

        /// BodyData object representing the body being mapped.
        BodyData const & body_;

        /// Lower latitude in simple cylindrical map.
        double lo_lat_;

        /// Upper latitude in simple cylindrical map.
        double hi_lat_;

        /// Lower longitude in simple cylindrical map.
        double lo_lon_;

        /// Upper longitude in simple cylindrical map.
        double hi_lon_;

        /// Flag that determines if bodygraphic latitudes are mapped
        /// instead of bodycentric latitudes.
        bool const graphic_lat_;

    };

}


#include "MaRC/SimpleCylindrical.cpp"

#endif
