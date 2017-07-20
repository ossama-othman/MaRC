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

    /// Constructor.
    Matrix (T const & value = T ())
    {
      for (std::size_t row = 0; row < M; ++row)
        for (std::size_t col = 0; col < N; ++col)
          this->matrix_[row][col] = value;
    }

    /// Copy constructor.
    Matrix (Matrix<T, M, N> const & rhs)
    {
      for (std::size_t row = 0; row < M; ++row)
        for (std::size_t col = 0; col < N; ++col)
          this->matrix_[row][col] = rhs (row, col);
    }

    /// Assignment from another matrix operator.
    Matrix<T, M, N> & operator=(Matrix<T, M, N> const & rhs)
    {
      for (std::size_t row = 0; row < M; ++row)
        for (std::size_t col = 0; col < N; ++col)
          this->matrix_[row][col] = rhs (row, col);

      return *this;
    }

    /// Assignment from a scalar operator.
    Matrix<T, M, N> & operator=(T const & value)
    {
      for (std::size_t row = 0; row < M; ++row)
        for (std::size_t col = 0; col < N; ++col)
          this->matrix_[row][col] = value;

      return *this;
    }

    inline std::size_t rows (void) const
    {
      return M;
    }

    inline std::size_t columns (void) const
    {
      return N;
    }

    /// Element accessor.
    /**
     * @param row Zero-based matrix row.
     * @param col Zero-based matrix column.
     */
    inline reference operator()(std::size_t row, std::size_t col)
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
    inline const_reference operator()(std::size_t row,
				      std::size_t col) const
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
  template <typename T, std::size_t M, std::size_t N>
  Matrix<T, M, N> operator+(Matrix<T, M, N> const & lhs,
                            Matrix<T, M, N> const & rhs)
  {
    Matrix<T, M, N> matrix;

    for (std::size_t row = 0; row < M; ++row)
      for (std::size_t col = 0; col < N; ++col)
        matrix (row, col) = lhs (row, col) + rhs (row, col);

    return matrix;
  }

  /// Matrix subtraction operator
  template <typename T, std::size_t M, std::size_t N>
  Matrix<T, M, N> operator-(Matrix<T, M, N> const & lhs,
			    Matrix<T, M, N> const & rhs)
  {
    Matrix<T, M, N> matrix;

    for (std::size_t row = 0; row < M; ++row)
      for (std::size_t col = 0; col < N; ++col)
        matrix (row, col) = lhs (row, col) - rhs (row, col);

    return matrix;
  }

  /// Matrix/matrix multiplication operator.
  template <typename T, std::size_t M, std::size_t N, std::size_t R>
  Matrix<T, M, R> operator*(Matrix<T, M, N> const & lhs,
                            Matrix<T, N, R> const & rhs)
  {
    Matrix<T, M, R> matrix;

    for (std::size_t m = 0; m < M; ++m)
      for (std::size_t r = 0; r < R; ++r)
        {
          T & element = matrix(m, r); // Already default initialized.

          for (std::size_t n = 0; n < N; ++n)
            element += lhs (m, n) * rhs (n, r);
        }

    return matrix;
  }

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

  /// Matrix/vector multiplication operator.
  template <typename T, std::size_t M, std::size_t N>
  Vector<T, M> operator*(Matrix<T, M, N> const & A, Vector<T, N> const & x)
  {
    Vector<T, M> v;

    for (std::size_t m = 0; m < M; ++m)
      for (std::size_t n = 0; n < N; ++n)
        v[m] += A(m, n) * x[n];

    return v;
  }

  // ---------------------------------------------------------

  /// Stream insertion operator
  template <typename T, std::size_t M, std::size_t N>
  std::ostream & operator<<(std::ostream & s, Matrix<T, M, N> const & m)
  {
    s << "(" << M << " x " << N << ")" << std::endl;

    for (std::size_t row = 0; row < M; ++row) {
      for (std::size_t col = 0; col < N; ++col) {
	s << " " << m(row, col);
      }

      s << std::endl;
    }

    return s;
  }

}


#endif  /* MARC_MATRIX_H */
