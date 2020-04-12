/**
 * @file Geometry_Test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/Geometry.h>
#include <marc/Mathematics.h>
#include <marc/Constants.h>

#include <catch2/catch.hpp>

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

/**
 * @test Test rotation of vectors to different coordinate system.
 */
TEST_CASE("Vector rotation", "[geometry]")
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
     * @todo Switch to the %MaRC Vector equality operator once it
     *       correctly supports comparison of floating point types.
     */
    auto cmp =
        [](auto lhs, auto rhs)
        { return MaRC::almost_equal(lhs, rhs, ulps); };

    REQUIRE(std::equal(std::cbegin(Vx),
                       std::cend(Vx),
                       std::cbegin(Wx),
                       cmp));
    REQUIRE(std::equal(std::cbegin(Vy),
                       std::cend(Vy),
                       std::cbegin(Wy),
                       cmp));
    REQUIRE(std::equal(std::cbegin(Vz),
                       std::cend(Vz),
                       std::cbegin(Wz),
                       cmp));
}

/**
 * @test Test creation of rotation matrices.
 */
TEST_CASE("Rotation matricies", "[geometry]")
{
    MaRC::DMatrix Mx = MaRC::Geometry::RotXMatrix(angle);
    MaRC::DMatrix My = MaRC::Geometry::RotYMatrix(angle);
    MaRC::DMatrix Mz = MaRC::Geometry::RotZMatrix(angle);

    /**
     * @todo Switch to the %MaRC Vector equality operator once it
     *       correctly supports comparison of floating point types.
     */
    auto cmp =
        [](auto lhs, auto rhs)
        { return MaRC::almost_equal(lhs, rhs, ulps); };

    REQUIRE(std::equal(std::cbegin(Rx),
                       std::cend(Rx),
                       std::cbegin(Mx),
                       cmp));
    REQUIRE(std::equal(std::cbegin(Ry),
                       std::cend(Ry),
                       std::cbegin(My),
                       cmp));
    REQUIRE(std::equal(std::cbegin(Rz),
                       std::cend(Rz),
                       std::cbegin(Mz),
                       cmp));
}
