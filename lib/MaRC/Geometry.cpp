#include "Geometry.h"
#include "Matrix.h"

#include <cmath>


void
MaRC::Geometry::RotX (const double & angle,
                      const DVector & vec,
                      DVector & rotated)
{
  rotated[0] =  vec[0];
  rotated[1] =  vec[1] * ::cos (angle) + vec[2] * ::sin (angle);
  rotated[2] = -vec[1] * ::sin (angle) + vec[2] * ::cos (angle);
}

void
MaRC::Geometry::RotY (const double & angle,
                      const DVector & vec,
                      DVector & rotated)
{
  rotated[0] = vec[0] * ::cos (angle) - vec[2] * ::sin (angle);
  rotated[1] = vec[1];
  rotated[2] = vec[0] * ::sin (angle) + vec[2] * ::cos (angle);
}

void
MaRC::Geometry::RotZ (const double &angle,
                      const DVector &vec,
                      DVector &rotated)
{
  rotated[0] =  vec[0] * ::cos (angle) + vec[1] * ::sin (angle);
  rotated[1] = -vec[0] * ::sin (angle) + vec[1] * ::cos (angle);
  rotated[2] =  vec[2];
}

MaRC::DMatrix
MaRC::Geometry::RotXMatrix (const double & angle)
{
  MaRC::DMatrix Matricks;

  Matricks = 0;

  Matricks (0, 0) =  1;
  Matricks (1, 1) =  ::cos (angle);
  Matricks (1, 2) =  ::sin (angle);
  Matricks (2, 1) = -::sin (angle);
  Matricks (2, 2) =  ::cos (angle);

  return Matricks;
}

MaRC::DMatrix
MaRC::Geometry::RotYMatrix (const double & angle)
{
  MaRC::DMatrix Matricks;

  Matricks = 0;

  Matricks (0, 0) =  ::cos (angle);
  Matricks (0, 2) = -::sin (angle);
  Matricks (1, 1) =  1;
  Matricks (2, 0) =  ::sin (angle);
  Matricks (2, 2) =  ::cos (angle);

  return Matricks;
}

MaRC::DMatrix
MaRC::Geometry::RotZMatrix (const double & angle)
{
  MaRC::DMatrix Matricks;

  Matricks = 0;

  Matricks (0, 0) =  ::cos (angle);
  Matricks (0, 1) =  ::sin (angle);
  Matricks (1, 0) = -::sin (angle);
  Matricks (1, 1) =  ::cos (angle);
  Matricks (2, 2) =  1;

  return Matricks;
}

double
MaRC::Geometry::Magnitude (const DVector & vec)
{
  return ::sqrt (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

double
MaRC::Geometry::Magnitude (const FVector & vec)
{
  return ::sqrt (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// The following really should be only for float or double vectors,
// not integer vectors!
void
MaRC::Geometry::toUnitVector (DVector &vec)
{
  const double mag = Geometry::Magnitude (vec);

  for (unsigned int i = 0; i < 3; ++i)
    vec[i] /= mag;
}

void
MaRC::Geometry::toUnitVector (FVector &vec)
{
  const double mag = Geometry::Magnitude (vec);

  for (unsigned int i = 0; i < 3; ++i)
    vec[i] /= static_cast<float> (mag);
}
