// -*- C++ -*-
/**
 * @file plot_info.h
 *
 * Copyright (C) 2018-2019  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PLOT_INFO_H
#define MARC_PLOT_INFO_H

#include <marc/extrema.h>
#include <marc/Notifier.h>

#include <optional>
#include <limits>
#include <utility>
#include <cstdint>
#include <cstddef>


namespace MaRC
{
    /// Type used to store "blank" integer values.
    using blank_type = std::optional<std::intmax_t>;

    /**
     * @class plot_info plot_info.h <marc/plot_info.h>
     *
     * @brief Map plotting information.
     *
     * Information that will be used when a plotting a map is
     * enscapsulated in this class.
     */
    template <typename T>
    class plot_info
    {
    public:

        /// Convenience alias for the progress notifier type.
        using notifier_type = Progress::Notifier;

        /**
         * @brief Constructor used when no blank value is provided.
         *
         * By default, %MaRC will use @c 0 as the blank value for
         * integer typed maps, and always use %c NaN for floating
         * point typed maps.
         *
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         */
        plot_info(std::size_t samples,
                  std::size_t lines)
            : samples_(samples)
            , lines_(lines)
            , extrema_()
            , blank_()
            , notifier_()
        {
        }

        /**
         * @brief Constructor for integer blank types.
         *
         * Constructor used when supplying an integer typed @a blank
         * value.
         *
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         * @param[in] blank   Blank map array value for integer typed
         *                    maps.  The blank map array value
         *                    corresponds to undefined "blank"
         *                    physical values.
         */
        plot_info(std::size_t samples,
                  std::size_t lines,
                  blank_type blank)
            : samples_(samples)
            , lines_(lines)
            , extrema_()
            , blank_(std::move(blank))
            , notifier_()
        {
        }

        /**
         * @brief Constructor for floating point blank types.
         *
         * Constructor used when supplying a floating point typed
         * @a blank value.  In this case, the floating point value is
         * actually ignored since the @c NaN constant is expected to
         * be used as the blank value in %MaRC generated floating
         * point typed map projections.
         *
         * This constructor exists to prevent implicit conversions
         * from a floating point blank value to the integer based
         * @c blank_type used by this class.  For example, this
         * constructor prevents attempts to assign
         * std::numeric_limits<float>::lowest() as the blank
         * value since that would result in an overflow.
         *
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         */
        plot_info(std::size_t samples,
                  std::size_t lines,
                  double /* unused */)
            : plot_info(samples, lines)
        {
        }

        /// Get the number of samples (columns in the map).
        auto samples() const { return this->samples_; }

        /// Get the number of lines (rows in the map).
        auto lines() const { return this->lines_; }

        /// Get minimum mapped physical data value.
        auto minimum() const { return this->extrema_.minimum(); }

        /// Get maximum mapped physical data value.
        auto maximum() const { return this->extrema_.maximum(); }

        /**
         * @brief Update mapped physical data value extrema.
         *
         * @param[in] datum Physical data value.
         */
        void update_extrema(T datum) { this->extrema_.update(datum); }

        // Was data plotted to the map?
        bool data_mapped() const { return this->extrema_.is_valid(); }

        /// Get blank map array value.
        auto const & blank() const { return this->blank_; }

        /**
         * @brief Get map progress notifier.
         *
         * Use this notifier to subscribe observers for map progress
         * notifications.
         *
         * @see Notifier
         */
        auto & notifier() const { return this->notifier_; }

    private:

        /// Number of samples (columns) in map.
        std::size_t const samples_;

        /// Number of lines (rows) in map.
        std::size_t const lines_;

        /// Minimum and maximum values of mapped physical data.
        extrema<T> extrema_;

        /**
         * @brief  Value of pixels with undefined physical value.
         *
         * @note This value is only valid for integer typed maps.
         */
        blank_type const blank_;

        /// Map progress notifier.
        mutable notifier_type notifier_;

    };

}  // MaRC

#endif  // MARC_PLOT_INFO_H
