/**
 * @file FITS_image_t.cpp
 *
 * Copyright (C) 2017-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_FITS_IMAGE_T_CPP
#define MARC_FITS_IMAGE_T_CPP

#include "FITS_traits.h"

#include <marc/Log.h>
#include <marc/utility.h>

#include <fitsio.h>

#include <array>  // For std::size().
#include <cmath>


template <typename T>
void
MaRC::FITS::image::update_fits_key(fitsfile * fptr,
                                   char const * key,
                                   T value,
                                   char const * comment)
{
    int status = 0;

    fits_update_key(fptr,
                    FITS::traits<T>::datatype,
                    key,
                    &value,
                    comment,
                    &status);

    MaRC::FITS::throw_on_error(status);
}

template <typename T>
void
MaRC::FITS::image::blank(blank_type blank)
{
     // Write the BLANK keyword and value into the map FITS file.
    if (std::is_integral<T>() && blank) {
        T blank_value = *blank;

        /**
         * @todo Should we bother checking if the blank value fits
         *       within the range of type @c T.  The
         *       @c MaRC::MapFactory::make_map() call already does
         *       that.
         */
        this->template update_fits_key<T>(
            this->fptr_.get(),
            "BLANK",
            blank_value,
            "value of pixels with undefined physical value");
    }
}

template <typename T>
void
MaRC::FITS::image::datamin(T min)
{
    if (!std::isnan(min))
        this->template update_fits_key<T>(
            this->fptr_.get(),
            "DATAMIN",
            min,
            "minimum valid physical data value");
}

template <typename T>
void
MaRC::FITS::image::datamax(T max)
{
    if (!std::isnan(max))
        this->template update_fits_key<T>(
            this->fptr_.get(),
            "DATAMAX",
            max,
            "maximum valid physical data value");
}

template <typename T>
bool
MaRC::FITS::image::write(T const & img)
{
    if (this->fpixel_>= this->max_elements_) {
        MaRC::error("FITS image array is already fully written.");

        return false;
    } else if(static_cast<LONGLONG>(std::size(img))
              != this->nelements_) {
        MaRC::error("FITS image and data array sizes, "
                    "{} and {}, do not match.",
                    this->nelements_, std::size(img));

        return false;
    }

    int status = 0;

    /*
      First pixel/element in FITS array (1-based).
         Plane 1: fpixel  = 1
         Plane 2: fpixel += nelements
         Plane 3: fpixel += nelements
         Plane 4: ... etc ...
    */

    auto data = std::data(img);

    /*
      CFITSIO expects the data to passed as a non-const pointer to
      void.  Since this method template supports both containers with
      an underlying contiguous block of memory (e.g. std::vector<>)
      and arrays, we go through hoops to go from "std::vector<T>
      const" or "T const *" to "T", for example.
    */
    using data_type =
        typename std::remove_const<
            typename std::remove_pointer<decltype(data)>::type>::type;

    fits_write_img(this->fptr_.get(),
                   FITS::traits<data_type>::datatype,
                   this->fpixel_,
                   this->nelements_,
                   const_cast<data_type *>(data),
                   &status);

    if (status != 0) {
        char err[FLEN_STATUS] = { 0 };
        fits_get_errstatus(status, err);

        MaRC::error(err);

        return false;
    }

    // Set offset in the FITS array to the next plane.
    this->fpixel_ += this->nelements_;

    return true;
}


#endif  /* MARC_FITS_IMAGE_T_CPP */
