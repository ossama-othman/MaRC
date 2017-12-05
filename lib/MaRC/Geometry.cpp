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
#include "Mathematics.h"

#include <cmath>


void
MaRC::Geometry::RotX(double angle,
                     DVector const & v,
                     DVector & r)
{
    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // { 1,           0,          0 }   { vec[0] }
    // { 0,  cos(angle), sin(angle) } * { vec[1] }
    // { 0, -sin(angle), cos(angle} }   { vec[2] }

    r[0] =  v[0];
    r[1] =  v[1] * cosine + v[2] * sine;
    r[2] = -v[1] * sine   + v[2] * cosine;
}

void
MaRC::Geometry::RotY(double angle,
                     DVector const & v,
                     DVector & r)
{
    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // { cos(angle), 0, -sin(angle) }   { vec[0] }
    // { 0,          1,           0 } * { vec[1] }
    // { sin(angle), 0,  cos(angle} }   { vec[2] }

    r[0] = v[0] * cosine - v[2] * sine;
    r[1] = v[1];
    r[2] = v[0] * sine   + v[2] * cosine;
}

void
MaRC::Geometry::RotZ(double angle,
                     DVector const & v,
                     DVector &r)
{
    double const cosine = std::cos(angle);
    double const sine   = std::sin(angle);

    // {  cos(angle), sin(angle), 0 }   { vec[0] }
    // { -sin(angle), cos(angle}, 0 } * { vec[1] }
    // {  0,          0,          1 }   { vec[2] }

    r[0] =  v[0] * cosine + v[1] * sine;
    r[1] = -v[0] * sine   + v[1] * cosine;
    r[2] =  v[2];
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
