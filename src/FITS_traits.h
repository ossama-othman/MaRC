// -*- C++ -*-
/**
 * @file FITS_traits.h
 *
 * Copyright (C) 2004, 2017, 2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_FITS_TRAITS_H
#define MARC_FITS_TRAITS_H

#include <fitsio.h>

#include <cstdint>


namespace MaRC
{
    /**
     * @namespace FITS
     *
     * @brief %FITS related types and traits.
     *
     * This namespace contains types and traits corresponding to the
     * data types used in %FITS images.
     */
    namespace FITS
    {
        /**
         * @name Numerical Data Types
         *
         * Types suitable for storing the numerical data types listed
         * in the %FITS standard.
         */
        //@{
        /// 8-bit unsigned integer type.
        using byte_type     = std::uint8_t;

        /// 16-bit signed integer type.
        using short_type    = std::int16_t;

        /// 32-bit signed integer type.
        using long_type     = std::int32_t;

        /// 64-bit signed integer type.
        using longlong_type = std::int64_t;

        /// 32-bit floating point type.
        using float_type    = float;

        /// 64-bit floating point type.
        using double_type   = double;
        //@}

        /**
         * @struct traits
         *
         * @brief Traits class template forward declaration for
         *        type-specific %FITS parameters.
         *
         * This class template must be fully specialized for each
         * %FITS numerical data type supported by both %MaRC and
         * CFITSIO.  Each specialization should contain the following
         * type-specific traits:
         *
         * @code{.cpp}
         * // CFITSIO value corresponding to FITS BITPIX keyword
         * // e.g. SHORT_IMG), i.e. the type of data written to a FITS
         * // file.
         * static constexpr int bitpix = ...;
         *
         * // CFITSIO type code corresponding to the data array type
         * // in memory being written  (e.g. TSHORT).
         * //
         * // If this type differs from the type stored in the FITS
         * // file as specified by the BITPIX keyword, CFITSIO will
         * // perform automatic data conversion as the data array is
         * // written to the FITS file.
         * static constexpr int datatype = ...;
         * @endcode
         *
         * @tparam T Data type.
         *
         * @note The traits in this class are specific to the CFITSIO
         *       library API.
         */
        template <typename T>
        struct traits;

        /**
         * @brief 8-bit unsigned integer @c traits specialization.
         *
         * @see @c traits.
         */
        template <>
        struct traits<byte_type>
        {
            /**
             * @name Specialized Members
             *
             * @c byte_type traits specializations.
             */
            //@{
            static constexpr int  bitpix   = BYTE_IMG;
            static constexpr int  datatype = TBYTE;
            //@}
        };

        /**
         * @brief 16-bit signed integer @c traits specialization.
         *
         * @see @c traits.
         */
        template <>
        struct traits<short_type>
        {
            /**
             * @name Specialized Members
             *
             * @c short_type traits specializations.
             */
            //@{
            static constexpr int  bitpix   = SHORT_IMG;
            static constexpr int  datatype = TSHORT;
            //@}
        };

        /**
         * @brief 32-bit signed integer @c traits specialization.
         *
         * @see @c traits.
         */
        template <>
        struct traits<long_type>
        {
            /**
             * @name Specialized Members
             *
             * @c long_type traits specializations.
             */
            //@{
            static constexpr int  bitpix   = LONG_IMG;
            /**
             * @note @c long may be 8 bytes on some 64 bit platforms
             *       so go with the CFITSIO @c TINT type code instead
             *       of @c TLONG to make sure CFITSIO knows we are
             *       using an array of 4 byte integers.
             */
            static constexpr int  datatype = TINT;
            //@}
        };

        /**
         * @brief 64-bit signed integer @c traits specialization.
         *
         * @see @c traits.
         */
        template <>
        struct traits<longlong_type>
        {
            /**
             * @name Specialized Members
             *
             * @c longlong_type traits specializations.
             */
            //@{
            static constexpr int  bitpix   = LONGLONG_IMG;
            static constexpr int  datatype = TLONGLONG;
            //@}
        };

        /**
         * @brief 32-bit floating point @c traits specialization.
         *
         * @see @c traits.
         */
        template <>
        struct traits<float>
        {
            /**
             * @name Specialized Members
             *
             * @c float traits specializations.
             */
            //@{
            static constexpr int  bitpix   = FLOAT_IMG;
            static constexpr int  datatype = TFLOAT;
            //@}
        };

        /**
         * @brief 64-bit floating point @c traits specialization.
         *
         * @see @c traits.
         */
        template <>
        struct traits<double>
        {
            /**
             * @name Specialized Members
             *
             * @c double traits specializations.
             */
            //@{
            static constexpr int  bitpix   = DOUBLE_IMG;
            static constexpr int  datatype = TDOUBLE;
            //@}
        };

    }  // FITS
}  // MaRC


#endif  /* MARC_FITS_TRAITS_H */
