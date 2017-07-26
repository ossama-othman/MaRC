//   -*- C++ -*-
/**
 * @file Geometry.h
 *
 * MaRC geometry related functions
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
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

#ifndef MARC_GEOMETRY_H
#define MARC_GEOMETRY_H

#include <cstddef>

#include <MaRC/Matrix.h>


namespace MaRC
{
  template <typename T, std::size_t M, std::size_t N> class Matrix;
  typedef Vector<double, 3> DVector;
  typedef Vector<float, 3>  FVector;
  typedef Matrix<double, 3, 3> DMatrix;

  namespace Geometry
  {
    // All coordinate transformations assume a right-handed coordinate system

    /// Rotate about x-axis
    void RotX(double, DVector const &, DVector &);

    /// Rotate about y-axis
    void RotY(double, DVector const &, DVector &);

    /// Rotate about z-axis
    void RotZ(double, DVector const &, DVector &);

    /// Return a transformation matrix that rotates about the x-axis.
    DMatrix RotXMatrix(double angle);

    /// Return a transformation matrix that rotates about the y-axis.
    DMatrix RotYMatrix(double angle);

    /// Return a transformation matrix that rotates about the z-axis.
    DMatrix RotZMatrix(double angle);

    /// Obtain magnitude of vector of type double.
    double Magnitude(DVector const &);

    /// Obtain magnitude of vector of type float.
    double Magnitude(const FVector &);

    /// Convert a vector of type double to a unit vector.
    void toUnitVector(DVector &);

    /// Convert a vector of type float to a unit vector
    void toUnitVector(FVector &);
  }
}

#endif  /* MARC_GEOMETRY_H */
