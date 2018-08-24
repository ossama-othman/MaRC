// -*- C++ -*-
/**
 * @file SourceImage.h
 *
 * Copyright (C) 1999, 2003-2004, 2017-2018  Ossama Othman
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
//==========================================================================

#ifndef MARC_SOURCE_IMAGE_H
#define MARC_SOURCE_IMAGE_H

#include <marc/Export.h>
#include <marc/details/scale_and_offset.h>

#include <cstddef>


namespace MaRC
{
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
     * floating point to integer.  To reduce the loss of significant
     * digits in such cases, the data should be scaled upward and
     * potentially offset from their original values so that more
     * significant digits end up to the left of the decimal point
     * prior to assignment to integer typed map array elements.
     *
     * For example, cosine values to be stored in a 16 bit signed
     * integer map could be scaled by 10000 with a zero offset to
     * increase the number of significant digits in the map data from
     * one to four, e.g. 0.1234567 becomes 1234.567, which ends up
     * being stored as 1234 in a 16 bit signed integer map.  A scale
     * factor of 10000 in this case is suitable since the scaled data
     * range, -10000 to 10000, never exceeds the 16 bit signed integer
     * data range, i.e -32768 to 32767.  The chosen scale order of
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
     *            values that allow data to fit within the map type
     *            @c T data range without decreasing the order of
     *            magnitude of the data, Otherwise a complete loss of
     *            significant digits would occur since they'd all be
     *            to the right of the decimal point, and ultimately
     *            truncated when assigned to an integer.  In
     *            particular, the scale value will always be greater
     *            than or equal to one if this function completes
     *            successfully.
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
     *               scale data without complete loss of significant
     *               digits when assigned to an integer due to
     *               trunctation, such as when a scale factor less
     *               than 1 would be required.
     */
    template <typename T>
    bool scale_and_offset(double min,
                          double max,
                          double & scale,
                          double & offset)
    {
        return details::scale_and_offset<T>()(min, max, scale, offset);
    }

    // --------------------------------------------------------------

    /**
     * @class SourceImage
     *
     * @brief Abstract base class for all images to be mapped.
     *
     * Concrete source image classes must implement the interface
     * required by this abstract base class.
     */
    class MARC_API SourceImage
    {
    public:

        SourceImage() = default;

        // Disallow copying.
        SourceImage(SourceImage const &) = delete;
        SourceImage & operator=(SourceImage const &) = delete;

        /// Destructor.
        virtual ~SourceImage() = default;

        /**
         * @brief Retrieve physical data from source image.
         *
         * @param[in]  lat  Planetocentric latitude in radians.
         * @param[in]  lon  Longitude in radians.
         * @param[out] data Physical data retrieved from image.
         *
         * @retval true  Physical data retrieved.
         * @retval false No physical data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const = 0;

        /// Retrieve data and weight from source image.
        /**
         * Retrieve physical data and weight from source image.  The
         * default implementation merely ignores the @a weight and
         * @a scan arguments, and forwards the call to the concrete
         * implementation of @c read_data().  Subclasses should
         * override this method if they will provide a @a weight along
         * with @a data.
         *
         * @param[in]     lat    Planetocentric latitude in radians.
         * @param[in]     lon    Longitude in radians.
         * @param[out]    data   Physical data retrieved from image.
         * @param[in,out] weight Distance from pixel to closest edge
         *                       or blank pixel.
         * @param[in]     scan   Flag that determines if a data weight
         *                       scan is performed..
         *
         * @retval true  Physical data retrieved.
         * @retval false No physical data retrieved.
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data,
                               std::size_t & weight,
                               bool scan) const;

        /**
         * @brief Unit of physical data.
         *
         * Get the units of the physical data, i.e. image data that
         * has been scaled and offset according to the following
         * equation:
         *
         * @code
         *     physical_data = image_data * scale + offset;
         * @endcode
         *
         * The unit should conform to IAU Style Manual
         * recommendations.
         *
         * @see https://www.iau.org/publications/proceedings_rules/units/
         *
         * @return Unit of physical data in the source image.
         */
        virtual char const * unit() const { return ""; }

    };

} // End MaRC namespace


#endif  /* MARC_SOURCE_IMAGE_H */
