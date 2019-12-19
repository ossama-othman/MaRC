// -*- C++ -*-
/**
 * @file SourceImageFactory.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_SOURCE_IMAGE_FACTORY_H
#define MARC_SOURCE_IMAGE_FACTORY_H

#include <memory>
#include <functional>


namespace MaRC
{
    class SourceImage;

    /**
     * @class SourceImageFactory
     *
     * @brief Abstract factory class containing interface for image
     *        factories.
     *
     * @note An @c ImageFactory can be considered a map plane factory as
     *       well.
     */
    class SourceImageFactory
    {
    public:

        /**
         * Type of functor used for determining scale and offset
         * appropriate for a map of given data type.
         *
         * @see MaRC::scale_and_offset()
         */
        using scale_offset_functor =
            std::function<bool(double, double, double &, double &)>;

        /// Constructor.
        SourceImageFactory();

        // Disallow copying.
        SourceImageFactory(SourceImageFactory const &) = delete;
        SourceImageFactory & operator=(
            SourceImageFactory const &) = delete;

        /// Destructor.
        virtual ~SourceImageFactory() = default;

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

        /// Set minimum allowed physical data value in map plane.
        /**
         * Set the minimum allowed physical data value, i.e.
         * data >= minimum, in the map plane to which an image will be
         * mapped.
         *
         * @throw std::invalid_argument Supplied minimum is either
         *                              not-a-number (NaN) or greater
         *                              than the maximum.
         */
        void minimum(double m);

        /// Set maximum allowed physical data value in map plane.
        /**
         * Set the maximum allowed physical data value, i.e.
         * data =< maximum, in the map plane to which an image will be
         * mapped.
         *
         * @throw std::invalid_argument Supplied maximum is either
         *                              not-a-number (NaN) or less
         *                              than the minimum.
         */
        void maximum(double m);

        /// Return minimum allowed physical data value in map plane.
        double minimum() const { return this->minimum_; }

        /// Return maximum allowed physical data value in map plane.
        double maximum() const { return this->maximum_; }

    private:

        /**
         * @brief Minimum allowed physical data value in map plane
         *        (data >= minimum).
         */
        double minimum_;

        /**
         * @brief Maximum allowed physical data value in map plane
         *        (data =< maximum)
         */
        double maximum_;

    };

}


#endif  /* MARC_SOURCE_IMAGE_FACTORY_H */
