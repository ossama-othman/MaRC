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

#include <array>
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
  using Vector = std::array<T, M>;

  // ---------------------------------------------------------

  /// Vector addition operator.
  template <typename T, std::size_t M>
  Vector<T, M> operator+(Vector<T, M> const & lhs,
			 Vector<T, M> const & rhs)
  {
    Vector<T, M> vector;

    for (std::size_t row = 0; row < M; ++row)
      vector[row] = lhs[row] + rhs[row];

    return vector;
  }

  /// Vector subtraction operator
  template <typename T, std::size_t M>
  Vector<T, M> operator- (Vector<T, M> const & lhs,
                          Vector<T, M> const & rhs)
  {
    Vector<T, M> vector;

    for (std::size_t row = 0; row < M; ++row)
      vector[row] = lhs[row] - rhs[row];

    return vector;
  }

  // ---------------------------------------------------------

  /// Stream insertion operator
  template <typename T, std::size_t M>
  std::ostream & operator<< (std::ostream & s, Vector<T, M> const & v)
  {
    s << "(" << M << ")\n";

    for (std::size_t row = 0; row < M; ++row)
      s << " " << v[row] << '\n';

    return s;
  }

}


#endif  /* MARC_VECTOR_H */
