// -*- C++ -*-

//==========================================================================
/**
 *  @file Vector.h
 *
 *  MaRC vector class and operations.
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_VECTOR_H
#define MARC_VECTOR_H


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
     * implementation.
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
            if (rhs.size() != M)
                throw
                    std::out_of_range("Number of vector / "
                                      "initializer list "
                                      "elements do not match.");

            std::copy(std::cbegin(rhs),
                      std::cend(rhs),
                      this->begin());
        }

        /// Copy constructor.
        Vector(Vector<T, M> const & rhs)
        {
            std::copy(std::cbegin(rhs),
                      std::cend(rhs),
                      this->begin());
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
            if (rhs.size() != M)
                throw
                    std::out_of_range("Number of vector / "
                                      "initializer list "
                                      "elements do not match.");

            std::copy(std::cbegin(rhs),
                      std::cend(rhs),
                      this->begin());
        }

        /// Assignment operator.
        Vector<T, M> & operator=(Vector<T, M> const & rhs)
        {
            std::copy(std::cbegin(rhs),
                      std::cend(rhs),
                      this->begin());

            return *this;
        }

        /**
         * Element accessor.
         *
         * @note No bounds checking.
         *
         * @param[in] row Zero-based vector row.
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
         */
        inline const_reference at(std::size_t row) const
        {
            if (row >= M)
                throw std::out_of_range ("Out of range vector index");

            return this->vector_[row];
        }

        /**
         * Get iterator to the beginning of the @c Vector.
         */
        inline iterator begin()
        {
            return &this->vector_[0];
        }

        /**
         * Get a const iterator to the beginning of the @c Vector.
         */
        inline const_iterator begin() const
        {
            return &this->vector_[0];
        }

        /**
         * Get iterator to the end of the @c Vector.
         */
        inline iterator end()
        {
            return &this->vector_[0] + M;
        }

        /**
         * Get const iterator to the end of the @c Vector.
         */
        inline const_iterator end() const
        {
            return &this->vector_[0] + M;
        }

        /**
         * Addition operator.
         */
        Vector<T, M> & operator+=(Vector<T, M> const & rhs)
        {
            auto dest = this->begin();
            auto const src_end = rhs.end();

            for (auto src = rhs.begin();
                 src != src_end;
                 ++src, ++dest)
                *dest += *src;

            return *this;
        }

        /**
         * Subtraction operator.
         */
        Vector<T, M> & operator-=(Vector<T, M> const & rhs)
        {
            auto dest = this->begin();
            auto const src_end = rhs.end();

            for (auto src = rhs.begin();
                 src != src_end;
                 ++src, ++dest)
                *dest -= *src;

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

// ---------------------------------------------------------

/// Vector equality operator.
template <typename T, std::size_t M>
bool operator==(MaRC::Vector<T, M> const & lhs,
                MaRC::Vector<T, M> const & rhs)
{
    return std::equal(lhs.begin(), lhs.end(),
                      rhs.begin(), rhs.end());
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
std::ostream & operator<< (std::ostream & s, MaRC::Vector<T, M> const & v)
{
    s << "(" << M << ")\n";

    for (std::size_t row = 0; row < M; ++row)
        s << " " << v[row] << '\n';

    return s;
}


#endif  /* MARC_VECTOR_H */
