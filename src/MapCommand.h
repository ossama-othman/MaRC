//   -*- C++ -*-
/**
 * @file MapCommand.h
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

#ifndef MARC_MAP_COMMAND_H
#define MARC_MAP_COMMAND_H

#include "ImageFactory.h"

#include <fitsio.h>

#include <list>
#include <string>
#include <memory>


namespace MaRC
{
    class Grid;

    /**
     * @class MapCommand
     *
     * @brief Abstract base class to be subclassed by concrete MaRC
     *        Command classes.
     *
     * All concrete MaRC Command classes must implement the interface
     * dictated by this abstract base class.
     */
    class MapCommand
    {
    public:

        typedef std::list<std::string> comment_list_type;
        typedef std::list<std::unique_ptr<MaRC::ImageFactory>>
        image_factories_type;

        /// Constructor.
        /**
         * @param[in,out] filename  Name of map output file.
         * @param[in,out] body_name Name of body being mapped.
         * @param[in]     samples   Number of samples in map.
         * @param[in]     lines     Number of lines in map.
         */
        MapCommand(std::string filename,
                   std::string body_name,
                   long samples,
                   long lines);

        // Disallow copying.
        MapCommand(MapCommand const &) = delete;
        MapCommand & operator=(MapCommand const &) = delete;

        /// Destructor.
        virtual ~MapCommand();

        /// Execute the command.
        virtual int execute();

        /// Get name of projection.
        virtual char const * projection_name() const = 0;

        /// Create FITS image array HDU.
        virtual void initialize_FITS_image(fitsfile * fptr,
                                           int & status) = 0;

        /// Create and write map planes.
        /**
         * This is a "template method" (the design pattern, not a C++
         * method) that performs type-specific map plane creation.
         */
        virtual void make_map_planes(fitsfile * fptr, int & status) = 0;

        /// Create and write grid image.
        /**
         * This is a "template method" (the design pattern, not a C++
         * method) that calls back on the type-specific MapFactory to
         * create the grid.
         *
         * @return @c Grid object containing grid image.
         */
        virtual Grid * make_grid(long samples,
                                 long lines,
                                 float lat_interval,
                                 float lon_interval) = 0;

        /// Set map author.
        void author(std::string const & author);

        /// Set organization or institution responsible for creating
        /// map.
        void origin(std::string const & origin);

        /// Set list of map comments to be written to FITS file.
        void comment_list (comment_list_type const & comments);

        /// Set list of grid comments to be written to FITS file.
        void xcomment_list(comment_list_type const & comments);

        /**
         * Set the latitude and longitude grid intervals.
         *
         * @note Call this method implicitly enables grid creation.
         */
         void grid_intervals(float lat_interval, float lon_interval);

        /// Set the value for the map FITS @c BZERO keyword.
        /**
         * The default value of the FITS @c BZERO keyword is zero.
         *
         * @param zero @c BZERO keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       FITS file to be transformed according the equation:
         * @code
         *            (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void data_zero(float zero);

        /// Set the value for the map FITS @c BSCALE keyword.
        /**
         * The default value of the FITS @c BSCALE keyword is one.
         *
         * @param scale @c BSCALE keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       FITS file to be transformed according the equation:
         * @code
         *            (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void data_scale(float scale);

        /// Set the value for the map FITS @c BLANK keyword.
        /**
         * The FITS @c BLANK keyword only applies to FITS images
         * containing integer types.  The corresponding "blank" value for
         * floating point FITS images is the IEEE "not-a-number"
         * constant.
         * @par
         * The @c BLANK keyword merely documents which physical (not FITS)
         * values in the image array are undefined.
         *
         * @param blank @c BLANK keyword value.
         */
        void data_blank(int blank);

        /// Set the list of @c ImageFactorys responsible for creating
        /// each of the planes in the map.
        void image_factories(image_factories_type const & factories);

    protected:

        /// Number of samples in map.
        long const samples_;

        /// Number of lines in map.
        long const lines_;

        /// List of @c ImageFactorys that create the @c SourceImage to
        /// be mapped on each map plane.
        image_factories_type image_factories_;

        /// Flag that determines whether the FITS @c BLANK value is
        /// set.
        bool blank_set_;

        /// Value of pixels with undefined physical value.
        int blank_;

    private:

        /// Map filename.
        std::string const filename_;

        /// Person responsible for compiling the data in the map.
        std::string author_;

        /// Organization or institution responsible for creating this
        /// FITS file.
        std::string origin_;

        /// Map comments.
        std::list<std::string> comments_;

        /// Grid comments.
        std::list<std::string> xcomments_;

        /// Name of body to be mapped.
        std::string body_name_;

        /// Latitude grid line interval.
        float lat_interval_;

        /// Longitude grid line interval.
        float lon_interval_;

        /// Coefficient of linear term in the scaling equation.
        float bscale_;

        /// Physical value corresponding to an array value of zero.
        float bzero_;

        /// Flag that determines if data written to the FITS map file
        /// is transformed using a linear equation.
        /**
         * The transformation equation used when writing data to a
         * FITS file is:
         * @code
         *   FITS value = (physical value - BZERO) / BSCALE
         * @endcode
         * Meaning that the data read from the FITS file will be
         * transformed according to the linear equation:
         * @code
         *   physical value = FITS value * BSCALE + BZERO
         * @endcode
         */
        bool transform_data_;

        /// Flag that determines if a grid is created.
        bool create_grid_;

    };

}


#endif  /* MARC_MAP_COMMAND_H */
