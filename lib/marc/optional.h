// -*- C++ -*-
/**
 * @file optional.h
 *
 * Copyright (C) 2018  Ossama Othman
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
 *
 * @deprecated This header will be removed in favor of the C++17
 *             @<optional@> header once MaRC requires C++17.
 */

#ifndef MARC_OPTIONAL_H
#define MARC_OPTIONAL_H

#if __cplusplus < 201703L
# include <utility>
# include <exception>
# include <type_traits>
#else
# include <optional>
#endif  // __cplusplus < 201703L


namespace MaRC
{
#if __cplusplus < 201703L
    /**
     * @struct nullopt_t
     *
     * @brief Null initialization type for @c optional.
     */
    struct nullopt_t {
        /// Constructor.
        explicit constexpr nullopt_t(int) {}
    };

    /// Constant used to null initialize an @c optional object.
    constexpr nullopt_t nullopt{0};

    /**
     * @class bad_optional_access
     *
     * @brief Exception thrown when an accessing a @c optional object
     *        doesn't contain a value.
     *
     * @deprecated This is an implementation of the C++17
     *             @c std::bad_optional_access exception.  It will be
     *             removed when MaRC requires C++17.
     *
     * @see https://en.cppreference.com/w/cpp/utility/optional/bad_optional_access
     */
    class bad_optional_access : public std::exception
    {
    public:
        bad_optional_access() = default;
        virtual ~bad_optional_access() = default;

        /// Return description of this exception.
        virtual char const * what () const noexcept
        {
            return "bad optional access";
        }
    };

    /**
     * @class optional
     *
     * @brief Contain an optional value.
     *
     * @note This class only implements a subset of the C++17
     *       optional<> class template interface.  In particular, it
     *       missing several constructors, the @c emplace() method,
     *       the @c std::hash() specialization, etc.
     *
     * @deprecated This is an implementation of a subset of the C++17
     *             @c std::optional<> class template.  It will be
     *             removed when MaRC requires C++17.
     *
     * @see https://en.cppreference.com/w/cpp/utility/optional
     */
    template <typename T>
    class optional
    {
    public:

        /**
         * @name Members Found in the C++17 std::optional Class
         *
         * These are members found in the C++17 @c std::optional<>
         * class template, implemented for @c MaRC::optional<>.
         */
        //@{
        using value_type = T;

        constexpr optional() = default;
        constexpr optional(nullopt_t) noexcept : value_() {}
        constexpr optional(optional const & other) = default;
        constexpr optional(optional && other) = default;
        constexpr optional(T v) : value_(v, true) {}
        ~optional() = default;
        optional & operator=(nullopt_t) noexcept
        {
            this->reset();
            return *this;
        }
        optional & operator=(optional const & other) = default;
        optional & operator=(optional && other) = default;

        constexpr T const * operator->() const
        {
            return &this->value_.first;
        }

        constexpr T * operator->()
        {
            return &this->value_.first;
        }

        constexpr T const & operator*() const &
        {
            return this->value_.first;
        }

        constexpr T & operator*() &
        {
            return this->value_.first;
        }

        constexpr const T && operator*() const &&
        {
            return this->value_.first;
        }

        constexpr T && operator*() &&
        {
            return this->value_.first;
        }

        constexpr explicit operator bool() const noexcept
        {
            return this->value_.second;
        }

        constexpr bool has_value() const noexcept
        {
            return this->value_.second;
        }

        constexpr T & value() &
        {
            if (!this->has_value())
                throw bad_optional_access();

            return this->value_.first;
        }

        constexpr T const & value() const &
        {
            if (!this->has_value())
                throw bad_optional_access();

            return this->value_.first;
        }

        constexpr T && value() &&
        {
            if (!this->has_value())
                throw bad_optional_access();

            return this->value_.first;
        }

        constexpr T const && value() const &&
        {
            if (!this->has_value())
                throw bad_optional_access();

            return this->value_.first;
        }

        template<typename U>
        constexpr T value_or(U && default_value) const &
        {
            return
                this->has_value()
                ? this->value_.first
                : static_cast<T>(std::forward<U>(default_value));
        }

        template<typename U>
        constexpr T value_or(U && default_value) &&
        {
            return
                this->has_value()
                ? std::move(this->value_.first)
                : static_cast<T>(std::forward<U>(default_value));
        }

        void swap(optional & other) noexcept(
            std::is_nothrow_move_constructible<T>::value
            /* && std::is_nothrow_swappable<T>::value */)
        {
            this->value_.swap(other.value_);
        }

        void reset() noexcept
        {
            this->value_ = decltype(this->value_)();
        }
        //@}

    private:

        /**
         * @brief Member that tracks the existence of an %optional
         *        value, and the value itself.
         */
        std::pair<T, bool> value_;

    };

    /**
     * @brief Create an @c optional object.
     *
     * @param[in,out] value Value to be stored in the created
     *                      @c optional object.
     *
     * @return @c optional object containing the provided @a value.
     */
    template<typename T>
    constexpr optional<std::decay_t<T>> make_optional(T && value)
    {
        return optional<std::decay_t<T>>(std::forward<T>(value));
    }

    /**
     * @name Comparison Operators
     */
    //@{
    template<typename T, typename U>
    constexpr bool operator==(optional<T> const & lhs,
                              optional<U> const & rhs)
    {
        return (lhs && rhs && *lhs == *rhs)
            || (!lhs && !rhs);
    }

    template<typename T, typename U>
    constexpr bool operator!=(optional<T> const & lhs,
                              optional<U> const & rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename U>
    constexpr bool operator<(optional<T> const & lhs,
                             optional<U> const & rhs)
    {
        return !lhs || (rhs && *lhs < *rhs);
    }

    template<typename T, typename U>
    constexpr bool operator<=(optional<T> const & lhs,
                              optional<U> const & rhs)
    {
        return !lhs || (rhs && *lhs <= *rhs);
    }

    template<typename T, typename U>
    constexpr bool operator>(optional<T> const & lhs,
                             optional<U> const & rhs)
    {
        return rhs < lhs;
    }

    template<typename T, typename U>
    constexpr bool operator>=(optional<T> const & lhs,
                              optional<U> const & rhs)
    {
        return rhs <= lhs;
    }

    // -----------------

    template< class T >
    constexpr bool operator==(optional<T> const & opt, nullopt_t) noexcept
    {
        return !opt;
    }

    template< class T >
    constexpr bool operator==(nullopt_t, optional<T> const & opt) noexcept
    {
        return !opt;
    }

    template< class T >
    constexpr bool operator!=(optional<T> const & opt, nullopt_t) noexcept
    {
        return bool(opt);
    }

    template< class T >
    constexpr bool operator!=(nullopt_t,
                              optional<T> const & opt) noexcept
    {
        return bool(opt);
    }

    template< class T >
    constexpr bool operator<(optional<T> const &, nullopt_t) noexcept
    {
        return false;
    }

    template< class T >
    constexpr bool operator<(nullopt_t, optional<T> const & opt) noexcept
    {
        return bool(opt);
    }

    template< class T >
    constexpr bool operator<=(optional<T> const & opt, nullopt_t) noexcept
    {
        return !opt;
    }

    template< class T >
    constexpr bool operator<=(nullopt_t, optional<T> const &) noexcept
    {
        return true;
    }

    template< class T >
    constexpr bool operator>(optional<T> const & opt, nullopt_t) noexcept
    {
        return bool(opt);
    }

    template< class T >
    constexpr bool operator>(nullopt_t, optional<T> const &) noexcept
    {
        return false;
    }

    template< class T >
    constexpr bool operator>=(optional<T> const &, nullopt_t) noexcept
    {
        return true;
    }

    template< class T >
    constexpr bool operator>=(nullopt_t,
                              optional<T> const & opt) noexcept
    {
        return !opt;
    }

    // -----------------

    template<typename T, typename U>
    constexpr bool operator==(optional<T> const & opt,
                              U const & value)
    {
        return opt ? *opt == value : false;
    }

    template<typename T, typename U>
    constexpr bool operator==(T const & value,
                              optional<U> const & opt)
    {
        return opt ? value == *opt : false;
    }

    template<typename T, typename U>
    constexpr bool operator!=(optional<T> const & opt,
                              U const & value)
    {
        return opt ? *opt != value : true;
    }

    template<typename T, typename U>
    constexpr bool operator!=(T const & value,
                              optional<U> const & opt)
    {
        return opt ? value != *opt : true;
    }

    template<typename T, typename U>
    constexpr bool operator<(optional<T> const & opt,
                             U const & value)
    {
        return opt ? *opt < value : true;
    }

    template<typename T, typename U>
    constexpr bool operator<(T const & value,
                             optional<U> const & opt)
    {
        return opt ? value < *opt : false;
    }

    template<typename T, typename U>
    constexpr bool operator<=(optional<T> const & opt,
                              U const & value)
    {
        return opt ? *opt <= value : true;
    }

    template<typename T, typename U>
    constexpr bool operator<=(T const & value,
                              optional<U> const & opt)
    {
        return opt ? value <= *opt : false;
    }

    template<typename T, typename U>
    constexpr bool operator>(optional<T> const & opt,
                             U const & value)
    {
        return opt ? *opt > value : false;
    }

    template<typename T, typename U>
    constexpr bool operator>(T const & value,
                             optional<U> const & opt)
    {
        return opt ? value > *opt : true;
    }

    template<typename T, typename U>
    constexpr bool operator>=(optional<T> const & opt,
                              U const & value)
    {
        return opt ? *opt >= value : false;
    }

    template<typename T, typename U>
    constexpr bool operator>=(T const & value,
                              optional<U> const & opt)
    {
        return opt ? value >= *opt : true;
    }
    //@}
#else
    using std::nullopt_t;
    using std::nullopt;
    using std::bad_optional_access;
    using std::optional;
    using std::make_optional;
#endif  // __cplusplus >= 201703L
}

#if __cplusplus < 201703L
namespace std
{
    /**
     * @brief Swap contents of MaRC::optional objects.
     */
    template<typename T>
    void swap(MaRC::optional<T> & lhs, MaRC::optional<T> & rhs) noexcept(
        noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }
}
#endif  // __cplusplus < 201703L

#endif  // MARC_OPTIONAL_H
