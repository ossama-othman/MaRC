// -*- C++ -*-
/**
 * @file utility.h
 *
 * %MaRC utility functions.
 *
 * Copyright (C) 2017, 2018, 2022  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_UTILITY_H
#define MARC_UTILITY_H

#include <iterator>
#include <algorithm>
#include <stdexcept>


namespace MaRC
{
    /**
     * @brief Invert image samples (columns).
     *
     * Invert samples (columns) in an image so that the image is
     * reflected from left to right about the center column.
     *
     * @param[in,out] image   Image to be inverted from left to
     *                        right.  This should be an array or
     *                        container.
     * @param[in]     samples Number of columns in the @a image.
     * @param[in]     lines   Number of rows    in the @a image.
     *
     * @throw std::invalid_argument Image size does not match number
     *                              of samples and lines.
     */
    template <typename T>
    void
    invert_samples(T & image,
                   std::size_t samples,
                   std::size_t lines)
    {
        if (std::size(image) != samples * lines)
            throw std::invalid_argument("Image size does not match "
                                        "number of samples and lines.");

        auto begin = std::begin(image);
        for (std::size_t line = 0; line < lines; ++line) {
            auto const end = begin + samples;
            std::reverse(begin, end);
            begin = end;
        }
    }

    /**
     * @brief Invert image lines (rows).
     *
     * Invert lines (rows) in an image so that the image is reflected
     * from top to bottom about the center row.
     *
     * @param[in,out] image   Image to be inverted from top to
     *                        bottom.  This should be an array or
     *                        container.
     * @param[in]     samples Number of columns in the @a image.
     * @param[in]     lines   Number of rows    in the @a image.
     *
     * @throw std::invalid_argument Image size does not match number
     *                              of samples and lines.
     */
    template <typename T>
    void
    invert_lines(T & image,
                 std::size_t samples,
                 std::size_t lines)
    {
        if (std::size(image) != samples * lines)
            throw std::invalid_argument("Image size does not match "
                                        "number of samples and lines.");

        std::size_t const middle = lines / 2;

        for (std::size_t line = 0; line < middle; ++line) {
            // Line from one end.
            auto const top_begin = std::begin(image) + line * samples;
            auto const top_end   = top_begin + samples;

            // Line from the other end.
            std::size_t const offset = (lines - line - 1) * samples;
            auto const bottom_begin = std::begin(image) + offset;

            // Swap the lines.
            std::swap_ranges(top_begin, top_end, bottom_begin);
        }
    }
}


#endif  /* MARC_UTILITY_H */
