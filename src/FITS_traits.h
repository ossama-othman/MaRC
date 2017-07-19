// -*- C++ -*-

//==========================================================================
/**
 *  @file FITS_traits.h
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_FITS_TRAITS_H
#define MARC_FITS_TRAITS_H

#include <fitsio.h>

#include <cstdint>


namespace MaRC
{
    /**
     * @namespace FITS
     *
     * @brief FITS related types and traits.
     *
     * This namespace contains types and traits corresponding to the
     * data types used in FITS images.
     */
    namespace FITS
    {
        typedef uint8_t byte_type;     // Unsigned
        typedef int16_t short_type;    // Signed
        typedef int32_t long_type;
        typedef int64_t longlong_type;
        typedef float   float_type;
        typedef double  double_type;

        /**
         * @struct traits
         *
         * @brief Traits class for type-specific FITS parameters.
         *
         * @note The traits in this class are specific to the CFITSIO
         *       library API.
         */
        template <typename T>
        struct traits
        {
            /// CFITSIO value corresponding to FITS @c BITPIX keyword,
            /// i.e. the type of data written to a FITS file.
            static constexpr int bitpix = 0;

            /// CFITSIO value corresponding to the data array type in
            /// memory being written.
            /**
             * If this type differs from the type stored in the FITS
             * file as specified by the @c BITPIX keyword, CFITSIO
             * will perform automatic data conversion from as the data
             * array is written to the FITS file.
             */
            static constexpr int datatype = 0;

            /// @c true if the @c BLANK keyword is supported for the
            /// given type.  @c false otherwise.
            /**
             * The FITS @c BLANK keyword is only supported for integer
             * types.
             */
            static constexpr bool constsupports_blank_keyword = false;
        };

        template <>
        struct traits<byte_type>
        {
            static constexpr int  bitpix                 = BYTE_IMG;
            static constexpr int  datatype               = TBYTE;
            static constexpr bool supports_blank_keyword = true;
        };

        template <>
        struct traits<short_type>
        {
            static constexpr int  bitpix                 = SHORT_IMG;
            static constexpr int  datatype               = TSHORT;
            static constexpr bool supports_blank_keyword = true;
        };

        template <>
        struct traits<long_type>
        {
            static constexpr int  bitpix                 = LONG_IMG;
            static constexpr int  datatype               = TLONG;
            static constexpr bool supports_blank_keyword = true;
        };

        template <>
        struct traits<float>
        {
            static constexpr int  bitpix                 = FLOAT_IMG;
            static constexpr int  datatype               = TFLOAT;
            static constexpr bool supports_blank_keyword = false;
        };

        template <>
        struct traits<double>
        {
            static constexpr int  bitpix                 = DOUBLE_IMG;
            static constexpr int  datatype               = TDOUBLE;
            static constexpr bool supports_blank_keyword = false;
        };

    }  // FITS
}  // MaRC


#endif  /* MARC_FITS_TRAITS_H */
