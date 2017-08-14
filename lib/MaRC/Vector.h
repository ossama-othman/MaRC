// -*- C++ -*-
/**
 * @file Vector.h
 *
 * MaRC mathematical vector class and operations.
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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
         *     Vector<int, 3> m{0, 1, 2};
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
         *     Vector<int, 3> m{0, 1, 2};
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

}

// ---------------------------------------------------------

/// Vector addition operator.
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator+(MaRC::Vector<T, M> const & lhs,
                                   MaRC::Vector<T, M> const & rhs)
{
    MaRC::Vector<T, M> vector(lhs);
    vector += rhs;

    return vector;
}

/// Vector subtraction operator
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator-(MaRC::Vector<T, M> const & lhs,
                                   MaRC::Vector<T, M> const & rhs)
{
    MaRC::Vector<T, M> vector(lhs);
    vector -= rhs;

    return vector;
}

/// Vector/scalar multiplication operator
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator*(MaRC::Vector<T, M> const & V,
                                   T x)
{
    MaRC::Vector<T, M> vector(V);
    vector *= x;

    return vector;
}

/// Vector/scalar multiplication operator
template <typename T, std::size_t M>
MaRC::Vector<T, M> const operator*(T x,
                                   MaRC::Vector<T, M> const & V)
{
    return V * x;
}

// ---------------------------------------------------------

/// Vector equality operator.
template <typename T, std::size_t M>
bool operator==(MaRC::Vector<T, M> const & lhs,
                MaRC::Vector<T, M> const & rhs)
{
    return std::equal(lhs.begin(), lhs.end(),
                      rhs.begin(), rhs.end(),
                      MaRC::equal_to<T>);
}

/// Vector inequality operator.
template <typename T, std::size_t M>
bool operator!=(MaRC::Vector<T, M> const & lhs,
                MaRC::Vector<T, M> const & rhs)
{
    return !(lhs == rhs);
}

// ---------------------------------------------------------

/// Stream insertion operator
template <typename T, std::size_t M>
std::ostream & operator<<(std::ostream & s, MaRC::Vector<T, M> const & v)
{
    s << "(" << M << ")\n";

    for (std::size_t row = 0; row < M; ++row)
        s << " " << v[row] << '\n';

    return s;
}


#endif  /* MARC_VECTOR_H */
