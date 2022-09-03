/**
 * @file GLLGeometricCorrection.cpp
 *
 * Copyright (C) 2003-2004, 2017, 2022  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */
#include <marc/GLLGeometricCorrection.h>

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
        /// The distortion constant must be a positive number.
        static constexpr double DISTORTION = 0.00000000658;

        /// Default optical axis line.
        static constexpr std::size_t OA_LINE = 400;

        /// Default optical axis sample.
        static constexpr std::size_t OA_SAMPLE = 400;
    }
}

MaRC::GLLGeometricCorrection::GLLGeometricCorrection(std::size_t samples)
    : summation_mode_(samples <= MaRC::GLL::OA_SAMPLE)
{
}

void
MaRC::GLLGeometricCorrection::image_to_object(double & line,
                                              double & sample) const
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
    double const is_rad = std::hypot(x, y);

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
                                              double & sample) const
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
