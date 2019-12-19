// -*- C++ -*-
/**
 * @file PhotoImageFactory.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PHOTO_IMAGE_FACTORY_H
#define MARC_PHOTO_IMAGE_FACTORY_H

#include "SourceImageFactory.h"

#include "marc/PhotoImageParameters.h"
#include "marc/ViewingGeometry.h"

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
     * decouple %FITS (for example) file and image operations from the
     * PhotoImage class.  It also exists to decouple the %MaRC parser
     * grammar from the PhotoImage class.  This allows PhotoImage
     * object creation to be delayed until it is time for the data in
     * the PhotoImage to be mapped, which reduces run-time memory
     * requirements.
     */
    class PhotoImageFactory final : public SourceImageFactory
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param[in] filename Name of file containing image.
         */
        PhotoImageFactory(char const * filename);

        /// Destructor.
        ~PhotoImageFactory() override = default;

        /// Create a @c PhotoImage.
        std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) override;

        /// Set the flat field image filename.
        void flat_field(char const * name);

        /// Enable the geometric correction strategy during
        /// lat/lon to pixel conversion, and vice-versa.
        void geometric_correction(bool enable);

        /// Enable the photometric correction strategy.
        // void photometric_correction(bool enable);

        /// Set image interpolation flag.
        void interpolate(bool enable);

        /// Set the image inversion flags.
        void invert(bool vertical, bool horizontal);

        /// Set @c PhotoImage configuration parameters.
        void photo_config(std::unique_ptr<PhotoImageParameters> config);

        /// Set @c PhotoImage viewing geometry.
        void viewing_geometry(std::unique_ptr<ViewingGeometry> geometry);

    private:

        /// Perform flat-field correction on the photo image.
        /**
         * If a flat-field file was provided perform flat-field
         * correction on the photo image by substracting the
         * corresponding flat-field image elements from the photo
         * image.
         *
         * @param[in,out] img     Image to be flat-field corrected.
         * @param[in]     samples Number of samples in the image to be
         *                        flat-field corrected.
         * @param[in]     lines   Number of samples in the image to be
         *                        flat-field corrected.
         */
        void flat_field_correct(std::vector<double> & img,
                                std::size_t samples,
                                std::size_t lines) const;

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
        // bool photometric_correction_;

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
