//   -*- C++ -*-

//==========================================================================
/**
 *  @file Geometry.h
 *
 *  MaRC geometry related functions
 *
 *  @author Ossama Othman
 */
//==========================================================================

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
