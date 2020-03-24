// -*- C++ -*-
/**
 * @file SourceImageFactory.h
 *
 * Copyright (C) 2004, 2017, 2019-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_SOURCE_IMAGE_FACTORY_H
#define MARC_SOURCE_IMAGE_FACTORY_H

#include <memory>
#include <functional>

#include <marc/extrema.h>


namespace MaRC
{
    class SourceImage;
    class MapParameters;

    /**
     * @class SourceImageFactory
     *
     * @brief Abstract factory class containing interface for source
     *        image factories.
     *
     * @note An @c SourceImageFactory can be considered a map plane
     *       factory as well.
     */
    class SourceImageFactory
    {
    public:

        /**
         * @brief Convenience type alias for physical data extrema.
         *
         * @bug A @c double typed extremum is not appropriate for 64
         *      bit integer values more than 53 bits wide, i.e. the
         *      width of the significand in 64 bit IEEE 754 floating
         *      point values.
         */
        using extrema_type = extrema<double>;

        /**
         * Type of functor used for determining scale and offset
         * appropriate for a map of given data type.
         *
         * @see MaRC::scale_and_offset()
         */
        using scale_offset_functor =
            std::function<bool(double, double, double &, double &)>;

        /// Constructor.
        SourceImageFactory() = default;

        // Disallow copying.
        SourceImageFactory(SourceImageFactory const &) = delete;
        SourceImageFactory & operator=(
            SourceImageFactory const &) = delete;

        /// Destructor.
        virtual ~SourceImageFactory() = default;

        /**
         * @brief Populate map parameters.
         *
         * Set map parameters based on @c SourceImage
         * characteristics.
         *
         * @param[in,out] parameters Map parameters to be populated.
         *
         * @return @c true if parameters were successfully populated,
         *         @c false otherwise.
         */
        virtual bool populate_parameters(
            MapParameters & parameters) const = 0;

        /// Create a @c SourceImage for a map of given data type.
        /**
         * Create a @c SourceImage for a map of given data type.
         *
         * @param[in] calc_so Functor used for determining scale and
         *                    offset appropriate for a map of given
         *                    type.  This is implemented by
         *                    @c MaRC::scale_and_offset() but is
         *                    passed in as a @c std::function to
         *                    prevent @c ImageFactory from having a
         *                    compile-time dependency on the map data
         *                    type.
         *
         * @return @c SourceImage from which map data will be
         *         sourced.
         */
        virtual std::unique_ptr<SourceImage> make(
            scale_offset_functor calc_so) = 0;

        /**
         * @brief Set the minimum physical data value.
         *
         * Set the minimum physical data value if it hasn't already
         * been set.
         *
         * @param[in] datum Physical data value.
         */
        void minimum(double datum);

        /**
         * @brief Set the maximum physical data value.
         *
         * Set the maximum physical data value if it hasn't already
         * been set.
         *
         * @param[in] datum Physical data value.
         */
        void maximum(double datum);

        /**
         * @brief Get the minimum and maximum physical data values.
         *
         * @note Only a @c const reference is accessible to prevent
         *       @c SourceImageFactory subclasses from potentially
         *       overriding previously set extrema, in particular
         *       those specified by the user in a %MaRC configuration
         *       or input file.
         */
        extrema_type const & minmax() const { return this->extrema_; }

    private:

        /**
         * @brief Minimum and maximum physical data values.
         *
         * Minimum and maximum physical data values in the
         * source image. (minimum <= data <= maximum).
         *
         * @note Both, one, or none of the extrema may be set.
         *
         * @note This member is private to prevent
         *       @c SourceImageFactory subclasses from potentially
         *       overriding previously set extrema, in particular
         *       those specified by the user in a %MaRC configuration
         *       or input file.
         */
        extrema_type extrema_;

    };

}


#endif  /* MARC_SOURCE_IMAGE_FACTORY_H */
