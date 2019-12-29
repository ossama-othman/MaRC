// -*- C++ -*-
/**
 * @file FITS_image.h
 *
 * Copyright (C) 2018-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_FITS_IMAGE_H
#define MARC_FITS_IMAGE_H

#include "FITS_file.h"

#include <fitsio.h>

namespace MaRC
{
    namespace FITS
    {

        /**
         * @class image
         *
         * @brief Encapsulate %FITS image creation.
         */
        class image
        {
        public:

            /// Convenience alias for the blank integer type.
            using blank_type = file::blank_type;

            /**
             * @brief Constructor
             *
             * Create an image array in the %FITS file represented by
             * the @a fptr object.
             *
             * @param[in] fptr    Underlying CFITSIO @c fitsfile
             *                    object.
             * @param[in] bitpix  Bits per pixel value as defined by
             *                    the %FITS standard.
             * @param[in] samples The number of columns in the %FITS
             *                    image.
             * @param[in] lines   The number of rows in the %FITS
             *                    image.
             * @param[in] planes  Number of planes in the %FITS
             *                    image.
             * @param[in] extname Name of %FITS image extension.  Set
             *                    to @c nullptr for primary %FITS
             *                    image.
             *
             * @throw std::runtime_error Error creating image in
             *                           %FITS file.
             */
            image(file::shared_ptr fptr,
                  int bitpix,
                  std::size_t samples,
                  std::size_t lines,
                  std::size_t planes,
                  char const * extname);

            image() = delete;
            image(image const &) = delete;
            image & operator=(image const &) = delete;

            /// Destructor.
            ~image();

            /**
             * @name Standard %FITS Keyword Accessors
             *
             * @brief Set values of standard %FITS keywords.
             *
             * These methods have corresponding standard %FITS
             * keywords, e.g. @c author() corresponds to the standard
             * %FITS @c AUTHOR keyword.
             */
            //@{
            /// Set author.
            void author(std::string const & a);

            /**
             * @brief Set the value for the map %FITS @c BZERO
             *        keyword.
             *
             * The default value of the %FITS @c BZERO keyword is
             * zero.
             *
             * @param[in] zero @c BZERO keyword value.
             *
             * @note Setting this value will cause the data written to
             *       the %FITS file to be transformed according the
             *       equation:
             * @code{.cpp}
             *     (FITS value) = ((physical value) - BZERO) / BSCALE
             * @endcode
             */
            void bzero(double zero);

            /**
             * @brief Set the value for the map %FITS @c BSCALE
             *        keyword.
             *
             * The default value of the %FITS @c BSCALE keyword is
             * one.
             *
             * @param[in] scale @c BSCALE keyword value.
             *
             * @note Setting this value will cause the data written to
             *       the %FITS file to be transformed according the
             *       equation:
             * @code{.cpp}
             *     (FITS value) = ((physical value) - BZERO) / BSCALE
             * @endcode
             */
            void bscale(double scale);

            /**
             * @brief Set the unit of physical data.
             *
             * Set the units of the physical data, i.e. image data
             * that should be scaled and offset according to the
             * following equation:
             *
             * @code{.cpp}
             *     physical_data = image_data * scale + offset;
             * @endcode
             *
             * The unit should conform to IAU Style Manual
             * recommendations.
             *
             * @see https://www.iau.org/publications/proceedings_rules/units/

             * @note This value corresponds to the %FITS "BUNIT" keyword.
             */
            void bunit(std::string const & unit);

            /**
             * @brief Set the value for the map %FITS @c BLANK
             *        keyword.
             *
             * The %FITS @c BLANK keyword only applies to %FITS images
             * containing integer types.  The corresponding "blank"
             * value for floating point %FITS images is the IEEE
             * "not-a-number constant.
             *
             * The @c BLANK keyword merely documents which physical
             * (not %FITS) values in the image array are undefined.
             *
             * @tparam    T     Image data type.
             * @param[in] blank %FITS @c BLANK keyword value.
             */
            template <typename T>
            void blank(blank_type blank);

            /**
             * @brief Set the minimum valid physical value.
             *
             * @tparam    T   Image data type.
             * @param[in] min Minimum valid physical value.
             */
            template <typename T>
            void datamin(T min);

            /**
             * @brief Set the maximum valid physical value.
             *
             * @tparam    T   Image data type.
             * @param[in] max Maximum valid physical value.
             */
            template <typename T>
            void datamax(T max);

            /// Set name of object being mapped.
            void object(std::string const & o);

            /**
             * @brief Set organization or institution responsible for
             *        creating the map.
             */
            void origin(std::string const & o);
            //@}

            /**
             * @brief Write a %FITS comment to the image.
             *
             * @param[in] c Comment to be written.
             */
            void comment(std::string const & c);

            /**
             * @brief Write a %FITS history comment to the image.
             *
             * @param[in] h History comment to be written.
             */
            void history(std::string const & h);

            /**
             * @brief Set internal CFITSIO scaling factors.
             *
             * Setting the internal scaling factors overrides use of
             * the @c BSCALE and @c BZERO %FITS values when writing
             * the data.  Data will be scaled using the internal
             * scaling factors instead.
             *
             * @param[in] scale  Internal linear scaling coefficient.
             *                   This overrides @c BSCALE.
             * @param[in] offset Internal linear offset.  This
             *                   overrides @c BZERO.
             */
            void internal_scale(double scale, double offset);

            /**
             * @brief Write the image into the %FITS file.
             *
             * @tparam    T   Image/data container type.
             * @param[in] img Array or vector containing the image
             *                data to be written to the %FITS file.
             *
             * @return @c true on success, and @c false otherwise.
             */
            template <typename T>
            bool write(T const & img);

        private:

            /**
             * @brief Set the value for the given %FITS keyword.
             *
             * @tparam    T       Value type.
             * @param[in] fptr    Pointer to CFITSIO @c fitsfile
             *                    object.
             * @param[in] key     %FITS keyword.
             * @param[in] value   %FITS keyword value.
             * @param[in] comment %FITS keyword comment.
             *
             * @throw std::runtime_error %FITS key could not be
             *                            updated.
             */
            template <typename T>
            void
            update_fits_key(fitsfile * fptr,
                            char const * key,
                            T value,
                            char const * comment);

            /**
             * @brief Set the %FITS keyword string value.
             *
             * @param[in] fptr    Pointer to CFITSIO @c fitsfile
             *                    object.
             * @param[in] key     %FITS keyword.
             * @param[in] value   %FITS keyword value.
             * @param[in] comment %FITS keyword comment.
             *
             * @throw std::runtime_error %FITS key could not be
             *                           updated.
             */
            void
            update_fits_key(fitsfile * fptr,
                            char const * key,
                            std::string const & value,
                            char const * comment);

            /**
             * @brief Set the %FITS keyword string value.
             *
             * @param[in] fptr    Pointer to CFITSIO @c fitsfile
             *                    object.
             * @param[in] key     %FITS keyword.
             * @param[in] value   %FITS keyword value.
             * @param[in] comment %FITS keyword comment.
             *
             * @throw std::runtime_error %FITS key could not be
             *                           updated.
             */            void
            update_fits_key(fitsfile * fptr,
                            char const * key,
                            char const * value,
                            char const * comment);

        private:

            /// Underlying CFITSIO @c fitsfile object.
            file::shared_ptr fptr_;

            /**
             * @brief First pixel in FITS image.
             *
             * @note @c LONGLONG is a CFITSIO type.
             */
            LONGLONG fpixel_;

            /// Pixels per image plane.
            LONGLONG const nelements_;

            /// Maximum number of elements in the %FITS image.
            LONGLONG const max_elements_;

        };

    }  // FITS
}  // MaRC


#include "FITS_image_t.cpp"

#endif  /* MARC_FITS_IMAGE_H */
