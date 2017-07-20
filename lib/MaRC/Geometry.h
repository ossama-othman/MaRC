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


namespace MaRC
{
  template <unsigned int M, typename T> class Vector;
  template <unsigned int M, unsigned int N, typename T> class Matrix;
  typedef Vector<3, double> DVector;
  typedef Vector<3, float>  FVector;
  typedef Matrix<3, 3, double> DMatrix;

  namespace Geometry
  {
    // All coordinate transformations assume a right-handed coordinate system

    /// Rotate about x-axis
    void RotX (const double &, const DVector &, DVector &);

    /// Rotate about y-axis
    void RotY (const double &, const DVector &, DVector &);

    /// Rotate about z-axis
    void RotZ (const double &, const DVector &, DVector &);

    /// Return a transformation matrix that rotates about the x-axis.
    DMatrix RotXMatrix (const double & angle);

    /// Return a transformation matrix that rotates about the y-axis.
    DMatrix RotYMatrix (const double & angle);

    /// Return a transformation matrix that rotates about the z-axis.
    DMatrix RotZMatrix (const double & angle);

    /// Obtain magnitude of vector of type double.
    double Magnitude (const DVector &);

    /// Obtain magnitude of vector of type float.
    double Magnitude (const FVector &);

    /// Convert a vector of type double to a unit vector.
    void toUnitVector (DVector &);

    /// Convert a vector of type float to a unit vector
    void toUnitVector (FVector &);
  }
}

#endif  /* MARC_GEOMETRY_H */
