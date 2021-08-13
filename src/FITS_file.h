// -*- C++ -*-
/**
 * @file FITS_file.h
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_FITS_FILE_H
#define MARC_FITS_FILE_H

#include "FITS_traits.h"

#include <optional>
#include <array>
#include <vector>
#include <string>
#include <memory>

#include <fitsio.h>


namespace MaRC
{
    namespace FITS
    {
        class image;

        /**
         * @brief Throw an exception if a CFITSIO error occurred.
         *
         * @param[in] status CFITSIO error status.
         *
         * @throw std::runtime_error CFITSIO error occurred.
         */
        void throw_on_error(int status);

        /**
         * @class file
         *
         * @brief Encapsulate basic %FITS image file operations.
         *
         * This class encapsulates operations performed on a %FITS
         * file containing an image, such as opening, closing, image
         * parameter retrieval, etc.
         *
         * @see output_file
         * @see input_file
         */
        class file
        {
        public:

            /**
             * @brief CFITSIO file close functor.
             *
             * This functor closes a %FITS file opened by CFITSIO
             * functions.  It is meant for use as the
             * @c std::unique_ptr @c Deleter template parameter.
             */
            struct closer
            {
                /**
                 * @param[in] fptr Pointer to CFITSIO file instance
                 *                 that will be closed.
                 */
                void operator()(fitsfile * fptr) const
                {
                    int status = 0;
                    fits_close_file(fptr, &status);

                    // At this point we don't really care about the
                    // status.  Don't bother checking it.
                }
            };

            /**
             * Type alias of @c std::shared_ptr that automatically
             * closes the managed CFITSIO file up on exiting the scope
             * in which an instance of this @c file_shared_ptr
             * resides.
             */
            using shared_ptr = std::shared_ptr<fitsfile>;

            /**
             * Type used to store a %FITS @c BLANK integer value.
             */
            using blank_type = std::optional<FITS::longlong_type>;

            /// Default constructor.
            file() = delete;

        protected:

            /**
             * @brief Constructor
             *
             * @param[in] filename Name of %FITS file to opened.
             * @param[in] create   Create file @a filename for writing
             *                     if @c true.  Otherwise open @a
             *                     filename read-only if @c false.
             *
             * @throw std::runtime_error Error opening %FITS file
             *                           @a filename.
             */
            explicit file(char const * filename, bool create = false);

            /// Destructor
            ~file() = default;

        public:

            // Prevent copying.
            file(file const &) = delete;
            void operator=(file const &) = delete;

            /// Get author.
            std::string author() const;

            /**
             * @brief Get map %FITS bits per pixel code.
             *
             * Obtain the bits per pixel "BITPIX" in the map %FITS
             * file, as defined by the %FITS standard.  This value may
             * either be supplied by the user or determined at
             * run-time based on source image data being mapped.
             *
             * @retval   8  8 bit unsigned integer data.
             * @retval  16 16 bit signed   integer data.
             * @retval  32 32 bit signed   integer data.
             * @retval  64 64 bit signed   integer data.
             * @retval -32 32 bit floating point   data.
             * @retval -64 64 bit floating point   data.
             *
             * @throw std::runtime_error Unable to determine bitpix
             *                           value.
             */
            int bitpix() const;

            /// Get the value for the map %FITS @c BZERO keyword.
            std::optional<double> bzero() const;

            /// Get the value for the map %FITS @c BSCALE keyword.
            std::optional<double> bscale() const;

            /// Get the value for the map %FITS @c BLANK keyword.
            blank_type blank() const;

            /// Get the physical unit of the image array values.
            std::string bunit() const;

            /**
             * @brief Get maximum valid physical value.
             *
             * @bug On platforms that implement the IEEE 754 floating
             *      point standard, the use of @c double as the
             *      underlying @c DATAMAX type will cause loss of
             *      precision if the %FITS @c DATAMAX values is an
             *      integer that requires more than 53 bits since the
             *      significand of a 64 bit IEEE 754 floating point
             *      value is only 53 bits wide.  Special handling will
             *      be need to be implemented to handle such a case.
             */
            std::optional<double> datamax() const;

            /**
             * @brief Get the minimum valid physical value.
             *
             * @bug On platforms that implement the IEEE 754 floating
             *      point standard, the use of @c double as the
             *      underlying @c DATAMIN type will cause loss of
             *      precision if the %FITS @c DATAMIN values is an
             *      integer that requires more than 53 bits since the
             *      significand of a 64 bit IEEE 754 floating point
             *      value is only 53 bits wide.  Special handling will
             *      be need to be implemented to handle such a case.
             */

            std::optional<double> datamin() const;

            /// Get the date of the observation.
            std::string date_obs() const;

            /// Get the equinox of the celestial coordinate system.
            std::optional<double> equinox() const;

            /**
             * @brief Get the name of the instrument that acquired the
             *        data.
             */
            std::string instrument() const;

            /// Get the number of axes in the image.
            int naxis() const;

            /**
             * @brief Get the size of all image axes.
             *
             * @attention %MaRC currently only supports of maximum of
             *            three dimensions when creating %FITS images,
             *            and two when reading them.
             */
            std::array<LONGLONG, 3> naxes() const;

            /// Get name of object being mapped.
            std::string object() const;

            /// Get who acquired the data.
            std::string observer() const;

            /**
             * @brief Set organization or institution responsible for
             *        creating the map.
             */
            void origin(std::string o);

            /**
             * @brief Get organization or institution responsible for
             *        creating the map.
             *
             * @todo We may not want to automatically pull this value
             *       from the source %FITS file since that "origin"
             *       may not be the same as the one creating the map.
             */
            std::string origin() const;

            /**
             * @brief Get bibliographic reference of publication
             *        associated with data.
             */
            std::string reference() const;

            /// Get name of telescope used to acquire the data.
            std::string telescope() const;
            //@}

        protected:

            /// Underlying CFITSIO @c fitsfile object.
            shared_ptr fptr_;

        };

        // -------------------------------------------------------

        /**
         * @class output_file
         *
         * @brief Encapsulate a %FITS image output file.
         *
         * This class encapsulates write operations performed on a
         * %FITS file containing an image.
         */
        class output_file : public file
        {
        public:

            /**
             * @brief Constructor
             *
             * @param[in] filename Name of %FITS file to create and
             *                     open for writing.
             *
             * @throw std::runtime_error Error opening %FITS file
             *                           @a filename.
             */
            explicit output_file(char const * filename);

            /// Destructor
            ~output_file() = default;

            /**
             * @brief Create a %FITS image array HDU.
             *
             * Create an image array in the %FITS file represented
             * by this @c output_file.
             *
             * @param[in] bitpix  Bits-per-pixel in the image,
             *                    corresponding to the %FITS @c BITPIX
             *                    keyword.
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
            std::unique_ptr<image> make_image(
                int bitpix,
                size_t samples,
                size_t lines,
                size_t planes,
                char const * extname = nullptr);

        };

        // -------------------------------------------------------

        /**
         * @class input_file
         *
         * @brief Encapsulate a %FITS image input file.
         *
         * This class encapsulates read operations performed on a
         * %FITS file containing an image.
         */
        class input_file : public file
        {
        public:

            /**
             * @brief Constructor
             *
             * @param[in] filename Name of %FITS file to open for
             *                     reading.
             *
             * @throw std::runtime_error Error opening %FITS file
             *                           @a filename.
             */
            explicit input_file(char const * filename);

            /// Destructor
            ~input_file() = default;

            /**
             * @brief Read the %FITS image into the given @c vector.
             *
             * @param[in,out] img     Vector that will contain the
             *                        read %FITS image data.
             * @param[out]    samples The number of columns in the %FITS
             *                        image.
             * @param[out]    lines   The number of rows in the %FITS
             *                        image.
             *
             * @throw std::runtime_error Error reading image from
             *                           %FITS file.
             *
             * @bug On platforms that implement the IEEE 754 floating
             *      point standard, a container of type @c double may
             *      not be suitable if the source image contains 64
             *      bit integer data.  Loss of precision could occur
             *      if the source data contains integers that require
             *      more than 53 bits since the significand of a 64
             *      bit IEEE 754 floating point value is only 53 bits
             *      wide.  Special handling will be need to be
             *      implemented to handle such a case.
             */
            void read(std::vector<double> & img,
                      std::size_t & samples,
                      std::size_t & lines) const;

        };

    }  // FITS
}  // MaRC


#endif  /* MARC_FITS_FILE_H */
