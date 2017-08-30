/**
 * @file GLLGeometricCorrection.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 * Copyright (C) 2005-2006  Nathanael Nerode
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
 * @author Nathanael Nerode
 */
#include <MaRC/GLLGeometricCorrection.h>

#include <cmath>


namespace MaRC
{
    /**
     * @namespace GLL
     *
     * @brief Galileo spacecraft lens aberration constants.
     */
    namespace GLL
    {
        // The distortion constant must be a positive number.
        static constexpr double DISTORTION = 0.00000000658;

        // Default optical axis line.
        static constexpr double OA_LINE = 400;

        // Default optical axis sample.
        static constexpr double OA_SAMPLE = 400;
    }
}


// Base summation mode detection on (samples > 1.1 * OA_SAMPLE)
// instead of (samples > OA_SAMPLE) to avoid potential inexact
// comparisons of two equal values e.g. (400.0 > 400.0000001).
MaRC::GLLGeometricCorrection::GLLGeometricCorrection(std::size_t samples)
    : summation_mode_(samples > 1.1 * MaRC::GLL::OA_SAMPLE
                      ? false : true)
{
}

void
MaRC::GLLGeometricCorrection::image_to_object(double & z, double & x)
{
    // -------------- Image Space to Object Space  --------------------

    // Image-space radius from optical axis
    double is_rad = std::hypot(x, z);

    // must always be radius in pixels in *full-frame* mode
    if (this->summation_mode_) {
        is_rad *= 2;
    }

    // if (is_rad == 0) then no correction is necessary.

    if (is_rad != 0) {
        double const common_term1 = is_rad / (2 * MaRC::GLL::DISTORTION);
        double const common_term2 =
            std::sqrt(std::pow(common_term1, 2) +
                      std::pow(1.0 / (3.0 * MaRC::GLL::DISTORTION), 3));

        double const A_3 = common_term1 + common_term2;
        double const B_3 = common_term1 - common_term2;

        // Object-space radius from optical axis
        double const os_rad = std::cbrt(A_3) + std::cbrt(B_3);

      z = (os_rad * z) / is_rad;
      x = (os_rad * x) / is_rad;
    }
}

void
MaRC::GLLGeometricCorrection::object_to_image(double & z, double & x)
{
    // -------------- Object Space to Image Space  --------------------

    double distance_squared = x * x + z * z;
    if (this->summation_mode_) {
        // Distance must be in *full-frame* coordinates
        distance_squared *= 4;
    }

    double const common_term =
        1.0 + (MaRC::GLL::DISTORTION * distance_squared);

    z *= common_term;
    x *= common_term;
}
