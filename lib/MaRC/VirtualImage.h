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
#include <MaRC/Export.h>

#include <limits>
#include <cmath>


namespace MaRC
{
    /**
     * @brief Integer data scale and offset calculation functor.
     *
     * Determine the best scale and offset parameters applied to
     * floating point data read from source images to retain as many
     * significant digits as possible when storing that data in
     * integer typed maps.
     *
     * @see MaRC::scale_and_offset()
     *
     * @internal This functor is not part of the public MaRC library
     *           API.
     */
    template <typename T>
    struct scale_and_offset_impl
    {
        bool operator()(double min,
                        double max,
                        double & scale,
                        double & offset) const
        {
            constexpr auto T_lowest = std::numeric_limits<T>::lowest();
            constexpr auto T_max    = std::numeric_limits<T>::max();

            static_assert(
                T_lowest >= std::numeric_limits<double>::lowest() / 2
                && T_max <= std::numeric_limits<double>::max() / 2,
                "Integer type is too large for scale/offset calculation");

            /*
              Avoid integer overflow by performing a floating point
              subtraction.  No overflow will occur since we already
              know:
                T_max - T_lowest < std::numeric_limits<double>::max()
            */
            constexpr double type_range =
                static_cast<double>(T_max) - T_lowest;

            double const data_range = max - min;

            if (!std::isfinite(data_range)
                || data_range < 0
                || data_range > type_range) {
                // data_range is not a finite value (e.g. overflowed)
                // or min > max
                // or can't fit all data into desired type T
                return false;
            }

            int const exponent =
                static_cast<int>(std::numeric_limits<T>::digits10)
                - static_cast<int>(std::log10(data_range));

            scale = std::pow(10, exponent);

            if (min * scale < T_lowest)
                offset = data_range / 2 * scale;
            else if (max * scale > T_max)
                offset = -data_range / 2 * scale;
            else
                offset = 0;

            return true;
        }
    };

    /**
     * @brief @c float typed data scale and offset calculation
     *        functor.
     *
     * Automatic source data scaling is not performed when mapping to
     * floating point typed maps.  This implementation is basically a
     * no-op, and returns @a scale and @a offset values that result in
     * the source data remaining unchanged.
     *
     * @see MaRC::scale_and_offset()
     *
     * @internal This functor is not part of public MaRC library API.
     */
    template <>
    struct scale_and_offset_impl<float>
    {
        bool operator()(double /* min */,
                        double /* max */,
                        double & scale,
                        double & offset) const
        {
            // No auto-scaling for floating point map data.
            scale  = 1;
            offset = 0;

            return true;
        }
    };

    /**
     * @brief @c double typed data scale and offset calculation
     *        functor.
     *
     * Automatic source data scaling is not performed when mapping to
     * floating point typed maps.  This implementation is basically a
     * no-op, and returns @a scale and @a offset values that result in
     * the source data remaining unchanged.
     *
     * @see MaRC::scale_and_offset()
     *
     * @internal This functor is not part of public MaRC library API.
     */
    template <>
    struct scale_and_offset_impl<double>
    {
        bool operator()(double /* min */,
                        double /* max */,
                        double & scale,
                        double & offset) const
        {
            // No auto-scaling for floating point map data.
            scale  = 1;
            offset = 0;

            return true;
        }
    };

    /**
     * @brief Determine suitable data scale and offset values.
     *
     * Determine the best scale and offset parameters applied to
     * floating point data read from source images to retain as many
     * significant digits as possible when storing that data in
     * integer typed maps.
     *
     * Some source images, e.g. @c MaRC::VirtualImage subclasses, only
     * provide floating point numbers.  That is a problem when storing
     * those numbers in integer typed maps since significant digits
     * after the decimal point could be truncated when casting from
     * floating point to integer.  To prevent loss of significant
     * digits in such cases, the data should be scaled upward and
     * potentially offset from their original values so that more
     * significant digits end up to the left decimal point prior to
     * assignment to elements in integer typed maps.
     *
     * For example, cosine values to be stored in a 16 bit signed
     * integer map could be scaled by 10000 with a zero offset to
     * increase the number of significant digits in the map data from
     * one to four, e.g. 0.1234567 becomes 1234.567, which ends being
     * stored as 1234 in a 16 bit signed integer map.  A scale factor
     * of 10000 in this case is suitable since the scaled data range,
     * -10000 to 10000, never exceeds the 16 bit signed integer data
     * range, i.e -32768 to 32767.  The chosen scale order of
     * magnitude is the largest it can be without causing transformed
     * data to exceed the map data range.
     *
     * To reduce potential confusion about what the data actually is,
     * only power of 10 scale factors (i.e. 1, 10, 100, etc) are
     * chosen.  Unless the data minimum (@a min) and maximum (@a max)
     * are not symmetrical and/or the map data type is unsigned, the
     * offset value will generally be zero.
     *
     * The physical data should be transformed according to the
     * following equation prior to mapping:
     *
     * @code
     * map_data = scale * physical_data + offset
     * @endcode
     *
     * Retrieving the original physical data from the map would then
     * require the following equation:
     *
     * @code
     * physical_data = (map_data - offset) / scale
     * @endcode
     *
     * @note The @a scale and @a offset will always be 1 and 0,
     *       respectively, if the map data type is a floating point
     *       type, i.e. @c float or @c double.
     *
     * @attention This function only generates @a scale and @a offset
     *            values that allow data to fit within the map data
     *            range without decreasing the order of magnitude of
     *            the data, resulting in loss of significant digits.
     *            In particular, the scale value will always be
     *            greater than or equal to one if this function
     *            completes successfully.
     *
     * @param[in]  min    The lowest physical value to be plotted on a
     *                    map.  For example, this would be -1 for
     *                    source images that generate cosines.
     * @param[in]  max    The highest physical value to be plotted on
     *                    a map.  For example, this would be 1 for
     *                    source images that generate cosines.
     * @param[out] scale  Linear scaling value by which physical data
     *                    should be multiplied to maximize the number
     *                    of significant digits prior to storing data
     *                    in an integer typed map. This will be 1 for
     *                    floating point typed maps.
     * @param[out] offset Offset value to be added to data after the
     *                    scaling factor has been applied to force
     *                    that data to fit within the integer typed
     *                    map data range.  This will be 0 for floating
     *                    point typed maps.
     *
     * @retval true  Suitable @a scale and @a offset values were
     *               found.
     * @retval false Suitable @a scale and @a offset values were not
     *               found.  This can occur if it isn't possible to
     *               scale data without loss of significant digits,
     *               such as when a scale factor less than 1 would be
     *               required.
     */
    template <typename T>
    bool scale_and_offset(double min,
                          double max,
                          double & scale,
                          double & offset)
    {
        return scale_and_offset_impl<T>()(min, max, scale, offset);
    }

    // --------------------------------------------------------------

    /**
     * @class VirtualImage
     *
     * @brief Base class for virtual source images.
     *
     * Data from virtual images are computed at run-time rather than
     * retrieved from static sources such as images stored on a
     * filesystem.
     */
    class MARC_API VirtualImage : public SourceImage
    {
    public:

        /// Constructor.
        /**
         * @param[in] s Linear scaling coefficient applied to computed
         *              data.
         * @param[in] o Linear offset value applied to all (scaled)
         *              computed data.
         */
        VirtualImage(double s = 1, double o = 0);

        /// Retrieve data from virtual image.
        /**
         * Retrieve data from virtual image and apply configured data
         * transformations, if any.  Raw data is computed/retrieved
         * from the @c read_data_i() template method.
         *
         * @param[in]  lat  Bodycentric (e.g. planetocentric) latitude
         *                  in radians.
         * @param[in]  lon  Longitude in radians.
         *
         * @param[out] data Data retrieved from image.
         *
         * @retval true  Data retrieved
         * @retval false No data retrieved.
         *
         * @see read_data_i()
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const;

        /**
         * @name Linear Data Transformation
         *
         * These methods return the scale and offset values that
         * should be used when linearly transforming data read from a
         * @ c VirtualImage and plotted on a map to the true physical
         * values.  In particular, data should be transformed
         * according to the following equation:
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
         * Offset value that should be applied to all (scaled)
         * computed data.  This value corresponds to zero in the
         * virtual image.
         *
         * @return Data offset.
         */
        double offset() const { return this->scale() * -this->offset_; }
        //@}

        /**
         * @brief Physical unit of data after application of scale and
         *        offset.
         *
         * Get the physical units of the data after transforming the
         * virtual image data with the values returned from @c scale()
         * and @c offset().  The unit should conform to IAU Style Manual
         * recommendations.
         *
         * @see @c VirtualImage::scale()
         * @see @c VirtualImage::offset()
         * @see https://www.iau.org/publications/proceedings_rules/units/
         *
         * @return Physical unit of data in the virtual image.
         */
        virtual char const * unit() const { return ""; }

    private:

        /// Compute data specific to a given virtual image.
        /**
         * This template method is the core implementation of the
         * @c read_data() method.
         *
         * @param[in]  lat  Bodycentric (e.g. planetocentric) latitude
         *                  in radians.
         * @param[in]  lon  Longitude in radians.
         *
         * @param[out] data Data retrieved from image.
         *
         * @retval true  Data retrieved
         * @retval false No data retrieved.
         *
         * @see read_data()
         */
        virtual bool read_data_i(double lat,
                                 double lon,
                                 double & data) const = 0;

    private:

        /// Linear scaling coefficient applied to physical data.
        /**
         * Linear scaling coefficient applied to physical data to
         * allow data to fit in map array element of specific type
         * with the most amount of significant digits.
         *
         * @note This is inverse of the of the scaling coefficient
         *       needed to retrieve the true physical value.
         *
         * @see @c VirtualImage::scale()
         */
        double const scale_;

        /// Offset value applied to scaled physical data.
        /**
         * This offset is applied to scaled physical data to allow
         * data fit in a map array element of specific type with the
         * most amount of significant digits.
         *
         * @note This is the negative offset multiplied by the inverse
         *       of the of scaling value needed to retrieve the true
         *       physical value.
         *
         * @see @c VirtualImage::offset()
         */
        double const offset_;

    };

} // End MaRC namespace


#endif  /* MARC_VIRTUAL_IMAGE_H */
