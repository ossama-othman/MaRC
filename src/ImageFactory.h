// -*- C++ -*-
/**
 * @file ImageFactory.h
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#ifndef MARC_IMAGE_FACTORY_H
#define MARC_IMAGE_FACTORY_H

#include <memory>
#include <functional>


namespace MaRC
{
    class SourceImage;

    /**
     * @class ImageFactory
     *
     * @brief Abstract factory class containing interface for image
     *        factories.
     *
     * @note An @c ImageFactory can be considered a map plane factory as
     *       well.
     */
    class ImageFactory
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
        ImageFactory();

        // Disallow copying.
        ImageFactory(ImageFactory const &) = delete;
        ImageFactory & operator=(ImageFactory const &) = delete;

        /// Destructor.
        virtual ~ImageFactory();

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

        /// Set minimum allowed data value in map plane.
        /**
         * Set the minimum allowed data value, i.e. data >= minimum,
         * in the map plane to which an image will be mapped.
         */
        void minimum(double m) { this->minimum_ = m; }

        /// Set maximum allowed data value in map plane.
        /**
         * Set the maximum allowed data value, i.e. data =< maximum,
         * in the map plane to which an image will be mapped.
         */
        void maximum(double m) { this->maximum_ = m; }

        /// Return minimum allowed data value in map plane.
        double minimum() const { return this->minimum_; }

        /// Return maximum allowed data value in map plane.
        double maximum() const { return this->maximum_; }

    private:

        /// Minimum allowed data value in map plane.  (data >= minimum)
        double minimum_;

        /// Maximum allowed data value in map plane.  (data =< maximum)
        double maximum_;

    };

}


#endif  /* MARC_IMAGE_FACTORY_H */
