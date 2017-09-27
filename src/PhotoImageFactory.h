// -*- C++ -*-
/**
 * @file PhotoImageFactory.h
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

#ifndef MARC_PHOTO_IMAGE_FACTORY_H
#define MARC_PHOTO_IMAGE_FACTORY_H

#include "ImageFactory.h"

#include "MaRC/PhotoImageParameters.h"
#include "MaRC/ViewingGeometry.h"

#include <vector>
#include <string>
#include <cstddef>


namespace MaRC
{

    /**
     * @class PhotoImageFactory
     *
     * @brief Factory class that create PhotoImage objects.
     *
     * This class creates PhotoImage objects.  It is designed to
     * decouple FITS (for example) file and image operations from the
     * PhotoImage class.  It also exists to decouple the MaRC parser
     * grammar from the PhotoImage class.  This allows PhotoImage
     * object creation to be delayed until it is time for the data in
     * the PhotoImage to be mapped, which reduces run-time memory
     * requirements.
     */
    class PhotoImageFactory : public ImageFactory
    {
    public:

        /// Constructor.
        /**
         * @param[in] filename Name of file containing image.
         */
        PhotoImageFactory(char const * filename,
                          std::unique_ptr<PhotoImageParameters> config,
                          std::unique_ptr<ViewingGeometry> geometry);

        /// Create a @c PhotoImage.
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so);

        /// Set the flat field image filename.
        void flat_field(char const * name);

        /// Enable the geometric correction strategy during
        /// lat/lon to pixel conversion, and vice-versa.
        void geometric_correction(bool enable);

        /// Enable the photometric correction strategy.
        void photometric_correction(bool enable);

        /// Set image interpolation flag.
        void interpolate(bool enable);

        /// Set the image inversion flags.
        void invert(bool vertical, bool horizontal);

        /// Invert image from left to right.
        /**
         * @param[in,out] img     Image to be inverted.
         * @param[in]     samples Number of samples in image.
         * @param[in]     lines   Number of lines   in image.
         */
        static void invert_h(std::vector<double> & image,
                             std::size_t samples,
                             std::size_t lines);

        /// Invert image from top to bottom.
        /**
         * @param[in,out] img     Image to be inverted.
         * @param[in]     samples Number of samples in image.
         * @param[in]     lines   Number of lines   in image.
         */
        static void invert_v(std::vector<double> & image,
                             std::size_t samples,
                             std::size_t lines);

    private:

        /// Perform flat-field correction on the photo image.
        /**
         * If a flat-field file was provided perform flat-field
         * correction on the photo image by substracting the
         * corresponding flat-field image elements from the photo
         * image.
         *
         * @param[in] naxes Photo image dimensions retrieved from its
         *                  FITS file.
         *
         * @return 0 on success.
         */
        int flat_field_correct(long const naxes[2],
                               std::vector<double> & img) const;

    private:

        /// Name of photo/image to be mapped.
        std::string const filename_;

        /// Name of flat field image to be substracted from the
        /// photo/image containing the actual data.
        std::string flat_field_;

        /// Enable/disable geometric correction.
        /**
         * @note Only GLL spacecraft geometric lens abberration
         *       correct is currently supported.
         */
        bool geometric_correction_;

        /// Enable/disable photometric correction.
        bool photometric_correction_;

        /// Perform pixel interpolation.
        bool interpolate_;

        /// Invert image top to bottom.
        bool invert_v_;

        /// Invert image left to right.
        bool invert_h_;

        /// @c PhotoImage configuration parameters.
        std::unique_ptr<PhotoImageParameters> config_;

        /// @c PhotoImage viewing geometry.
        std::unique_ptr<ViewingGeometry> geometry_;

    };

}

#endif  /* MARC_PHOTO_IMAGE_FACTORY_H */
