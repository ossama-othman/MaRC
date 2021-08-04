// -*- C++ -*-
/**
 * @file marc/Vector.h
 *
 * %MaRC mathematical vector class and operations.
 *
 * Copyright (C) 2004, 2017-2018, 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_VECTOR_H
#define MARC_VECTOR_H

#include "marc/details/vector.h"

#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <stdexcept>
#include <ostream>


namespace MaRC
{
    /**
     * @class Vector Vector.h <marc/Vector.h>
     *
     * @brief Zero-based index mathematical vector implementation.
     *
     * This is a simple zero-based index mathematical vector
     * implementation with @a M rows.
     *
     * @tparam T Vector element type.  It should be an arithmetic type
     *           such as an integer or floating point type.
     * @tparam M The number of dimensions (rows) in the vector, i.e an
     *           @a M -dimensional vector.
     */
    template <typename T, std::size_t M>
    class Vector
    {
    public:

        /**
         * STL style container type aliases.
         */
        //@{
        using value_type      = T;
        using element_type    = T;
        using reference       = T &;
        using const_reference = T const &;
        using vector_type     = Vector<T, M>;
        using iterator        = T *;
        using const_iterator  = T const *;
        //@}

        /**
         * @brief Default constructor.
         *
         * Initialize the elements of the vector to the default value,
         * i.e. @c T(), which is 0 for arithmetic types.
         */
        Vector()
            : vector_{}
        {
            static_assert(std::is_arithmetic<T>(),
                          "Vector type should be an integer "
                          "or floating point type.");
        }

        /**
         * @brief Construct @c Vector from a parameter pack.
         *
         * This constructor allows a @c Vector to be direct
         * initialized like so:
         * @code
         *     Vector<int, 3> m(0, 1, 2); // direct initialization
         * @endcode
         */
        template <typename ... Args>
        constexpr Vector(Args ... args)
            : vector_{T(args) ...}
        {
        }

        /**
         * @brief Construct @c Vector from an initializer list.
         *
         * This constructor allows a @c Vector to be list initialized
         * like so:
         * @code
         *     Vector<int, 3> m{0, 1, 2};  // direct-list-initialization
         * @endcode
         * or:
         * @code
         *     Vector<int, 3> m = {0, 1, 2}; // copy-list-initialization
         * @endcode
         */
        Vector(std::initializer_list<T> rhs)
        {
            if (rhs.size() != M) {
                throw std::out_of_range("Number of vector / "
                                        "initializer list "
                                        "elements do not match.");
            }

            std::copy(std::cbegin(rhs), std::cend(rhs), this->begin());
        }

        // Default special members.
        Vector(Vector<T, M> const & rhs) = default;
        Vector<T, M> & operator=(Vector<T, M> const & rhs) = default;
        Vector(Vector && rhs) noexcept = default;
        Vector & operator=(Vector &&) noexcept = default;
        ~Vector() = default;

        /**
         * @brief Element accessor.
         *
         * @note No bounds checking.
         *
         * @param[in] row Zero-based vector row.
         *
         * @return Reference to element at given @c Vector @a row.
         */
        inline reference operator[](std::size_t row)
        {
            return this->vector_[row];
        }

        /**
         * @brief Const element accessor.
         *
         * @note No bounds checking.
         *
         * @param[in] row Zero-based vector row.
         *
         * @return Reference to @c const element at given @c Vector
         *         @a row.
         */
        inline const_reference operator[] (std::size_t row) const
        {
            return this->vector_[row];
        }

        /**
         * @brief Element accessor.
         *
         * @note With bounds checking.
         *
         * @param[in] row Zero-based vector row.
         *
         * @return Reference to element at given @c Vector @a row.
         */
        inline reference at(std::size_t row)
        {
            if (row >= M)
                throw std::out_of_range("Out of range vector index");

            return this->vector_[row];
        }

        /**
         * @brief Const element accessor.
         *
         * @note With bounds checking.
         *
         * @param[in] row Zero-based vector row.
         *
         * @return Reference to @c const element at given @c Vector
         *         @a row.
         */
        inline const_reference at(std::size_t row) const
        {
            if (row >= M)
                throw std::out_of_range ("Out of range vector index");

            return this->vector_[row];
        }

        /**
         * @brief Get iterator to the beginning of the @c Vector.
         *
         * @return Iterator to the beginning of this @c Vector.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the vector.  It is not intended for
         *       general use.
         */
        inline iterator begin()
        {
            return &this->vector_[0];
        }

        /**
         * @brief Get a @c const iterator to the beginning of the
         *        @c Vector.
         *
         * @return @c const iterator to the beginning of of this
         *         @c Vector.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the vector.  It is not intended for
         *       general use.
         */
        inline const_iterator begin() const
        {
            return &this->vector_[0];
        }

        /**
         * @brief Get iterator to the end of the @c Vector.
         *
         * @return Iterator to the end of of this @c Vector.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the vector.  It is not intended for
         *       general use.
         */
        inline iterator end()
        {
            return &this->vector_[0] + M;
        }

        /**
         * @brief Get @c const iterator to the end of the @c Vector.
         *
         * @return @c const iterator to the end of of this @c Vector.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the vector.  It is not intended for
         *       general use.
         */
        inline const_iterator end() const
        {
            return &this->vector_[0] + M;
        }

        /**
         * @brief Addition operator.
         *
         * @param[in] rhs @c Vector to be added to this one.
         *
         * @return This @c Vector after adding the @a rhs @c Vector to
         *         this one.
         */
        Vector<T, M> & operator+=(Vector<T, M> const & rhs)
        {
            auto dest = this->begin();

            for (auto const & src : rhs)
                *(dest++) += src;

            return *this;
        }

        /**
         * @brief Subtraction operator.
         *
         * @param[in] rhs @c Vector to be subtracted from this one.
         *
         * @return This @c Vector after substracting the @a rhs
         *         @c Vector from this one.
         */
        Vector<T, M> & operator-=(Vector<T, M> const & rhs)
        {
            auto dest = this->begin();

            for (auto const & src : rhs)
                *(dest++) -= src;

            return *this;
        }

        /**
         * @brief Scalar multiplication operator.
         *
         * @param[in] rhs Scalar by which this @c Vector will be
         *                multiplied.
         *
         * @return This @c Vector after multiplying it by the scalar
         *         the @a rhs.
         */
        Vector<T, M> & operator*=(T rhs)
        {
            for (auto & elem : this->vector_)
                elem *= rhs;

            return *this;
        }

        /**
         * @brief Obtain magnitude (norm) of this vector.
         *
         * @todo Should we rename this method to @c norm() to be
         *       consistent with linear algebra conventions?
         *
         * @return Magnitude of this vector.
         */
        double magnitude() const
        {
            return details::magnitude(this->vector_);
        }

        /**
         * @brief Convert vector to a unit vector.
         *
         * @attention This function requires that the vector @a v contain
         *            floating point values since it is not possible to
         *            store fractional values in an integer.
         */
        void to_unit_vector()
        {
            // This method will only work for integer types if the
            // vector  is already a unit vector (e.g. {0, 0, 1}).
            static_assert(std::is_floating_point<T>::value,
                          "to_unit_vector() cannot work reliably "
                          "for integer typed vectors.");

            auto const mag = this->magnitude();

            for (auto & elem : this->vector_)
                elem /= mag;
        }

    private:

        /// Underlying vector array.
        T vector_[M];
    };

    /**
     * @brief Obtain dot product of two vectors.
     * Given two vectors @a a and @a b, calculate their dot product.
     *
     * @param[in] a First  vector operand.
     * @param[in] b Second vector operand.
     *
     * @return Dot product of vectors @a a and @a b.
     *
     * @relates MaRC::Vector
     */
    template <typename T, std::size_t M>
    auto dot_product(Vector<T, M> const & a,
                     Vector<T, M> const & b)
    {
        return std::inner_product(std::cbegin(a),
                                  std::cend(a),
                                  std::cbegin(b),
                                  T(0));
    }

}

// ---------------------------------------------------------

/**
 * @brief Vector addition operator.
 *
 * @relates MaRC::Vector
 */
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator+(MaRC::Vector<T, M> const & lhs,
                                   MaRC::Vector<T, M> const & rhs)
{
    MaRC::Vector<T, M> vector(lhs);
    vector += rhs;

    return vector;
}

/**
 * @brief Vector subtraction operator.
 *
 * @relates MaRC::Vector
 */
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator-(MaRC::Vector<T, M> const & lhs,
                                   MaRC::Vector<T, M> const & rhs)
{
    MaRC::Vector<T, M> vector(lhs);
    vector -= rhs;

    return vector;
}

/**
 * @brief Vector/scalar multiplication operator.
 *
 * @relates MaRC::Vector
 */
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator*(MaRC::Vector<T, M> const & V,
                                   T x)
{
    MaRC::Vector<T, M> vector(V);
    vector *= x;

    return vector;
}

/**
 * @brief Vector/scalar multiplication operator.
 *
 * @relates MaRC::Vector
 */
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator*(T x,
                                   MaRC::Vector<T, M> const & V)
{
    return V * x;
}

// ---------------------------------------------------------

/**
 * @brief Vector equality operator.
 *
 * @relates MaRC::Vector
 */
template <typename T, std::size_t M>
bool operator==(MaRC::Vector<T, M> const & lhs,
                MaRC::Vector<T, M> const & rhs)
{
    /**
     * @bug This implementation only works reliably when the element
     *      type @c T is an integer, not floating point.
     */
    return std::equal(lhs.begin(), lhs.end(),
                      rhs.begin(), rhs.end());
}

/**
 * @brief Vector inequality operator.
 *
 * @relates MaRC::Vector
 */
template <typename T, std::size_t M>
bool operator!=(MaRC::Vector<T, M> const & lhs,
                MaRC::Vector<T, M> const & rhs)
{
    return !(lhs == rhs);
}


#endif  /* MARC_VECTOR_H */
