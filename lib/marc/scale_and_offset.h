// -*- C++ -*-
/**
 * @file marc/scale_and_offset.h
 *
 * Copyright (C) 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_SCALE_AND_OFFSET_H
#define MARC_SCALE_AND_OFFSET_H

#include <marc/details/scale_and_offset.h>


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
     *            @a T data range without decreasing the order of
     *            magnitude of the data, Otherwise a complete loss of
     *            significant digits would occur since they'd all be
     *            to the right of the decimal point, and ultimately
     *            truncated when assigned to an integer.  In
     *            particular, the scale value will always be greater
     *            than or equal to one if this function completes
     *            successfully.
     *
     * @tparam     T      Destination data type.
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

}


#endif  // MARC_SCALE_AND_OFFSET_H
