/**
 * @file Geometry_Test.cpp
 *
 * Copyright (C) 2017 Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <MaRC/Geometry.h>
#include <MaRC/Mathematics.h>

#include <iostream>

namespace
{
    // "Units in the last place" for floating point equality
    // comparison.
    constexpr int ulps = 2;
}

bool test_vector_rotation()
{
    MaRC::DVector const v{ 3, 4, 5 };
    MaRC::DVector vr;

    constexpr double angle = 30;

    MaRC::Geometry::RotX(angle, v, vr);


    MaRC::Geometry::RotY(angle, v, vr);


    MaRC::Geometry::RotZ(angle, v, vr);


    return true;
}

bool test_transformation_matrices()
{
    constexpr double angle = 30;

    DMatrix xr = MaRC::Geometry::RotXMatrix(angle);
    DMatrix yr = MaRC::Geometry::RotYMatrix(angle);
    DMatrix zr = MaRC::Geometry::RotZMatrix(angle);

    return true;
}

bool test_vector_magnitude()
{
    MaRC::DVector const v{ 3, 4, 5 };

    double const mag =
        std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    return MaRC::almost_equal(MaRC::Geometry::Magnitude(v),
                              mag,
                              ulps);
}

bool test_unit_vector()
{
    MaRC::DVector v{ 3, 4, 5 };
    MaRC::Geometry::toUnitVector(v);

    constexpr double mag = 1;  // Unit vector magnitude is always 1.

    return
           std::abs(v[0]) <= 1
        && std::abs(v[1]) <= 1
        && std::abs(v[2]) <= 1
        && MaRC::almost_equal(MaRC::Geometry::Magnitude(v),
                              mag,
                              ulps);
}

int main()
{
    return
        test_vector_rotation()
        && test_transformation_matrices()
        && test_vector_magnitude()
        && test_unit_vector()
        ? 0 : -1;
}
