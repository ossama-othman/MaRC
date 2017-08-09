// $Id: GLLGeometricCorrection.cc,v 1.3 2004/07/06 00:01:41 othman Exp $

#include <MaRC/GLLGeometricCorrection.h>

#include <cmath>


namespace MaRC
{
  // Note that these values are specific to Galileo spacecraft
  // images.
  namespace GLL
  {
    // The distortion constant must be a positive number.
    static const double DISTORTION      = 0.00000000658;

    // Default optical axis line.
    static const double OA_LINE = 400;

    // Default optical axis sample.
    static const double OA_SAMPLE = 400;
  }
}


// Base summation mode detection on (samples > 1.1 * OA_SAMPLE)
// instead of (samples > OA_SAMPLE) to avoid potential inexact
// comparisons of two equal values e.g. (400.0 > 400.0000001).
MaRC::GLLGeometricCorrection::GLLGeometricCorrection (unsigned int samples)
  : summation_mode_ (samples > 1.1 * MaRC::GLL::OA_SAMPLE ? false : true)
{
}

void
MaRC::GLLGeometricCorrection::image_to_object (double & x,
                                               double & z)
{
  // -------------- Image Space to Object Space  --------------------

  // Image-space radius from optical axis
  double is_rad = ::sqrt (x * x + z * z);
  // must always be radius in pixels in *full-frame* mode
  if (this->summation_mode_)
    {
      is_rad *= 2;
    }

  // if (is_rad == 0) then no correction is necessary.

  if (is_rad != 0)
    {
      const double common_term1 = is_rad / (2 * MaRC::GLL::DISTORTION);
      const double common_term2 =
        ::sqrt (::pow (common_term1, 2) +
                ::pow (1.0 / (3.0 * MaRC::GLL::DISTORTION), 3));

      const double A_3 = common_term1 + common_term2;
      const double B_3 = common_term1 - common_term2;

      //  Object-space radius from optical axis
      const double os_rad = this->cube_root (A_3) + this->cube_root (B_3);

      z = (os_rad * z) / is_rad;
      x = (os_rad * x) / is_rad;
    }
}

void
MaRC::GLLGeometricCorrection::object_to_image (double & x,
                                               double & z)
{
  // -------------- Object Space to Image Space  --------------------

  double distance_squared = x * x + z * z;
  if (this->summation_mode_)
    {
      // Distance must be in *full-frame* coordinates
      distance_squared *= 4;
    }
  const double common_term = 1.0 + (MaRC::GLL::DISTORTION * distance_squared);

  z *= common_term;
  x *= common_term;	
}

MaRC::GeometricCorrection *
MaRC::GLLGeometricCorrection::clone (void) const
{
  // A copy constructed instance will suffice.
  return new GLLGeometricCorrection (*this);
}

inline double
MaRC::GLLGeometricCorrection::cube_root (const double & x)
{
  if      (x == 0) return 0;
  else if (x < 0)  return (-::exp (::log (::fabs (x)) / 3.0));
  else             return ( ::exp (::log (x) / 3.0));
}
