// -*- C++ -*-
/**
 * @file plot_info.h
 *
 * Copyright (C) 2018  Ossama Othman
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

#ifndef MARC_PLOT_INFO_H
#define MARC_PLOT_INFO_H

#include <MaRC/Export.h>
#include <MaRC/Notifier.h>
#include <MaRC/optional.h>


namespace MaRC
{
    class SourceImage;

    /**
     * @class plot_info
     *
     * @brief Map plotting information.
     *
     * Information that will be used when a plotting a map is
     * enscapsulated in this class.
     */
    class MARC_API plot_info
    {
    public:

        using notifier_type = Progress::Notifier;

        /**
         * @brief Constructor
         *
         * @param[in] source   @c SourceImage object containing the
         *                     data to be mapped.
         * @param[in] minimum  Minimum allowed value on map, i.e. all
         *                     data greater than or equal to
         *                     @a minimum.
         * @param[in] maximum  Maximum allowed value on map, i.e. all
         *                     data less than or equal to @a maximum.
         */
        plot_info(SourceImage const & source,
                  double minimum,
                  double maximum)
            : source_(source)
            , minimum_(minimum)
            , maximum_(maximum)
            , blank_()
            , notifier_()
        {
        }

        /**
         * @brief Constructor
         *
         * @param[in] source   @c SourceImage object containing the
         *                     data to be mapped.
         * @param[in] minimum  Minimum allowed value on map, i.e. all
         *                     data greater than or equal to
         *                     @a minimum.
         * @param[in] maximum  Maximum allowed value on map, i.e. all
         *                     data less than or equal to @a maximum.
         * @param[in] blank    Blank map array value for integer typed
         *                     maps.  The blank map array value
         *                     corresponds to undefined "blank"
         *                     physical values.
         */
        plot_info(SourceImage const & source,
                  double minimum,
                  double maximum,
                  MaRC::optional<int64_t> blank)
            : source_(source)
            , minimum_(minimum)
            , maximum_(maximum)
            , blank_(blank)
            , notifier_()
        {
        }

        ~plot_info() = default;
        plot_info(plot_info const &) = delete;
        void operator=(plot_info const &) = delete;

        /// Get data to be mapped.
        SourceImage const & source() const { return this->source_; }

        /// Get minimum allowed value on map.
        double minimum() const { return this->minimum_; }

        /// Get maximum allowed value on map.
        double maximum() const { return this->maximum_; }

        /// Get blank map array value.
        MaRC::optional<int64_t> const & blank() const
        {
            return this->blank_;
        }

        /**
         * @brief Get map progress notifier.
         *
         * Use this notifier to subscribe observers for map progress
         * notifications.
         *
         * @see Notifier
         */
        notifier_type & notifier() const { return this->notifier_; }

    private:

        /// @c SourceImage object containing the data to be mapped.
        SourceImage const & source_;

        /// Minimum allowed value on map, i.e. data >= @c minimum_.
        double const minimum_;

        /// Maximum allowed value on map, i.e. data <= @c maximum_.
        double const maximum_;

        /**
         * @brief  Value of pixels with undefined physical value.
         *
         * @note This value is only valid for integer typed maps.
         */
        MaRC::optional<int64_t> const blank_;

        /// Map progress notifier.
        mutable notifier_type notifier_;

    };

}  // MaRC

#endif  // MARC_PLOT_INFO_H
