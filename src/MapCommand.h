//   -*- C++ -*-
/**
 * @file MapCommand.h
 *
 * Copyright (C) 2004, 2017-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MAP_COMMAND_H
#define MARC_MAP_COMMAND_H

#include "FITS_file.h"
#include "SourceImageFactory.h"
#include "MapParameters.h"

#include <marc/MapFactory.h>

#include <list>
#include <string>
#include <memory>


namespace MaRC
{
    /**
     * @class MapCommand
     *
     * @brief Drive creation of a single map and grid.
     */
    class MapCommand
    {
    public:

        /// Grid image type.
        using grid_type = MaRC::MapFactory::grid_type;

        /// Source image factories type.
        using image_factories_type =
            std::list<std::unique_ptr<MaRC::SourceImageFactory>>;

        /// Constructor.
        /**
         * @param[in,out] filename  Name of map output file.
         * @param[in]     samples   Number of samples in map.
         * @param[in]     lines     Number of lines in map.
         * @param[in,out] factory   @c MapFactory object responsible
         *                          for creating maps and grids.
         * @param[in,out] params    Map parameters.
         */
        MapCommand(std::string filename,
                   long samples,
                   long lines,
                   std::unique_ptr<MapFactory> factory,
                   std::unique_ptr<MapParameters> params);

        // Disallow copying.
        MapCommand(MapCommand const &) = delete;
        MapCommand & operator=(MapCommand const &) = delete;

        /// Destructor.
        ~MapCommand() = default;

        /// Execute the command.
        int execute();

        /// Get map file name.
        std::string const & filename() const { return this->filename_; }

        /// Get name of projection.
        char const * projection_name() const;

        /**
         * @brief Set the latitude and longitude grid intervals.
         *
         * @note Calling this method implicitly enables grid
         *       creation.
         */
         void grid_intervals(double lat_interval, double lon_interval);

        /**
         * Set the @c SourceImageFactory list responsible for creating
         * each of the planes in the map.
         */
        void image_factories(image_factories_type factories);

    private:

        /**
         * @brief Write @c VirtualImage information to %FITS file.
         *
         * Write information specific to @c VirtualImage (e.g.
         * @c MuImage) based map planes to the map %FITS file.
         *
         * @param[in] map_image  %FITS image array HDU enscapulation.
         * @param[in] plane      Map plane number of @c VirtualImage.
         * @param[in] num_planes Number of map planes being written
         *                       to the %FITS file.
         * @param[in] image      @c SourceImage object that may be a
         *                       @c VirtualImage about which facts
         *                       are being written to the %FITS
         *                       file.
         */
        void write_virtual_image_facts(FITS::image & map_image,
                                       std::size_t plane,
                                       std::size_t num_planes,
                                       SourceImage const * image);

        /**
         * @brief Create and write map planes.
         *
         * @tparam        T    Map data type.
         * @param[in,out] file Object representing %FITS output file.
         */
        template <typename T>
        void make_map_planes(MaRC::FITS::output_file & file);

        /**
         * @brief Create grid image.
         *
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
         * @brief Write map grid to the map %FITS file.
         *
         * @param[in,out] file Object representing %FITS output file.
         */
        void write_grid(MaRC::FITS::output_file & file);

        /**
         * @brief Automatically populate map parameters.
         *
         * Populate parameters automatically, such as from
         * @c SourceImage parameters, where possible.
         *
         * @return @c true if map parameter population succeded, and
         *         @c false otherwise.
         *
         * @note This method should be called after all required or
         *       user provided map parameters have been set so that
         *       inconsistencies or inadequacies in the user choices
         *       with respect to source image characteristics may be
         *       flagged prior to mapping.
         */
        bool populate_map_parameters();

    private:

        /// Number of samples in map.
        long const samples_;

        /// Number of lines in map.
        long const lines_;

        /**
         * @c MapFactory object responsible for creating maps and
         * grids.
         */
        std::unique_ptr<MapFactory> const factory_;

        /**
         * List of @c SourceImageFactory objects that create the
         * @c SourceImage to be mapped on each map plane.
         */
        image_factories_type image_factories_;

        /// Map filename.
        std::string const filename_;

        /// Latitude grid line interval.
        double lat_interval_;

        /// Longitude grid line interval.
        double lon_interval_;

        /**
         * @brief Flag that determines if data written to the %FITS
         *        map file is transformed using a linear equation.
         *
         * The transformation equation used when writing data to a
         * %FITS file is:
         * @code{.cpp}
         *   %FITS value = (physical value - BZERO) / BSCALE
         * @endcode
         * Meaning that the data read from the %FITS file will be
         * transformed according to the linear equation:
         * @code{.cpp}
         *   physical value = %FITS value * BSCALE + BZERO
         * @endcode
         */
        bool transform_data_;

        /// Flag that determines if a grid is created.
        bool create_grid_;

        /// %FITS related map parameters.
        std::unique_ptr<MapParameters> parameters_;

    };

}


#endif  /* MARC_MAP_COMMAND_H */
