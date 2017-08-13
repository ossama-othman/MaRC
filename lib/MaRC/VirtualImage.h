// -*- C++ -*-
/**
 * @file VirtualImage.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#ifndef MARC_VIRTUAL_IMAGE_H
#define MARC_VIRTUAL_IMAGE_H

#include "MaRC/SourceImage.h"

#include <limits>
#include <cmath>


namespace MaRC
{
    // Only increase significant digits, not reduce!
    template <typename T>
    static bool scale_and_offset(double min,
                                 double max,
                                 double & scale,
                                 double & offset)
    {
        constexpr double T_lowest = std::numeric_limits<T>::lowest();
        constexpr double T_max    = std::numeric_limits<T>::max();
        constexpr double type_range = T_max - T_lowest;

        double const data_range = max - min;

        if (data_range < 0 || data_range > type_range) {
            // min > max or can't fit all data into desired type T.
            return false;
        }

        int const exponent =
            static_cast<int>(std::numeric_limits<T>::digits10)
            - static_cast<int>(std::log10(data_range));

        scale = std::pow(10, exponent);

        if (min < T_lowest)
            offset = (T_lowest - min) * scale;
        else if (max > T_max)
            offset = (max - T_max) * scale;
        else
            offset = 0;

        return true;
    }


    /**
     * @class VirtualImage
     *
     * @brief Base class for virtual source images.
     *
     * Data from virtual images are computed at run-time rather than
     * retrieved from static sources such as images stored on a
     * filesystem.
     */
    class VirtualImage : public SourceImage
    {
    public:

        /// Constructor.
        /**
         * @param s Linear scaling coefficient applied to computed
         *          data.
         * @param o Linear offset value applied to all (scaled)
         *          computed data.
         */
        VirtualImage(double s = 1, double o = 0);

        /// Retrieve data from virtual image.
        /**
         * Retrieve data from virtual image and apply configured data
         * transformations, if any.  Raw data is computed/retrieved
         * from the @c read_data_i() template method.
         *
         * @param lat  Bodycentric (e.g. planetocentric) latitude in
         *             radians.
         * @param lon  Longitude in radians.
         *
         * @param[out] data Data retrieved from image.
         *
         * @return @c true - Data retrieved, @c false - No data retrieved.
         *
         * @see read_data_i()
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const;

    private:

        /// Compute data specific to a given virtual image.
        /**
         * This template method is the core implementation of the
         * read_data() method.
         *
         * @param lat  Bodycentric (e.g. planetocentric) latitude in
         *             radians.
         * @param lon  Longitude in radians.
         * @param data Data retrieved from image.
         *
         * @retval @c true   Data retrieved
         * @retval  @c false No data retrieved.
         *
         * @see read_data().
         */
        virtual bool read_data_i(double lat,
                                 double lon,
                                 double & data) const = 0;

        /// Is point at given latitude and longitude visible to the
        /// observer?
        /**
         * @param lat Bodycentric (e.g. planetocentric) latitude in
         *            radian.
         * @param lon Longitude in radians.
         *
         * @retval @c true  Point is visible.
         * @retval @c false Point is not visible.
         *
         * @note The default implementation always returns @c true.
         */
        virtual bool is_visible(double lat, double lon) const;

        // virtual double minimum() const = 0;
        // virtual double maximum() const = 0;

        /**
         * @name Linear Data Transformation
         *
         * These methods return the scale and offset values that
         * should be used when linearly transforming data read from a
         * @ c VirtualImage and plotted on a map to the true physical
         * values.  In particular, data will be transformed according
         * to the following equation:
         *
         * @code
         *     physical_value = map_value * scale() + offset();
         * @endcode
         *
         * The default implementation returns values that causes no
         * transformation to occur.
         */
        //@{
        /// Data scale.
        /**
         * Linear scaling coefficient that should be applied to map
         * data to transform that data to true physical data.
         *
         * @return Data scale.
         */
        double scale() const { return 1 / this->scale_; }

        /// Data offset
        /**
         * Offset value applied to all (scaled) computed data.
         *
         * @return Data offset
         */
        double offset() const { return scale() * (-this->offset_); }
        //@}

    private:

        /// Linear scaling coefficient applied to physical data.
        /**
         * Linear scaling coefficient applied to physical data to
         * allow it to fit in map array element of specific type with
         * the most amount of significant digits.
         *
         * @note This is inverse of the of scaling coefficient needed
         *       to retrieve the true physical value.
         */
        double const scale_;

        /// Offset value applied to scaled physical data.
        /**
         * This offset is applied to scaled physical data to allow it
         * to fit in map array element of specific type with the most
         * amount of significant digits.
         *
         * @note This is the offset multiplied by the inverse of the
         *       of scaling value needed to retrieve the true physical
         *       value.
         */
        double const offset_;

    };

} // End MaRC namespace


#endif  /* MARC_VIRTUAL_IMAGE_H */
