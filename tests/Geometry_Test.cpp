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
#include <MaRC/Constants.h>

#include<algorithm>


namespace
{
    // "Units in the last place" for floating point equality
    // comparison.
    constexpr int ulps = 4;

    // Arbitrary angle through which coordinate system rotations will
    // be performed.
    constexpr double angle = 30 * C::degree;  // radians

    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // Basic coordinate system rotations for a right-handed coordinate
    // system, as opposed to rotations in a fixed coordinate system
    // which would be the transpose of these matrices.
    MaRC::DMatrix const Rx{ { 1, 0,      0      },
                            { 0, cosine, sine   },
                            { 0, -sine,  cosine } };

    MaRC::DMatrix const Ry{ { cosine, 0, -sine  },
                            { 0,      1, 0      },
                            { sine,   0, cosine } };

    MaRC::DMatrix const Rz{ { cosine, sine,   0 },
                            { -sine,  cosine, 0 },
                            { 0,      0,      1 } };
}

bool test_vector_rotation()
{
    MaRC::DVector const V{ 3, 4, 5 };
    MaRC::DVector const Vx{ Rx * V };
    MaRC::DVector const Vy{ Ry * V };
    MaRC::DVector const Vz{ Rz * V };

    MaRC::DVector Wx, Wy, Wz;

    MaRC::Geometry::RotX(angle, V, Wx);
    MaRC::Geometry::RotY(angle, V, Wy);
    MaRC::Geometry::RotZ(angle, V, Wz);

    /**
     * @todo Switch to the MaRC Vector equality operator once it
     *       correctly supports comparison of floating point types.
     */
    auto cmp =
        [](auto lhs, auto rhs)
        { return MaRC::almost_equal(lhs, rhs, ulps); };

    return
        std::equal(std::cbegin(Vx),
                   std::cend(Vx),
                   std::cbegin(Wx),
                   cmp)
        && std::equal(std::cbegin(Vy),
                      std::cend(Vy),
                      std::cbegin(Wy),
                      cmp)
        && std::equal(std::cbegin(Vz),
                      std::cend(Vz),
                      std::cbegin(Wz),
                      cmp);
}

bool test_rotation_matrices()
{
    MaRC::DMatrix Mx = MaRC::Geometry::RotXMatrix(angle);
    MaRC::DMatrix My = MaRC::Geometry::RotYMatrix(angle);
    MaRC::DMatrix Mz = MaRC::Geometry::RotZMatrix(angle);

    /**
     * @todo Switch to the MaRC Vector equality operator once it
     *       correctly supports comparison of floating point types.
     */
    auto cmp =
        [](auto lhs, auto rhs)
        { return MaRC::almost_equal(lhs, rhs, ulps); };

    return
        std::equal(std::cbegin(Rx),
                   std::cend(Rx),
                   std::cbegin(Mx),
                   cmp)
        && std::equal(std::cbegin(Ry),
                      std::cend(Ry),
                      std::cbegin(My),
                      cmp)
        && std::equal(std::cbegin(Rz),
                      std::cend(Rz),
                      std::cbegin(Mz),
                      cmp);
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

    // Unit vector magnitude is always 1.
    constexpr double unit_mag = 1;

    return
           std::abs(v[0]) <= unit_mag
        && std::abs(v[1]) <= unit_mag
        && std::abs(v[2]) <= unit_mag
        && MaRC::almost_equal(MaRC::Geometry::Magnitude(v),
                              unit_mag,
                              ulps);
}

int main()
{
    return
        test_vector_rotation()
        && test_rotation_matrices()
        && test_vector_magnitude()
        && test_unit_vector()
        ? 0 : -1;
}
