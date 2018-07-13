// -*- C++ -*-
/**
 * @file Vector.h
 *
 * MaRC mathematical vector class and operations.
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
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

#ifndef MARC_VECTOR_H
#define MARC_VECTOR_H

#include "MaRC/Mathematics.h"

#include <type_traits>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <stdexcept>
#include <ostream>


namespace MaRC
{
    /**
     * @class Vector
     *
     * @brief Zero-based index mathematical vector implementation.
     *
     * This is a simple zero-based index mathematical vector
     * implementation with @a M rows.
     */
    template <typename T, std::size_t M>
    class Vector
    {
    public:

        typedef T            value_type;
        typedef T            element_type;
        typedef T &          reference;
        typedef T const &    const_reference;
        typedef Vector<T, M> vector_type;
        typedef T *          iterator;
        typedef T const *    const_iterator;

        /**
         * Default constructor.
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
         * Construct @c Vector from an initializer list.
         *
         * This constructor allows a @c Vector to be initialized like
         * so:
         *     Vector<int, 3> m{{0, 1, 2}};
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

        /// Copy constructor.
        Vector(Vector<T, M> const & rhs)
        {
            std::copy(std::cbegin(rhs), std::cend(rhs), this->begin());
        }

        /**
         * Construct @c Vector from an initializer list.
         *
         * This constructor allows a @c Vector to be initialized like
         * so:
         *     Vector<int, 3> m{{0, 1, 2}};
         */
        Vector<T, M> & operator=(std::initializer_list<T> rhs)
        {
            if (rhs.size() != M) {
                throw std::out_of_range("Number of vector / "
                                        "initializer list "
                                        "elements do not match.");
            }

            std::copy(std::cbegin(rhs), std::cend(rhs), this->begin());
        }

        /// Copy assignment operator.
        Vector<T, M> & operator=(Vector<T, M> const & rhs)
        {
            std::copy(std::cbegin(rhs), std::cend(rhs), this->begin());

            return *this;
        }

        /**
         * Element accessor.
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
         * Const element accessor.
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
         * Element accessor.
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
         * Const element accessor.
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
         * Get iterator to the beginning of the @c Vector.
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
         * Get a @c const iterator to the beginning of the @c Vector.
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
         * Get iterator to the end of the @c Vector.
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
         * Get @c const iterator to the end of the @c Vector.
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
         * Addition operator.
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
         * Subtraction operator.
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
         * Scalar multiplication operator.
         *
         * @param[in] rhs Scalar by which this @c Vector will be
         *                multiplied.
         *
         * @return This @c Vector after multiplying it by the scalar
         *         the @a rhs.
         */
        Vector<T, M> & operator*=(T rhs)
        {
            for (auto & elem : *this)
                elem *= rhs;

            return *this;
        }

  private:

        /// Underlying vector array.
        T vector_[M];
    };

    /// Obtain magnitude of vector.
    /**
     * This generalized implementation can return the magnitude of a
     * vector with an arbitrary number of rows
     *
     * @param[in] v Vector for which the magnitude will be
     *            calculated.
     *
     * @return Magnitude of vector @a v.
     *
     * @relates MaRC::Vector
     */
    template <typename T, std::size_t M>
    auto magnitude(Vector<T, M> const & v)
    {
        double const m = 0;

        /**
         * @bug This implementation is subject to overflow or
         *      underflow.
         */
        for (auto i = 0; i < M; ++i)
            m += v[i] * v[i];

        return std::sqrt(m);
    }

    /// Obtain magnitude of vector with three rows.
    /**
     * This implementation avoids overflow and underflow when
     * calculating the magnitude of vectors with three rows.
     *
     * @param[in] v Vector for which the magnitude will be
     *            calculated.
     *
     * @return Magnitude of vector @a v.
     *
     * @relates MaRC::Vector
     */
    template <typename T>
    auto magnitude(Vector<T, 3> const & v)
    {
        return MaRC::hypot(v[0], v[1], v[2]);
    }

    /// Obtain magnitude of vector with two rows.
    /**
     * This implementation avoids overflow and underflow when
     * calculating the magnitude of vectors with two rows.
     *
     * @param[in] v Vector for which the magnitude will be
     *            calculated.
     *
     * @return Magnitude of vector @a v.
     *
     * @relates MaRC::Vector
     */
    template <typename T>
    auto magnitude(Vector<T, 2> const & v)
    {
        return std::hypot(v[0], v[1]);
    }

    /// Obtain magnitude of vector with one row.
    /**
     * This implementation returns the magnitude of vector with one
     * row.
     *
     * @param[in] v Vector for which the magnitude will be
     *            calculated.
     *
     * @return Magnitude of vector @a v.
     *
     * @relates MaRC::Vector
     */
    template <typename T>
    auto magnitude(Vector<T, 1> const & v)
    {
        return v[0];
    }

    /// Convert a vector to a unit vector.
    /**
     * @param[in,out] v Vector to convert to a unit vector.
     *
     * @attention This function requires that the vector @a v contain
     *            floating point values since it is not possible to
     *            store fractional values in an integer.
     *
     * @relates MaRC::Vector
     */
    template <typename T, std::size_t M>
    void to_unit_vector(Vector<T, M> & v)
    {
        static_assert(std::is_floating_point<T>::value,
                      "MaRC::to_unit_vector() cannot work as expected "
                      "for integer typed vectors.");

        auto const mag = MaRC::magnitude(v);

        for(std::size_t i = 0; i < M; ++i)
            v[i] /= mag;
    }

    /// Obtain dot product of two vectors.
    /**
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
 * @brief Vector subtraction operator
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
 * Vector/scalar multiplication operator
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
 * Vector/scalar multiplication operator
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

// ---------------------------------------------------------

/**
 * @brief Stream insertion operator
 *
 * @relates MaRC::Vector
 */
template <typename T, std::size_t M>
std::ostream & operator<<(std::ostream & s, MaRC::Vector<T, M> const & v)
{
    s << "(" << M << ")\n";

    for (std::size_t row = 0; row < M; ++row)
        s << " " << v[row] << '\n';

    return s;
}


#endif  /* MARC_VECTOR_H */
