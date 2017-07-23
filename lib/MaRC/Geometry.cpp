#include "Geometry.h"
#include "Matrix.h"

#include <cmath>


void
MaRC::Geometry::RotX(double angle,
                     DVector const & vec,
                     DVector & rotated)
{
  double const cosine = ::cos(angle);
  double const sine   = ::sin(angle);

  rotated[0] =  vec[0];
  rotated[1] =  vec[1] * cosine + vec[2] * sine;
  rotated[2] = -vec[1] * sine   + vec[2] * cosine;
}

void
MaRC::Geometry::RotY(double angle,
                     DVector const & vec,
                     DVector & rotated)
{
  double const cosine = ::cos(angle);
  double const sine   = ::sin(angle);

  rotated[0] = vec[0] * cosine - vec[2] * sine;
  rotated[1] = vec[1];
  rotated[2] = vec[0] * sine   + vec[2] * cosine;
}

void
MaRC::Geometry::RotZ(double angle,
                     DVector const & vec,
                     DVector &rotated)
{
  double const cosine = ::cos(angle);
  double const sine   = ::sin(angle);

  rotated[0] =  vec[0] * cosine + vec[1] * sine;
  rotated[1] = -vec[0] * sine   + vec[1] * cosine;
  rotated[2] =  vec[2];
}

MaRC::DMatrix
MaRC::Geometry::RotXMatrix(double angle)
{
  MaRC::DMatrix matrix;

  double const cosine = ::cos(angle);
  double const sine   = ::sin(angle);

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

  double const cosine = ::cos(angle);
  double const sine   = ::sin(angle);

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

  double const cosine = ::cos(angle);
  double const sine   = ::sin(angle);

  matrix(0, 0) =  cosine;
  matrix(0, 1) =  sine;
  matrix(1, 0) = -sine;
  matrix(1, 1) =  cosine;
  matrix(2, 2) =  1;

  return matrix;
}

double
MaRC::Geometry::Magnitude(const DVector & vec)
{
  return ::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

double
MaRC::Geometry::Magnitude(const FVector & vec)
{
  return ::sqrt(static_cast<double>(vec[0]) * vec[0]
                + vec[1] * vec[1]
                + vec[2] * vec[2]);
}

// The following really should be only for float or double vectors,
// not integer vectors!
void
MaRC::Geometry::toUnitVector(DVector &vec)
{
  double const mag = Geometry::Magnitude(vec);

  for(std::size_t i = 0; i < 3; ++i)
    vec[i] /= mag;
}

void
MaRC::Geometry::toUnitVector(FVector &vec)
{
  double const mag = Geometry::Magnitude(vec);

  for(std::size_t i = 0; i < 3; ++i)
      vec[i] /= static_cast<float>(mag);
}
