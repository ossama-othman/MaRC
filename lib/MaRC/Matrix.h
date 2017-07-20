// -*- C++ -*-

//==========================================================================
/**
 *  @file Matrix.h
 *
 *  MaRC matrix class and operations.
 *
 *  @author Ossama Othman
 */
//==========================================================================



#ifndef MARC_MATRIX_H
#define MARC_MATRIX_H

#include "MaRC/Vector.h"

namespace MaRC
{

  /**
   * @class Matrix
   *
   * @brief Simple and fast zero-based index matrix implementation.
   *
   * This matrix implementation is designed to be fast (e.g. no
   * dynamic memory allocations when initializing or copying).  It is
   * not super-optimized like some existing implementations but it is
   * good enough for MaRC's purposes.  Furthermore, it is really only
   * optimized for small matrices (e.g. 3x3).  Much larger matrices
   * will still incur large matrix element copying overhead.
   *
   * @note This matrix implementation uses zero-based indices.
   *
   * @todo Look into using super-optimized third party matrix
   *       implementations.
   */
  template <unsigned int M, unsigned int N, typename T = double>
  class Matrix
  {
  public:

    typedef T               value_type;
    typedef T               element_type;
    typedef T &             reference;
    typedef T const &       const_reference;
    typedef Matrix<M, N, T> matrix_type;
    typedef Matrix<N, M, T> transpose_type;

    /// Constructor.
    Matrix (T const & value = T ())
    {
      for (unsigned int row = 0; row < M; ++row)
        for (unsigned int col = 0; col < N; ++col)
          this->matrix_[row][col] = value;
    }

    /// Copy constructor.
    Matrix (Matrix<M, N, T> const & rhs)
    {
      for (unsigned int row = 0; row < M; ++row)
        for (unsigned int col = 0; col < N; ++col)
          this->matrix_[row][col] = rhs (row, col);
    }

    /// Assignment from another matrix operator.
    Matrix<M, N, T> & operator= (Matrix<M, N, T> const & rhs)
    {
      for (unsigned int row = 0; row < M; ++row)
        for (unsigned int col = 0; col < N; ++col)
          this->matrix_[row][col] = rhs (row, col);

      return *this;
    }

    /// Assignment from a scalar operator.
    Matrix<M, N, T> & operator= (T const & value)
    {
      for (unsigned int row = 0; row < M; ++row)
        for (unsigned int col = 0; col < N; ++col)
          this->matrix_[row][col] = value;

      return *this;
    }

    inline unsigned int rows (void) const
    {
      return M;
    }

    inline unsigned int columns (void) const
    {
      return N;
    }

    /// Element accessor.
    /**
     * @param row Zero-based matrix row.
     * @param col Zero-based matrix column.
     */
    inline reference operator() (unsigned int row, unsigned int col)
    {
      if (row >= M || col >= N)
        throw std::out_of_range ("Out of range matrix index or indices");

      return this->matrix_[row][col];
    }

    /// Const element accessor.
    /**
     * @param row Zero-based matrix row.
     * @param col Zero-based matrix column.
     */
    inline const_reference operator() (unsigned int row,
                                       unsigned int col) const
    {
      if (row >= M || col >= N)
        throw std::out_of_range ("Out of range matrix index or indices");

      return this->matrix_[row][col];
    }

  private:

    /// Underlying matrix array.
    T matrix_[M][N];

  };

  // ---------------------------------------------------------

  /// Matrix addition operator.
  template <unsigned int M, unsigned int N, typename T>
  Matrix<M, N, T> operator+ (Matrix<M, N, T> const & lhs,
                             Matrix<M, N, T> const & rhs)
  {
    Matrix<M, N, T> matrix;

    for (unsigned int row = 0; row < M; ++row)
      for (unsigned int col = 0; col < N; ++col)
        matrix (row, col) = lhs (row, col) + rhs (row, col);

    return matrix;
  }

  /// Matrix subtraction operator
  template <unsigned int M, unsigned int N, typename T>
  Matrix<M, N, T> operator- (Matrix<M, N, T> const & lhs,
                             Matrix<M, N, T> const & rhs)
  {
    Matrix<M, N, T> matrix;

    for (unsigned int row = 0; row < M; ++row)
      for (unsigned int col = 0; col < N; ++col)
        matrix (row, col) = lhs (row, col) - rhs (row, col);

    return matrix;
  }

  /// Matrix/matrix multiplication operator.
  template <unsigned int M, unsigned int N, unsigned int R, typename T>
  Matrix<M, R, T> operator* (Matrix<M, N, T> const & lhs,
                             Matrix<N, R, T> const & rhs)
  {
    Matrix<M, R, T> matrix;

    for (unsigned int m = 0; m < M; ++m)
      for (unsigned int r = 0; r < R; ++r)
        {
          T & element = matrix (m, r); // Already default initialized.

          for (unsigned int n = 0; n < N; ++n)
            element += lhs (m, n) * rhs (n, r);
        }

    return matrix;
  }

  /// Matrix transpose.
  template <unsigned int M, unsigned int N, typename T>
  Matrix<N, M, T> transpose (Matrix<M, N, T> const & m)
  {
    Matrix<N, M, T> t;

    for (unsigned int row = 0; row < M; ++row)
      for (unsigned int col = 0; col < N; ++col)
        t (col, row) = m (row, col);

    return t;
  }

  /// Matrix/vector multiplication operator.
  template <unsigned int M, unsigned int N, typename T>
  Vector<M, T> operator* (Matrix<M, N, T> const & A, Vector<N, T> const & x)
  {
    Vector<M, T> v;

    for (unsigned int m = 0; m < M; ++m)
      for (unsigned int n = 0; n < N; ++n)
        v[m] += A (m, n) * x[n];

    return v;
  }

  // ---------------------------------------------------------

  /// Stream insertion operator
  template <unsigned int M, unsigned int N, typename T>
  std::ostream & operator<< (std::ostream & s, Matrix<M, N, T> const & m)
  {
    s << "(" << M << " x " << N << ")" << std::endl;

    for (unsigned int row = 0; row < M; ++row)
      {
        for (unsigned int col = 0; col < N; ++col)
          {
            s << " " << m (row, col);
          }

        s << std::endl;
      }

    return s;
  }

}


#endif  /* MARC_MATRIX_H */
