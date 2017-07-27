/**
 * @file GLLGeometricCorrection.cpp
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
#include <MaRC/GLLGeometricCorrection.h>

#include <cmath>


namespace MaRC
{
    // Note that these values are specific to Galileo spacecraft
    // images.
    namespace GLL
    {
        // The distortion constant must be a positive number.
        static const double DISTORTION = 0.00000000658;

        // Default optical axis line.
        static const double OA_LINE = 400;

        // Default optical axis sample.
        static const double OA_SAMPLE = 400;
    }
}


// Base summation mode detection on (samples > 1.1 * OA_SAMPLE)
// instead of (samples > OA_SAMPLE) to avoid potential inexact
// comparisons of two equal values e.g. (400.0 > 400.0000001).
MaRC::GLLGeometricCorrection::GLLGeometricCorrection (
    unsigned int samples)
    : summation_mode_(samples > 1.1 * MaRC::GLL::OA_SAMPLE
                      ? false : true)
{
}

void
MaRC::GLLGeometricCorrection::image_to_object(double & line,
                                              double & sample)
{
  // -------------- Image Space to Object Space  --------------------

  double x, y;  //  Optical axis corrected location

    if (this->summation_mode_) {
        // Convert to full-frame coordinates.
        x = sample * 2 - MaRC::GLL::OA_SAMPLE;
        y = line   * 2 - MaRC::GLL::OA_LINE;
    } else {
        x = sample - MaRC::GLL::OA_SAMPLE;
        y = line   - MaRC::GLL::OA_LINE;
    }

    // Image-space radius from optical axis
    double const is_rad = ::sqrt(x * x + y * y);

    // if (is_rad == 0) then no correction is necessary.

    if (is_rad != 0) {
        double const common_term1 = is_rad / (2 * MaRC::GLL::DISTORTION);
        double const common_term2 =
            ::sqrt(::pow (common_term1, 2) +
                   ::pow(1.0 / (3.0 * MaRC::GLL::DISTORTION), 3));

        double const A_3 = common_term1 + common_term2;
        double const B_3 = common_term1 - common_term2;

        // Object-space radius from optical axis
        double const os_rad =
            this->cube_root(A_3) + this->cube_root(B_3);

        line   = (os_rad * y) / is_rad + MaRC::GLL::OA_LINE;
        sample = (os_rad * x) / is_rad + MaRC::GLL::OA_SAMPLE;

        if (this->summation_mode_) {
            line   /= 2;
            sample /= 2;
        }
    }
}

void
MaRC::GLLGeometricCorrection::object_to_image(double & line,
                                              double & sample)
{
  // -------------- Object Space to Image Space  --------------------

    double x, y;  //  Optical axis corrected location

    if (this->summation_mode_) {
        // Convert to full-frame coordinates.
        x = sample * 2 - MaRC::GLL::OA_SAMPLE;
        y = line   * 2 - MaRC::GLL::OA_LINE;
    } else {
        x = sample - MaRC::GLL::OA_SAMPLE;
        y = line   - MaRC::GLL::OA_LINE;
    }

    double const common_term =
        1.0 + (MaRC::GLL::DISTORTION * (x * x + y * y));

    line   = y * common_term + MaRC::GLL::OA_LINE;
    sample = x * common_term + MaRC::GLL::OA_SAMPLE;

    if (this->summation_mode_) {
        line   /= 2;
        sample /= 2;
    }
}

double
MaRC::GLLGeometricCorrection::cube_root(double x)
{
    if      (x == 0) return 0;
    else if (x < 0)  return -::exp(::log(::fabs(x)) / 3.0);
    else             return  ::exp(::log(x) / 3.0);
}
