// -*- C++ -*-

//==========================================================================
/**
 *  @file FITS_traits.h
 *
 *  $Id: FITS_traits.h,v 1.2 2004/10/11 06:20:53 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================


#ifndef MARC_FITS_TRAITS_H
#define MARC_FITS_TRAITS_H

#include "MaRC/config.h"
#include "MaRC/Long.h"

#include <fitsio.h>


namespace MaRC
{
  /**
   * @struct FITS_traits
   *
   * @brief Traits class for type-specific FITS parameters.
   *
   * @note The traits in this class are specific to the CFITSIO
   *       library API.
   */
  template <typename T>
  struct FITS_traits
  {
    /// CFITSIO value corresponding to FITS @c BITPIX keyword,
    /// i.e. the type of data written to a FITS file.
    static const int bitpix = 0;

    /// CFITSIO value corresponding to the data array type in memory
    /// being written.
    /**
     * If this type differs from the type stored in the FITS file as
     * specified by the @c BITPIX keyword, CFITSIO will perform
     * automatic data conversion from as the data array is written to
     * the FITS file.
     */
    static const int datatype = 0;

    /// @c true if the @c BLANK keyword is supported for the given
    /// type.  @c false otherwise.
    /**
     * The FITS @c BLANK keyword is only supported for integer types.
     */
    static const bool supports_blank_keyword = false;

    /// Create FITS image array.
    /**
     * @param fptr        Pointer to fitsfile structure corresponding
     *                    to FITS file where image will be written.
     * @param naxis       Number of dimensions in FITS image array.
     * @param naxes       Array containing size of each FITS image
     *                    array dimension.
     * @param status      CFITSIO status variable/holder.
     */
    static void create_img (fitsfile * fptr,
                            int naxis,
                            long * naxes,
                            int & status)
    {
      fits_create_img (fptr,
                       FITS_traits<T>::bitpix,
                       naxis,
                       naxes,
                       &status);
    }

    /// Write image array to FITS file.
    /**
     * @param fptr        Pointer to fitsfile structure corresponding
     *                    to FITS file where image will be written.
     * @param first_pixel First pixel image to write (1-based).
     * @param nelements   Number of elements in the image array.
     * @param img         Image array to be written to FITS file.
     * @param status      CFITSIO status variable/holder.
     */
    static void write_img (fitsfile * fptr,
                           long first_pixel,
                           unsigned int nelements,
                           T * img,
                           int & status)
    {
      fits_write_img (fptr,
                      FITS_traits<T>::datatype,
                      first_pixel,
                      static_cast<long> (nelements),
                      img,
                      &status);
    }
  };

  template <>
  struct FITS_traits<unsigned char>
  {
    static const int  bitpix                 = BYTE_IMG;
    static const int  datatype               = TBYTE;
    static const bool supports_blank_keyword = true;

    static void create_img (fitsfile * fptr,
                            int naxis,
                            long * naxes,
                            int & status)
    {
      fits_create_img (fptr,
                       FITS_traits<unsigned char>::bitpix,
                       naxis,
                       naxes,
                       &status);
    }

    static void write_img (fitsfile * fptr,
                           long first_pixel,
                           unsigned int nelements,
                           unsigned char * img,
                           int & status)
    {
      fits_write_img (fptr,
                      FITS_traits<unsigned char>::datatype,
                      first_pixel,
                      static_cast<long> (nelements),
                      img,
                      &status);
    }
  };

  template <>
  struct FITS_traits<short>
  {
    static const int  bitpix                 = SHORT_IMG;
    static const int  datatype               = TSHORT;
    static const bool supports_blank_keyword = true;

    static void create_img (fitsfile * fptr,
                            int naxis,
                            long * naxes,
                            int & status)
    {
      fits_create_img (fptr,
                       FITS_traits<short>::bitpix,
                       naxis,
                       naxes,
                       &status);
    }

    static void write_img (fitsfile * fptr,
                           long first_pixel,
                           unsigned int nelements,
                           short * img,
                           int & status)
    {
      fits_write_img (fptr,
                      FITS_traits<short>::datatype,
                      first_pixel,
                      static_cast<long> (nelements),
                      img,
                      &status);
    }
  };

  template <>
  struct FITS_traits<MaRC::Long>
  {
    static const int  bitpix                 = LONG_IMG;
    static const int  datatype               = TLONG;
    static const bool supports_blank_keyword = true;

    static void create_img (fitsfile * fptr,
                            int naxis,
                            long * naxes,
                            int & status)
    {
      fits_create_img (fptr,
                       FITS_traits<MaRC::Long>::bitpix,
                       naxis,
                       naxes,
                       &status);
    }

    static void write_img (fitsfile * fptr,
                           long first_pixel,
                           unsigned int nelements,
                           MaRC::Long * img,
                           int & status)
    {
      fits_write_img (fptr,
                      FITS_traits<MaRC::Long>::datatype,
                      first_pixel,
                      static_cast<long> (nelements),
                      img,
                      &status);
    }
  };

  template <>
  struct FITS_traits<float>
  {
    static const int  bitpix                 = FLOAT_IMG;
    static const int  datatype               = TFLOAT;
    static const bool supports_blank_keyword = false;

    static void create_img (fitsfile * fptr,
                            int naxis,
                            long * naxes,
                            int & status)
    {
      fits_create_img (fptr,
                       FITS_traits<float>::bitpix,
                       naxis,
                       naxes,
                       &status);
    }

    static void write_img (fitsfile * fptr,
                           long first_pixel,
                           unsigned int nelements,
                           float * img,
                           int & status)
    {
      fits_write_img (fptr,
                      FITS_traits<float>::datatype,
                      first_pixel,
                      static_cast<long> (nelements),
                      img,
                      &status);
    }
  };

  template <>
  struct FITS_traits<double>
  {
    static const int  bitpix                 = DOUBLE_IMG;
    static const int  datatype               = TDOUBLE;
    static const bool supports_blank_keyword = false;

    static void create_img (fitsfile * fptr,
                            int naxis,
                            long * naxes,
                            int & status)
    {
      fits_create_img (fptr,
                       FITS_traits<double>::bitpix,
                       naxis,
                       naxes,
                       &status);
    }

    static void write_img (fitsfile * fptr,
                           long first_pixel,
                           unsigned int nelements,
                           double * img,
                           int & status)
    {
      fits_write_img (fptr,
                      FITS_traits<double>::datatype,
                      first_pixel,
                      static_cast<long> (nelements),
                      img,
                      &status);
    }
  };

}

#endif  /* MARC_FITS_TRAITS_H */
