// -*- C++ -*-
/**
 * @file Matrix.h
 *
 * MaRC matrix class and operations.
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
         * Construct @c Matrix from an initializer list.
         *
         * This constructor allows a @c Matrix to be initialized like
         * so:
         *     Matrix<int, 3, 3> m{{0, 1, 2},
         *                         {1, 2, 3},
         *                         {2, 3, 4}};
         */
        Matrix(std::initializer_list<std::initializer_list<T>> rhs)
        {
            if (rhs.size() != M || rhs.begin()->size() != N)
                throw
                    std::out_of_range("Number of matrix / "
                                          "initializer list "
                                          "rows do not match.");

            T* dest = this->begin();

            for (auto const & row : rhs) {
                if (row.size() != N)
                    throw
                    std::out_of_range("Number of matrix / "
                                          "initializer list "
                                          "columns do not match.");

                std::copy(row.begin(), row.end(), dest);
                dest += N;  // Next matrix row.
            }
        }

        /// Copy constructor.
        Matrix(Matrix<T, M, N> const & rhs)
        {
            // Efficiently copy the matrix by treating it as a
            // one-dimensional array.

            std::copy(rhs.begin(),
                      rhs.end(),
                      this->begin());
        }

        /**
         * Assign an initializer list to a @c Matrix.
         *
         * This constructor allows an initializer_list to be assigned
         * to a @c Matrix list so:
         *     Matrix<int, 3, 3> m{{0, 1, 2},
         *                         {1, 2, 3},
         *                         {2, 3, 4}};
         */
        Matrix<T, M, N> & operator=(
            std::initializer_list<std::initializer_list<T>> rhs)
        {
            if (rhs.size() != M || rhs.begin()->size() != N)
                throw
                    std::out_of_range("Number of matrix / "
                                      "initializer list "
                                      "rows do not match.");

            T* dest = this->begin();

            for (auto const & row : rhs) {
                if (row.size() != N)
                    throw
                    std::out_of_range("Number of matrix / "
                                      "initializer list "
                                      "columns do not match.");

                std::copy(row.begin(), row.end(), dest);
                dest += N;  // Next matrix row.
            }

            return *this;
        }

        /// Assignment from another matrix operator.
        Matrix<T, M, N> & operator=(Matrix<T, M, N> const & rhs)
        {
            // Efficiently copy the matrix by treating it as a
            // one-dimensional array.
            //
            // Non-throwing for arithmetic types.
            std::copy(std::cbegin(rhs),
                      std::cend(rhs),
                      this->begin());

            return *this;
        }

        /**
         * Element accessor.
         *
         * @param[in] row Zero-based matrix row.
         * @param[in] col Zero-based matrix column.
         */
        inline reference operator()(std::size_t row, std::size_t col)
        {
            if (row >= M || col >= N)
                throw std::out_of_range("Out of range matrix index "
                                        "or indices");

            return this->matrix_[row][col];
        }

        /**
         * Const element accessor.
         * @param[[n] row Zero-based matrix row.
         * @param[in] col Zero-based matrix column.
         */
        inline const_reference operator()(std::size_t row,
                                          std::size_t col) const
        {
            if (row >= M || col >= N)
                throw std::out_of_range("Out of range matrix index "
                                        "or indices");

            return this->matrix_[row][col];
        }

        /**
         * Addition operator.
         */
        Matrix<T, M, N> & operator+=(Matrix<T, M, N> const & rhs)
        {
            T * dest = this->begin();
            T const * const src_end = rhs.end();

            for (T const * src = rhs.begin();
                 src != src_end;
                 ++src, ++dest)
                *dest += *src;

            return *this;
        }

        /**
         * Subtraction operator.
         */
        Matrix<T, M, N> & operator-=(Matrix<T, M, N> const & rhs)
        {
            T * dest = this->begin();
            T const * const src_end = rhs.end();

            for (T const * src = rhs.begin();
                 src != src_end;
                 ++src, ++dest)
                *dest -= *src;

            return *this;
        }

        /**
         * Get the begin iterator of the matrix.
         *
         * Return an iterator to the beginning of the flattened form
         * of this matrix.
         */
        constexpr inline iterator begin()
        {
            return &this->matrix_[0][0];
        }

        /**
         * Get the begin const iterator of the matrix.
         *
         * Return a const iterator to the beginning of the flattened
         * form of this matrix.  This method exists solely to
         * facilitate efficient copying of the matrix.
         */
        constexpr inline const_iterator begin() const
        {
            return &this->matrix_[0][0];
        }

        /**
         * Get the end iterator of the matrix.
         *
         * Return an iterator to the end of the flattened form of this
         * matrix.
         */
        constexpr inline iterator end()
        {
            return this->begin() + M * N;
        }

        /**
         * Get the end const iterator of the matrix.
         *
         * Return a const iterator to the end of the flattened form of
         * this matrix.
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

    /// Matrix transpose.
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

/// Matrix addition operator.
template <typename T, std::size_t M, std::size_t N>
MaRC::Matrix<T, M, N> operator+(MaRC::Matrix<T, M, N> const & lhs,
                                MaRC::Matrix<T, M, N> const & rhs)
{
    MaRC::Matrix<T, M, N> matrix(lhs);
    matrix += rhs;

    return matrix;
}

/// Matrix subtraction operator
template <typename T, std::size_t M, std::size_t N>
MaRC::Matrix<T, M, N> operator-(MaRC::Matrix<T, M, N> const & lhs,
                                MaRC::Matrix<T, M, N> const & rhs)
{
    MaRC::Matrix<T, M, N> matrix(lhs);
    matrix -= rhs;

    return matrix;
}

/// Matrix/matrix multiplication operator.
template <typename T, std::size_t M, std::size_t N, std::size_t R>
MaRC::Matrix<T, M, R> operator*(MaRC::Matrix<T, M, N> const & lhs,
                                MaRC::Matrix<T, N, R> const & rhs)
{
    MaRC::Matrix<T, M, R> matrix;

    for (std::size_t m = 0; m < M; ++m)
        for (std::size_t r = 0; r < R; ++r) {
            T & element = matrix(m, r); // Already default initialized.

            for (std::size_t n = 0; n < N; ++n)
                element += lhs(m, n) * rhs(n, r);
        }

    return matrix;
}

/// Matrix/vector multiplication operator.
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

// ---------------------------------------------------------

/// Matrix equality operator.
template <typename T, std::size_t M, std::size_t N>
bool operator==(MaRC::Matrix<T, M, N> const & lhs,
                MaRC::Matrix<T, M, N> const & rhs)
{
    return std::equal(lhs.begin(), lhs.end(),
                      rhs.begin(), rhs.end());
}

/// Matrix inequality operator.
template <typename T, std::size_t M, std::size_t N>
bool operator!=(MaRC::Matrix<T, M, N> const & lhs,
                MaRC::Matrix<T, M, N> const & rhs)
{
    return !(lhs == rhs);
}

// ---------------------------------------------------------

/// Stream insertion operator
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
