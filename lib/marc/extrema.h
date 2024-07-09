// -*- C++ -*-
/**
 * @file extrema.h
 *
 * Copyright (C) 2019-2020  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_EXTREMA_H
#define MARC_EXTREMA_H

#include <marc/Map_traits.h>

#include <optional>
#include <limits>
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
        extrema() = default;

        /**
         * @brief Constructor.
         *
         * @tparam U Type of minimum and maximum to be converted to
         *           type @c T.
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

        /// Copy constructor.
        extrema(extrema const &) = default;

        /**
         * @brief Converting copy constructor.
         *
         * @tparam U Underyling type of extrema to be converted to
         *           extrema of type @c T.
         * @param[in] other Extrema to be copied.
         */
        template <typename U>
        extrema(extrema<U> const & other)
            : extrema()
        {
            auto const & minimum = other.minimum();
            auto const & maximum = other.maximum();

            if (minimum)
                this->minimum_ = validate_minimum(*minimum);

            if (maximum)
                this->maximum_ = validate_maximum(*maximum);
        }

        /// Copy assignment operator.
        extrema & operator=(extrema const &) = default;

        /**
         * @brief Converting copy assignment operator.
         *
         * @tparam U Underyling type of extrema to be converted to
         *           extrema of type @c T.
         * @param[in] rhs Extrema to be copied.
         */
        template <typename U>
        extrema & operator=(extrema<U> rhs)
        {
            this->swap(rhs);

            return *this;
        }

        /// Move constructor.
        extrema(extrema && rhs) noexcept = default;

        /// Move assignment operator.
        extrema & operator=(extrema && rhs) noexcept = default;

        /// Destructor.
        ~extrema() = default;

        /// Set minimum physical data value.
        template <typename U>
        void minimum(U m) { this->minimum_ = validate_minimum(m); }

        /// Get minimum physical data value.
        std::optional<T> const & minimum() const { return this->minimum_; }

        /// Set maximum physical data value.
        template <typename U>
        void maximum(U m) { this->maximum_ = validate_maximum(m); }

        /// Get maximum physical data value.
        std::optional<T> const & maximum() const { return this->maximum_; }

        /// Is the current pair of extrema valid?
        bool is_valid() const
        {
            /*
              Valid if both extrema are set, and if minimum_ is less
              than or equal to maximum_.
            */
            return this->minimum_ && this->maximum_
                && this->minimum_ <= this-> maximum_;
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
            if (datum < this->minimum_ || !this->minimum_)
                this->minimum_ = datum;

            if (datum > this->maximum_ || !this->maximum_)
                this->maximum_ = datum;
        }

        /**
         * @brief Update physical data value extrema.
         *
         * Update extrema such that the minimum @c e.minimum() is less
         * than the current minimum, and the maximum will only be
         * updated if the @c e.maximum() datum is greater than the
         * current maximum.
         *
         * @param[in] e Extrema to be merged to this instance of
         *              extrema.
         */
        void update(extrema const & e)
        {
            /**
             * @todo Should we validate @c e.minimum_ and
             *       @c e.maximum_ as is done in the constructor?
             */
            if (e.minimum_ && *e.minimum_ < this->minimum_)
                this->minimum_ = *e.minimum_;

            if (e.maximum_ && *e.maximum_ > this->maximum_)
                this->maximum_ = *e.maximum_;
        }

        /**
         * @brief Reset to a state than can be updated.
         *
         * Reset to extrema values that will always allow the initial
         * valid update to succeed.
         */
        void reset()
        {
            this->minimum_.reset();
            this->maximum_.reset();
        }

        /**
         * @brief Swap extrema.
         *
         * @param[in,out] other Extrema to be swapped in to this
         *                      @c extrema<T> instance.
         */
        void swap(extrema & other) noexcept(
            std::is_nothrow_move_constructible<T>::value
            && std::is_nothrow_swappable<T>::value)
        {
            using std::swap;

            swap(this->minimum_, other.minimum_);
            swap(this->maximum_, other.maximum_);
        }

    private:

        /**
         * @brief Verify extremum (minimum or maxium) is valid.
         *
         * Verify that the given extremum @a value is valid, i.e. not
         * @c NaN.
         *
         * @tparam U Type of extremum to be converted to extremum of
         *           type @c T.
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
         * @tparam U Type of minimum to be converted to minimum of
         *           type @c T.
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
         * @tparam U Type of maximum to be converted to minimum of
         *           type @c T.
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
        std::optional<T> minimum_;

        /// Maximum value.
        std::optional<T> maximum_;

    };

    /**
     * @brief Swap contents of @c MaRC::extrema objects.
     *
     * @relates MaRC::extrema
     */
    template<typename T>
    void swap(MaRC::extrema<T> & lhs, MaRC::extrema<T> & rhs) noexcept(
        noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }

    /**
     * @brief Create an @c extrema object.
     *
     * @tparam T Underyling type of @c extrema.
     * @param[in] minimum Minium Value to be stored in the created
     *                    @c extrema object.
     * @param[in] maximum Maxium Value to be stored in the created
     *                    @c extrema object.
     *
     * @return @c extrema Object containing the provided @a minimum
     *                    and @c maximum..
     *
     * @relates MaRC::extrema
     */
    template<typename T>
    extrema<T> make_extrema(T minimum, T maximum)
    {
        return extrema<T>(minimum, maximum);
    }

    /**
     * @brief Create an @c extrema object.
     *
     * @tparam T Underyling type of @c extrema.
     * @tparam U Type of extrema to be converted to extrema of type
     *           @c T.
     * @param[in] minimum Minium Value to be stored in the created
     *                    @c extrema object.
     * @param[in] maximum Maxium Value to be stored in the created
     *                    @c extrema object.
     *
     * @return @c extrema Object containing the provided @a minimum
     *                    and @c maximum clipped to fit within type
     *                    @c T.
     *
     * @relates MaRC::extrema
     */
    template<typename T, typename U>
    extrema<T> make_extrema(U minimum, U maximum)
    {
        return extrema<T>(minimum, maximum);
    }

}  // MaRC

#endif  // MARC_EXTREMA_H
