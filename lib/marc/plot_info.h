// -*- C++ -*-
/**
 * @file plot_info.h
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PLOT_INFO_H
#define MARC_PLOT_INFO_H

#include <marc/Export.h>
#include <marc/Notifier.h>

#include <optional>
#include <limits>
#include <utility>
#include <cstdint>
#include <cmath>
#include <stdexcept>


namespace MaRC
{
    class SourceImage;

    /**
     * @class plot_info plot_info.h <marc/plot_info.h>
     *
     * @brief Map plotting information.
     *
     * Information that will be used when a plotting a map is
     * enscapsulated in this class.
     */
    class MARC_API plot_info
    {
    public:

        /// Convenience alias for the progress notifier type.
        using notifier_type = Progress::Notifier;

        /// Convenience alias for the blank integer type.
        using blank_type = std::optional<std::intmax_t>;

        /**
         * @brief Constructor used when no blank value is provided.
         *
         * By default, %MaRC will use @c 0 as the blank value for
         * integer typed maps, and always use %c NaN for floating
         * point typed maps.
         *
         * @param[in] source   @c SourceImage object containing the
         *                     data to be mapped.
         * @param[in] minimum  Minimum allowed physical data value on
         *                     map, i.e. all data greater than or
         *                     equal to @a minimum.
         * @param[in] maximum  Maximum allowed physical data value on
         *                     map, i.e. all data less than or equal
         *                     to @a maximum.
         */
        plot_info(SourceImage const & source,
                  double minimum,
                  double maximum)
            : source_(source)
            , desired_minimum_(validate_extremum(minimum))
            , desired_maximum_(validate_extremum(maximum))
            , minimum_(std::numeric_limits<double>::max())  // see below
            , maximum_(std::numeric_limits<double>::lowest())
            , blank_()
            , notifier_()
        {
            /**
             * @note @c minimum_ is initialized to the maximum
             *       @c double value, and @c maximum_ to the lowest
             *       @c double value to avoid potentially ignoring the
             *       value of the first plotted datum when setting the
             *       minimum and maximum plotted physical data
             *       values.
             */
        }

        /**
         * @brief Constructor for integer blank types.
         *
         * Constructor used when supplying an integer typed @a blank
         * value.
         *
         * @tparam    T        %MaRC blank integer type.
         * @param[in] source   @c SourceImage object containing the
         *                     data to be mapped.
         * @param[in] minimum  Minimum allowed physical data value on
         *                     map, i.e. all data greater than or
         *                     equal to @a minimum.
         * @param[in] maximum  Maximum allowed physical data value on
         *                     map, i.e. all data less than or equal
         *                     to @a maximum.
         * @param[in] blank    Blank map array value for integer typed
         *                     maps.  The blank map array value
         *                     corresponds to undefined "blank"
         *                     physical values.
         */
        template <typename T>
        plot_info(SourceImage const & source,
                  double minimum,
                  double maximum,
                  T blank)
            : source_(source)
            , desired_minimum_(validate_extremum(minimum))
            , desired_maximum_(validate_extremum(maximum))
            , minimum_(std::numeric_limits<double>::max())  // see below
            , maximum_(std::numeric_limits<double>::lowest())
            , blank_(std::move(blank))
            , notifier_()
        {
            /**
             * @note @c minimum_ is initialized to the maximum
             *       @c double value, and @c maximum_ to the lowest
             *       @c double value to avoid potentially ignoring the
             *       value of the first plotted datum when setting the
             *       minimum and maximum plotted physical data
             *       values.
             */
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
         * @param[in] source   @c SourceImage object containing the
         *                     data to be mapped.
         * @param[in] minimum  Minimum allowed physical data value on
         *                     map, i.e. all data greater than or
         *                     equal to @a minimum.
         * @param[in] maximum  Maximum allowed physical data value on
         *                     map, i.e. all data less than or equal
         *                     to @a maximum.
         */
        plot_info(SourceImage const & source,
                  double minimum,
                  double maximum,
                  double /* unused */)
            : plot_info(source, minimum, maximum)
        {
        }

        ~plot_info() = default;

        // Disallow copying.
        plot_info(plot_info const &) = delete;
        void operator=(plot_info const &) = delete;

        // Disallow moving.
        plot_info(plot_info &&) = delete;
        void operator=(plot_info &&) = delete;

        /// Get data to be mapped.
        SourceImage const & source() const { return this->source_; }

        /// Get minimum allowed physical data value on map.
        double desired_minimum() const { return this->desired_minimum_; }

        /// Set minimum allowed physical data value on map.
        void desired_minimum(double m)
        {
            this->desired_minimum_ = validate_extremum(m);
        }

        /// Get maximum allowed physical data value on map.
        double desired_maximum() const { return this->desired_maximum_; }

        /// Set maximum allowed physical data value on map.
        void desired_maximum(double m)
        {
            this->desired_maximum_ = validate_extremum(m);
        }

        /// Get minimum mapped physical data value.
        double minimum() const { return this->minimum_; }

        /// Set minimum mapped physical data value.
        void minimum(double m)
        {
            /**
             * @todo Should we validate 'm'?
             */
            if (m < this->minimum_)
                this->minimum_ = m;
        }

        /// Get maximum mapped physical data value.
        double maximum() const { return this->maximum_; }

        /// Set maximum mapped physical data value.
        void maximum(double m)
        {
            /**
             * @todo Should we validate 'm'?
             */
            if (m > this->maximum_)
                this->maximum_ = m;
        }

        // Was data plotted to the map?
        bool data_mapped() const
        {
            return this->minimum_ <= this->maximum_;
        }

        /// Get blank map array value.
        blank_type const & blank() const
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

        /**
         * @brief Verify extremum (minimum or maxium) is valid.
         *
         * Verify that the given extremum @a value is valid, i.e. not
         * @c NaN.
         *
         * @param[in] value Physical data extremum to be validated.
         *
         * @return @a value if it is valid.
         *
         * @throw std::invalid_argument @a value is @c NaN.
         */
        static double validate_extremum(double value)
        {
            if (std::isnan(value))
                throw std::invalid_argument(
                    "plot_info minimum/maximum should not be NaN.");

            return value;
        }

    private:

        /// @c SourceImage object containing the data to be mapped.
        SourceImage const & source_;

        /**
         * @brief Minimum allowed physical data value on the map,
         *        i.e. data >= @c minimum_.
         */
        double desired_minimum_;

        /**
         * @brief Maximum allowed physical data value on the map,
         *         i.e. data <= @c maximum_.
         */
        double desired_maximum_;


        /**
         * @brief Minimum mapped physical data value.
         */
        double minimum_;

        /**
          * @brief Maximum mapped physical data value.
         */
        double maximum_;

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
