//   -*- C++ -*-
/**
 * @file MapCommand.h
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
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

#include <MaRC/MapFactory.h>

#include <fitsio.h>

#include <list>
#include <string>
#include <memory>


namespace MaRC
{

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

        using grid_type = MaRC::MapFactory::grid_type;
        using comment_list_type = std::list<std::string>;
        using image_factories_type =
            std::list<std::unique_ptr<MaRC::ImageFactory>>;

        /// Constructor.
        /**
         * @param[in,out] filename  Name of map output file.
         * @param[in,out] body_name Name of body being mapped.
         * @param[in]     samples   Number of samples in map.
         * @param[in]     lines     Number of lines in map.
         * @param[in,out] factory   @c MapFactory object responsible
         *                          for creating maps and grids.
         */
        MapCommand(std::string filename,
                   std::string body_name,
                   long samples,
                   long lines,
                   std::unique_ptr<MapFactory> factory);

        // Disallow copying.
        MapCommand(MapCommand const &) = delete;
        MapCommand & operator=(MapCommand const &) = delete;

        /// Destructor.
        virtual ~MapCommand() = default;

        /// Execute the command.
        int execute();

        /// Get name of projection.
        char const * projection_name() const;

        /// Set map author.
        void author(std::string author);

        /// Set organization or institution responsible for creating
        /// map.
        void origin(std::string origin);

        /// Set list of map comments to be written to FITS file.
        void comment_list (comment_list_type comments);

        /// Set list of grid comments to be written to FITS file.
        void xcomment_list(comment_list_type comments);

        /**
         * Set the latitude and longitude grid intervals.
         *
         * @note Call this method implicitly enables grid creation.
         */
         void grid_intervals(double lat_interval, double lon_interval);

        /// Set the value for the map FITS @c BZERO keyword.
        /**
         * The default value of the FITS @c BZERO keyword is zero.
         *
         * @param[in] zero @c BZERO keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       FITS file to be transformed according the equation:
         * @code
         *            (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void data_zero(double zero);

        /// Set the value for the map FITS @c BSCALE keyword.
        /**
         * The default value of the FITS @c BSCALE keyword is one.
         *
         * @param[in] scale @c BSCALE keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       FITS file to be transformed according the equation:
         * @code
         *         (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void data_scale(double scale);

        /// Set the value for the map FITS @c BLANK keyword.
        /**
         * The FITS @c BLANK keyword only applies to FITS images
         * containing integer types.  The corresponding "blank" value
         * for floating point FITS images is the IEEE "not-a-number"
         * constant.
         *
         * The @c BLANK keyword merely documents which physical (not
         * FITS) values in the image array are undefined.
         *
         * @param[in] blank FITS @c BLANK keyword value.
         */
        void data_blank(int blank);

        /// Set the @c ImageFactory list responsible for creating
        /// each of the planes in the map.
        void image_factories(image_factories_type factories);

    protected:

        /// Write @c VirtualImage information to FITS file.
        /**
         * Write information specific to @c VirtualImage (e.g.
         * @c MuImage) based map planes to the map FITS file.
         *
         * @param[in]  fptr       CFITSIO pointer to the map FITS
         *                        file.
         * @param[in]  plane      Map plane number of
         *                        @c VirtualImage.
         * @param[in]  num_planes Number of map planes being written
         *                        to the FITS file.
         * @param[in]  bitpix     Bits-per-pixel in the FITS file, as
         *                        defined by the FITS standard.
         * @param[in]  image      @c SourceImage object that may be a
         *                        @c VirtualImage about which facts
         *                        are being written to the FITS file.
         * @param[out] status     CFITSIO file operation status.
         */
        void write_virtual_image_facts(fitsfile * fptr,
                                       std::size_t plane,
                                       std::size_t num_planes,
                                       int bitpix,
                                       SourceImage const * image,
                                       int & status);

    private:

        /// Create FITS image array HDU.
        /**
         * @param[in]  fptr   CFITSIO pointer to the map FITS file.
         * @param[out] status CFITSIO file operation status.
         */
        virtual void initialize_FITS_image(fitsfile * fptr,
                                           int & status) = 0;

        /// Create and write map planes.
        /**
         * This is a "template method" (the design pattern, not a C++
         * class template member) that performs type-specific map
         * plane creation.
         *
         * @param[in]  fptr   CFITSIO pointer to the map FITS file.
         * @param[out] status CFITSIO file operation status.
         */
        virtual void make_map_planes(fitsfile * fptr, int & status) = 0;

        /// Create grid image.
        /**
         * This is a "template method" (the design pattern, not a C++
         * class template member) that calls back on the type-specific
         * @c MapFactory to create the grid.
         *
         * @param[in] samples      Number of samples in the map.
         * @param[in] lines        Number of lines   in the map.
         * @param[in] lat_interval Grid latitude  spacing.
         * @param[in] lon_interval Grid longitude spacing.
         *
         * @return @c Grid object containing grid image.
         */
        grid_type make_grid(long samples,
                            long lines,
                            double lat_interval,
                            double lon_interval);

        /**
         * @brief Write map grid to the map FITS file.
         *
         * @param[in]  fptr   CFITSIO pointer to the map FITS file.
         * @param[out] status CFITSIO file operation status.
         */
        void write_grid(fitsfile * fptr, int & status);

    protected:

        /// Number of samples in map.
        long const samples_;

        /// Number of lines in map.
        long const lines_;

        /// @c MapFactory object responsible for creating maps and
        /// grids.
        std::unique_ptr<MapFactory> const factory_;

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
        std::string const body_name_;

        /// Latitude grid line interval.
        double lat_interval_;

        /// Longitude grid line interval.
        double lon_interval_;

        /// Coefficient of linear term in the scaling equation.
        double bscale_;

        /// Physical value corresponding to an array value of zero.
        double bzero_;

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
