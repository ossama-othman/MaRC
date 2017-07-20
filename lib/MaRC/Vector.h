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

#include <stdexcept>
#include <iostream>

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
  template <unsigned int M, typename T = double>
  class Vector
  {
  public:

    typedef T            value_type;
    typedef T            element_type;
    typedef T &          reference;
    typedef T const &    const_reference;
    typedef Vector<M, T> vector_type;
    typedef T *          iterator;
    typedef T const *    const_iterator;

    /// Constructor.
    Vector (T const & value = T ())
    {
      for (unsigned int row = 0; row < M; ++row)
        this->vector_[row] = value;
    }

    /// Copy constructor.
    Vector (Vector<M, T> const & rhs)
    {
      for (unsigned int row = 0; row < M; ++row)
        this->vector_[row] = rhs[row];
    }

    /// Assignment from another vector operator.
    Vector<M, T> & operator= (Vector<M, T> const & rhs)
    {
      for (unsigned int row = 0; row < M; ++row)
        this->vector_[row] = rhs[row];

      return *this;
    }

    /// Assignment from a scalar operator.
    Vector<M, T> & operator= (T const & value)
    {
      for (unsigned int row = 0; row < M; ++row)
        this->vector_[row] = value;

      return *this;
    }

    inline unsigned int rows (void) const
    {
      return M;
    }

    /// Element accessor.
    /**
     * @param row Zero-based vector row.
     */
    inline reference operator[] (unsigned int row)
    {
      if (row >= M)
        throw std::out_of_range ("Out of range vector index");

      return this->vector_[row];
    }

    /// Const element accessor.
    /**
     * @param row Zero-based vector row.
     */
    inline const_reference operator[] (unsigned int row) const
    {
      if (row >= M)
        throw std::out_of_range ("Out of range vector index");

      return this->vector_[row];
    }

    inline iterator begin (void)
    {
      return &this->vector_[0];
    }

    inline const_iterator begin (void) const
    {
      return &this->vector_[0];
    }

    inline iterator end (void)
    {
      return &this->vector_[0];
    }

    inline const_iterator end (void) const
    {
      return &this->vector_[0];
    }


  private:

    /// Underlying vector array.
    T vector_[M];

  };

  // ---------------------------------------------------------

  /// Vector addition operator.
  template <unsigned int M, typename T>
  Vector<M, T> operator+ (Vector<M, T> const & lhs,
                          Vector<M, T> const & rhs)
  {
    Vector<M, T> vector;

    for (unsigned int row = 0; row < M; ++row)
      vector[row] = lhs[row] + rhs[row];

    return vector;
  }

  /// Vector subtraction operator
  template <unsigned int M, typename T>
  Vector<M, T> operator- (Vector<M, T> const & lhs,
                          Vector<M, T> const & rhs)
  {
    Vector<M, T> vector;

    for (unsigned int row = 0; row < M; ++row)
      vector[row] = lhs[row] - rhs[row];

    return vector;
  }

  // ---------------------------------------------------------

  /// Stream insertion operator
  template <unsigned int M, typename T>
  std::ostream & operator<< (std::ostream & s, Vector<M, T> const & v)
  {
    s << "(" << M << ")" << std::endl;

    for (unsigned int row = 0; row < M; ++row)
      s << " " << v[row] << std::endl;

    return s;
  }

}


#endif  /* MARC_MATRIX_H */
