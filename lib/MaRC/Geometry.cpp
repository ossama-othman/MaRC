/**
 * @file Geometry.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
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

#include "Geometry.h"
#include "Matrix.h"
#include "config.h"  // For MARC_HAS_3_PARAM_STD_HYPOT

#include <cmath>
#include <type_traits>  // For std::is_floating_point<>

namespace MaRC
{
    namespace
    {
#ifndef MARC_HAS_3_PARAM_STD_HYPOT
        /**
         * @brief Distance of (x,y,z) from the origin.
         *
         * Compute the distance of the point in space
         * (@a x, @a y, @a z) from the origin (0, 0, 0).  This
         * function is implemented in terms of the two-parameter
         * @c std::hypot() to leverage its ability to perform the
         * operation without floating point underflow or overflow, as
         * well as its excellent floating point error
         * characteristics.
         *
         * @param[in] x First  coordinate in space.
         * @param[in] y Second coordinate in space.
         * @param[in] z Third  coordinate in space.
         *
         * @return Distance from the origin to the point in space
         *         (@a x,@a y, @a z), i.e. the equivalent of the
         *         square root of the sum of the squares of each
         *         coordinate @c sqrt(x*x+y*y+z*z).
         *
         * @deprecated This implementation of the three-parameter
         *             @c std::hypot() will be dropped once we start
         *             using C++17 features in MaRC.
         */
        template <typename T>
        typename std::enable_if_t<std::is_floating_point<T>::value, T>
        hypot(T x, T y, T z)
        {
            /*
              Implement the missing three parameter std::hypot()
              function by nesting two std::hypot() calls.  This works
              since:
                  Given:
                      std::hypot(x,y)   = sqrt(x*x + y*y)
                  and:
                      std::hypot(x,y,z) = sqrt(x*x + y*y + z*z)
                  We have:
                      std::hypot(std::hypot(x, y), z)
                      = sqrt((sqrt(x*x + y*y) * sqrt(x*x + y*y)) + z*z)
                      = sqrt(x*x + y*y + z*z) =
                      = std::hypot(x, y, z)
             */
            return std::hypot(std::hypot(x, y), z);
        }
#endif  // !MARC_HAS_3_PARAM_STD_HYPOT

        using std::hypot;
    }
}

void
MaRC::Geometry::RotX(double angle,
                     DVector const & vec,
                     DVector & rotated)
{
  double const cosine = std::cos(angle);
  double const sine   = std::sin(angle);

  rotated[0] =  vec[0];
  rotated[1] =  vec[1] * cosine + vec[2] * sine;
  rotated[2] = -vec[1] * sine   + vec[2] * cosine;
}

void
MaRC::Geometry::RotY(double angle,
                     DVector const & vec,
                     DVector & rotated)
{
  double const cosine = std::cos(angle);
  double const sine   = std::sin(angle);

  rotated[0] = vec[0] * cosine - vec[2] * sine;
  rotated[1] = vec[1];
  rotated[2] = vec[0] * sine   + vec[2] * cosine;
}

void
MaRC::Geometry::RotZ(double angle,
                     DVector const & vec,
                     DVector &rotated)
{
  double const cosine = std::cos(angle);
  double const sine   = std::sin(angle);

  rotated[0] =  vec[0] * cosine + vec[1] * sine;
  rotated[1] = -vec[0] * sine   + vec[1] * cosine;
  rotated[2] =  vec[2];
}

MaRC::DMatrix
MaRC::Geometry::RotXMatrix(double angle)
{
  MaRC::DMatrix matrix;

  double const cosine = std::cos(angle);
  double const sine   = std::sin(angle);

  matrix(0, 0) =  1;
  matrix(1, 1) =  cosine;
  matrix(1, 2) =  sine;
  matrix(2, 1) = -sine;
  matrix(2, 2) =  cosine;

  return matrix;
}

MaRC::DMatrix
MaRC::Geometry::RotYMatrix(double angle)
{
  MaRC::DMatrix matrix;

  double const cosine = std::cos(angle);
  double const sine   = std::sin(angle);

  matrix(0, 0) =  cosine;
  matrix(0, 2) = -sine;
  matrix(1, 1) =  1;
  matrix(2, 0) =  sine;
  matrix(2, 2) =  cosine;

  return matrix;
}

MaRC::DMatrix
MaRC::Geometry::RotZMatrix(double angle)
{
  MaRC::DMatrix matrix;

  double const cosine = std::cos(angle);
  double const sine   = std::sin(angle);

  matrix(0, 0) =  cosine;
  matrix(0, 1) =  sine;
  matrix(1, 0) = -sine;
  matrix(1, 1) =  cosine;
  matrix(2, 2) =  1;

  return matrix;
}

double
MaRC::Geometry::Magnitude(DVector const & vec)
{
    return MaRC::hypot(vec[0], vec[1], vec[2]);
}

void
MaRC::Geometry::toUnitVector(DVector & vec)
{
    double const mag = Geometry::Magnitude(vec);

    for(std::size_t i = 0; i < 3; ++i)
        vec[i] /= mag;
}
