// -*- C++ -*-
/**
 * @file extrema.h
 *
 * Copyright (C) 2019  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_EXTREMA_H
#define MARC_EXTREMA_H

#include <marc/Map_traits.h>

#include <type_traits>
#include <stdexcept>
#include <cmath>


namespace MaRC
{
    /**
     * @class extrema
     *
     * @brief Encapsulate minimum and maximum values.
     *
     * @tparam T Underlying extrema data type.
     */
    template <typename T>
    class extrema
    {
    public:

        /// Convenience type alias for given extremum.
        using extremum_type = T;

        /// Constructor.
        extrema()
            : minimum_(std::numeric_limits<T>::max()) // see below
            , maximum_(std::numeric_limits<T>::lowest())
        {
            /**
             * @note @c minimum_ is initialized to the maximum value
             *       of type @c T, and @c maximum_ to the lowest
             *       value of type @c T so that the first extrema
             *       update always succeeds, and so that it is obvious
             *       if no updates to the extrema occured.
             */
        }

        /**
         * @brief Constructor.
         *
         * @param[in] minimum Initial minimum value.
         * @param[in] maximum Initial maximum value.
         */
        template <typename U>
        extrema(U minimum, U maximum)
            : minimum_(validate_minimum(minimum))
            , maximum_(validate_maximum(maximum))
        {
            if (!this->is_valid())
                throw std::invalid_argument(
                    "Initial minimum not less than "
                    "or equal to maximum.");
        }

        /// Get minimum physical data value.
        auto minimum() const { return this->minimum_; }

        /// Get maximum physical data value.
        auto maximum() const { return this->maximum_; }

        /// Is the current pair of extrema are valid?
        bool is_valid() const
        {
            return this->minimum_ <= this-> maximum_;
        }

        /**
         * @brief Update physical data value extrema.
         *
         * Update extrema such that the minimum will only be updated
         * if the @a datum is less than the current minimum, and the
         * maximum will only be updated if the @a datum is greater
         * than the current maximum.
         *
         * @param[in] datum Physical data value.
         */
        void update(T datum)
        {
            /**
             * @todo Should we validate @a datum as is done in the
             *       constructor?
             */
            if (datum < this->minimum_)
                this->minimum_ = datum;

            if (datum > this->maximum_)
                this->maximum_ = datum;
        }

        /**
         * @brief Reset to a state than can be updated.
         *
         * Reset to extrema values that will always allow the initial
         * valid update to succeed.
         */
        void reset()
        {
            constexpr auto worst_min = std::numeric_limits<T>::max();
            constexpr auto worst_max = std::numeric_limits<T>::lowest();

            this->minimum_ = worst_min;
            this->maximum_ = worst_max;
        }

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
        template <typename U>
        static auto validate_extremum(U value)
        {
            if (std::is_floating_point<U>::value && std::isnan(value))
                throw std::invalid_argument(
                    "Extremum should not be NaN.");

            return value;
        }

        /**
         * @brief Verify desired minimum physical value is valid.
         *
         * @param[in] value Physical data minimum to be validated.
         *
         * @return @a value if it is valid.
         *
         * @throw std::invalid_argument @a value is @c NaN.
         */
        template <typename U>
        static auto validate_minimum(U value)
        {
            return Map_traits<T>::minimum(validate_extremum(value));
        }

        /**
         * @brief Verify desired maximum physical value is valid.
         *
         * @param[in] value Physical data maximum to be validated.
         *
         * @return @a value if it is valid.
         *
         * @throw std::invalid_argument @a value is @c NaN.
         */
        template <typename U>
        static auto validate_maximum(U value)
        {
            return Map_traits<T>::maximum(validate_extremum(value));
        }

    private:

        /// Minimum value.
        double minimum_;

        /// Maximum value.
        double maximum_;

    };

}  // MaRC

#endif  // MARC_EXTREMA_H
