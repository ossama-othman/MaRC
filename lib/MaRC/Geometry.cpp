/**
 * @file Geometry.cpp
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

#include "Geometry.h"
#include "Matrix.h"
#include "Mathematics.h"

#include <cmath>


void
MaRC::Geometry::RotX(double angle,
                     DVector const & vec,
                     DVector & rotated)
{
    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // { 1,           0,          0 }   { vec[0] }
    // { 0,  cos(angle), sin(angle) } * { vec[1] }
    // { 0, -sin(angle), cos(angle} }   { vec[2] }

    rotated[0] =  vec[0];
    rotated[1] =  vec[1] * cosine + vec[2] * sine;
    rotated[2] = -vec[1] * sine   + vec[2] * cosine;
}

void
MaRC::Geometry::RotY(double angle,
                     DVector const & vec,
                     DVector & rotated)
{
    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // { cos(angle), 0, -sin(angle) }   { vec[0] }
    // { 0,          1,           0 } * { vec[1] }
    // { sin(angle), 0,  cos(angle} }   { vec[2] }

    rotated[0] = vec[0] * cosine - vec[2] * sine;
    rotated[1] = vec[1];
    rotated[2] = vec[0] * sine   + vec[2] * cosine;
}

void
MaRC::Geometry::RotZ(double angle,
                     DVector const & vec,
                     DVector &rotated)
{
    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // {  cos(angle), sin(angle), 0 }   { vec[0] }
    // { -sin(angle), cos(angle}, 0 } * { vec[1] }
    // {  0,          0,          1 }   { vec[2] }

    rotated[0] =  vec[0] * cosine + vec[1] * sine;
    rotated[1] = -vec[0] * sine   + vec[1] * cosine;
    rotated[2] =  vec[2];
}

MaRC::DMatrix
MaRC::Geometry::RotXMatrix(double angle)
{
    MaRC::DMatrix matrix;

    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // { 1,           0,          0 }
    // { 0,  cos(angle), sin(angle) }
    // { 0, -sin(angle), cos(angle} }

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

    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // { cos(angle), 0, -sin(angle) }
    // {          0, 1,           0 }
    // { sin(angle), 0,  cos(angle} }

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

    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // {  cos(angle), sin(angle), 0 }
    // { -sin(angle), cos(angle}, 0 }
    // {  0,          0,          1 }

    matrix(0, 0) =  cosine;
    matrix(0, 1) =  sine;
    matrix(1, 0) = -sine;
    matrix(1, 1) =  cosine;
    matrix(2, 2) =  1;

    return matrix;
}

double
MaRC::Geometry::Magnitude(DVector const & vec)
{
    return MaRC::hypot(vec[0], vec[1], vec[2]);
}

void
MaRC::Geometry::toUnitVector(DVector & vec)
{
    double const mag = Geometry::Magnitude(vec);

    for(std::size_t i = 0; i < 3; ++i)
        vec[i] /= mag;
}
