// -*- C++ -*-
/**
 * @file Matrix.h
 *
 * MaRC matrix class and operations.
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

#ifndef MARC_MATRIX_H
#define MARC_MATRIX_H

#include "MaRC/Vector.h"

#include <type_traits>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <ostream>


namespace MaRC
{

    /**
     * @class Matrix
     *
     * @brief Simple zero-based index matrix implementation.
     *
     * An @a M x @a N matrix implementation, where @a M is the number
     * of rows and @a N is the number of columns.
     *
     * This matrix implementation is designed to be fast (e.g. no
     * dynamic memory allocations when initializing or copying).  It
     * is not super-optimized like some existing implementations but
     * it is good enough for MaRC's purposes.  Furthermore, it is
     * really only optimized for small matrices (e.g. 3x3).  Much
     * larger matrices will still incur large matrix element copying
     * overhead.
     *
     * @note This matrix implementation uses zero-based indices.
     *
     * @todo Look into using super-optimized third party matrix
     *       implementations.
     *
     * @tparam T Matrix element type.  It should be an arithmetic type
     *           such as an integer or floating point type.
     * @tparam M The number of rows in the matrix.
     * @tparam N The number of columns in the matrix.
     */
    template <typename T, std::size_t M, std::size_t N>
    class Matrix
    {
    public:

        typedef T               value_type;
        typedef T               element_type;
        typedef T &             reference;
        typedef T const &       const_reference;
        typedef Matrix<T, M, N> matrix_type;
        typedef Matrix<T, N, M> transpose_type;
        typedef T *             iterator;
        typedef T const *       const_iterator;

        /// Constructor.
        Matrix()
            : matrix_{}
        {
            static_assert(std::is_arithmetic<T>(),
                          "Matrix type should be an integer "
                          "or floating point type.");
        }

        /**
         * @brief Construct @c Matrix from an initializer list.
         *
         * This constructor allows a @c Matrix to be initialized like
         * so:
         *     Matrix<int, 3, 2> m{{0, 1},
         *                         {1, 2},
         *                         {2, 3}};
         */
        Matrix(std::initializer_list<std::initializer_list<T>> rhs)
        {
            if (rhs.size() != M || rhs.begin()->size() != N) {
                throw std::out_of_range("Number of matrix / "
                                        "initializer list "
                                        "rows do not match.");
            }

            auto dest = this->begin();

            for (auto const & row : rhs) {
                if (row.size() != N) {
                    throw std::out_of_range("Number of matrix / "
                                            "initializer list "
                                            "columns do not match.");
                }

                std::copy(std::cbegin(row), std::cend(row), dest);
                dest += N;  // Next matrix row.
            }
        }

        /// Copy constructor.
        Matrix(Matrix<T, M, N> const & rhs)
        {
            // Efficiently copy the matrix by treating it as a
            // one-dimensional array.
            std::copy(std::cbegin(rhs), std::cend(rhs), this->begin());
        }

        /// Copy assignment operator.
        Matrix<T, M, N> & operator=(Matrix<T, M, N> const & rhs)
        {
            // Efficiently copy the matrix by treating it as a
            // one-dimensional array.
            //
            // Non-throwing for arithmetic types.
            std::copy(std::cbegin(rhs), std::cend(rhs), this->begin());

            return *this;
        }

        /**
         * @brief Element accessor.
         *
         * @note No bounds checking.
         *
         * @param[in] row    Zero-based matrix row.
         * @param[in] column Zero-based matrix column.
         *
         * @return Reference to element at given @c Matrix @a row and
         *         @a column.
         */
        inline reference operator()(std::size_t row, std::size_t column)
        {
            return this->matrix_[row][column];
        }

        /**
         * @brief Const element accessor.
         *
         * @note No bounds checking.
         *
         * @param[[n] row    Zero-based matrix row.
         * @param[in] column Zero-based matrix column.
         *
         * @return Reference to @c const element at given @c Matrix
         *         @a row and @a column.
         */
        inline const_reference operator()(std::size_t row,
                                          std::size_t column) const
        {
            return this->matrix_[row][column];
        }

        /**
         * @brief Element accessor.
         *
         * @note With bounds checking.
         *
         * @param[in] row    Zero-based matrix row.
         * @param[in] column Zero-based matrix column.
         *
         * @return Reference to element at given @c Matrix @a row and
         *         @a column.
         */
        inline reference at(std::size_t row, std::size_t column)
        {
            if (row >= M || column >= N) {
                throw std::out_of_range("Out of range matrix index "
                                        "or indices");
            }

            return this->matrix_[row][column];
        }

        /**
         * @brief Const element accessor.
         *
         * @note With bounds checking.
         *
         * @param[[n] row    Zero-based matrix row.
         * @param[in] column Zero-based matrix column.
         *
         * @return Reference to @c const element at given @c Matrix
         *         @a row and @a column.
         */
        inline const_reference at(std::size_t row,
                                  std::size_t column) const
        {
            if (row >= M || column >= N) {
                throw std::out_of_range("Out of range matrix index "
                                        "or indices");
            }

            return this->matrix_[row][column];
        }

        /**
         * @brief Addition operator.
         *
         * @param[in] rhs @c Matrix to be added to this one.
         *
         * @return This @c Matrix after adding the @a rhs @c Matrix to
         *         this one.
         */
        Matrix<T, M, N> & operator+=(Matrix<T, M, N> const & rhs)
        {
            auto dest = this->begin();

            for (auto const & src : rhs)
                *(dest++) += src;

            return *this;
        }

        /**
         * @brief Subtraction operator.
         *
         * @param[in] rhs @c Matrix to be substracted from this one.
         *
         * @return This @c Matrix after substracting the @a rhs
         *         @c Matrix from this one.
         */
        Matrix<T, M, N> & operator-=(Matrix<T, M, N> const & rhs)
        {
            auto dest = this->begin();

            for (auto const & src : rhs)
                *(dest++) -= src;

            return *this;
        }

        /**
         * @brief Scalar multiplication operator.
         *
         * @param[in] rhs Scalar by which this @c Matrix will be
         *                multiplied.
         *
         * @return This @c Matrix after multiplying it by the scalar
         *         the @a rhs.
         */
        Matrix<T, M, N> & operator*=(T rhs)
        {
            for (auto & elem : *this)
                elem *= rhs;

            return *this;
        }

        /**
         * @brief Get iterator to the beginning of the @c Matrix.
         *
         * @return Iterator to the beginning of the flattened form of
         *         this @c Matrix.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the matrix.  It is not intended for
         *       general use.
         */
        constexpr inline iterator begin()
        {
            return &this->matrix_[0][0];
        }

        /**
         * @brief Get a @c const iterator to the beginning of the
         *        @c Matrix.
         *
         * @return @c const iterator to the beginning of the flattened
         *         form of this @c Matrix.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the matrix.  It is not intended for
         *       general use.
         */
        constexpr inline const_iterator begin() const
        {
            return &this->matrix_[0][0];
        }

        /**
         * @brief Get iterator to the end of the @c Matrix.
         *
         * @return Iterator to the end of the flattened form of this
         *         @c Matrix.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the matrix.  It is not intended for
         *       general use.
         */
        constexpr inline iterator end()
        {
            return this->begin() + M * N;
        }

        /**
         * @brief Get a @c const iterator to the endof the @c Matrix.
         *
         * @return @c const iterator to the end of the flattened
         *         form of this @c Matrix.
         *
         * @note This method exists solely to facilitate efficient
         *       iteration of the matrix.  It is not intended for
         *       general use.
         */
        constexpr inline const_iterator end() const
        {
            return this->begin() + M * N;
        }

    private:

        /// Underlying matrix array.
        T matrix_[M][N];

    };

    // ---------------------------------------------------------

    /**
     * @brief Matrix transpose.
     *
     * @relates MaRC::Matrix
     */
    template <typename T, std::size_t M, std::size_t N>
    Matrix<T, N, M> transpose(Matrix<T, M, N> const & m)
    {
        Matrix<T, N, M> t;

        for (std::size_t row = 0; row < M; ++row)
            for (std::size_t col = 0; col < N; ++col)
                t(col, row) = m(row, col);

        return t;
    }

}

// ---------------------------------------------------------

/**
 * @brief Matrix addition operator.
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
MaRC::Matrix<T, M, N> operator+(MaRC::Matrix<T, M, N> const & lhs,
                                MaRC::Matrix<T, M, N> const & rhs)
{
    MaRC::Matrix<T, M, N> matrix(lhs);
    matrix += rhs;

    return matrix;
}

/**
 * @brief Matrix subtraction operator
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
MaRC::Matrix<T, M, N> operator-(MaRC::Matrix<T, M, N> const & lhs,
                                MaRC::Matrix<T, M, N> const & rhs)
{
    MaRC::Matrix<T, M, N> matrix(lhs);
    matrix -= rhs;

    return matrix;
}

/**
 * @brief Matrix/matrix multiplication operator.
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N, std::size_t R>
MaRC::Matrix<T, M, R> operator*(MaRC::Matrix<T, M, N> const & lhs,
                                MaRC::Matrix<T, N, R> const & rhs)
{
    MaRC::Matrix<T, M, R> matrix;

    for (std::size_t m = 0; m < M; ++m) {
        for (std::size_t r = 0; r < R; ++r) {
            T & element = matrix(m, r); // Already default initialized.

            for (std::size_t n = 0; n < N; ++n)
                element += lhs(m, n) * rhs(n, r);
        }
    }

    return matrix;
}

/**
 * @brief Matrix/vector multiplication operator.
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
MaRC::Vector<T, M> operator*(MaRC::Matrix<T, M, N> const & A,
                             MaRC::Vector<T, N> const & x)
{
    MaRC::Vector<T, M> v;

    for (std::size_t m = 0; m < M; ++m)
        for (std::size_t n = 0; n < N; ++n)
            v[m] += A(m, n) * x[n];

    return v;
}

/**
 * @brief Matrix/scalar multiplication operator.
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
MaRC::Matrix<T, M, N> operator*(MaRC::Matrix<T, M, N> const & A, T x)
{
    MaRC::Matrix<T, M, N> matrix(A);
    matrix *= x;

    return matrix;
}

/**
 * @brief Matrix/scalar multiplication operator.
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
MaRC::Matrix<T, M, N> operator*(T x, MaRC::Matrix<T, M, N> const & A)
{
    return A * x;
}

// ---------------------------------------------------------

/**
 * @brief Matrix equality operator.
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
bool operator==(MaRC::Matrix<T, M, N> const & lhs,
                MaRC::Matrix<T, M, N> const & rhs)
{
    /**
     * @bug This implementation only works reliably when the element
     *      type @c T is an integer, not floating point.
     */
    return std::equal(lhs.begin(), lhs.end(),
                      rhs.begin(), rhs.end());
}

/**
 * @brief Matrix inequality operator.
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
bool operator!=(MaRC::Matrix<T, M, N> const & lhs,
                MaRC::Matrix<T, M, N> const & rhs)
{
    return !(lhs == rhs);
}

// ---------------------------------------------------------

/**
 * @brief Stream insertion operator
 *
 * @relates MaRC::Matrix
 */
template <typename T, std::size_t M, std::size_t N>
std::ostream & operator<<(std::ostream & s,
                          MaRC::Matrix<T, M, N> const & m)
{
    s << "(" << M << " x " << N << ")\n";

    for (std::size_t row = 0; row < M; ++row) {
        for (std::size_t col = 0; col < N; ++col) {
            s << " " << m(row, col);
        }

        s << '\n';
    }

    return s;
}


#endif  /* MARC_MATRIX_H */
