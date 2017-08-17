/**
 * @file OblateSpheroid_Test.cpp
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

#include <MaRC/OblateSpheroid.h>
#include <MaRC/Mathematics.h>
#include <MaRC/Constants.h>

#include <memory>
#include <cmath>


namespace
{
    // Jupiter
    constexpr bool prograde = true;        // Prograde rotation
    constexpr double a      = 71492;       // Equatorial radius
    constexpr double c      = 66854;       // Polar radius
    constexpr double f      = (a - c) / a; // Flattening

    // "Units in the last place" for floating point equality
    // comparison.
    constexpr int ulps = 2;
}

bool test_initialization()
{
    static_assert (a >= c,
                   "Test equatorial radius less than polar radius");

    // Equatorial and polar radii given.
    auto o1 =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               c,
                                               -1 /* flattening*/);

    // Equatorial radius and flattening given.
    auto o2 =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               -1,  // polar radius
                                               f);

    // Polar radius and flattening given.
    auto o3 =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               -1,  // equatorial radius
                                               c,
                                               f);

    constexpr int ulps = 2;

    return
        prograde == o1->prograde()

        && MaRC::almost_equal(a, o1->eq_rad(),     ulps)
        && MaRC::almost_equal(c, o1->pol_rad(),    ulps)
        && MaRC::almost_equal(f, o1->flattening(), ulps)

        && MaRC::almost_equal(a, o2->eq_rad(),     ulps)
        && MaRC::almost_equal(c, o2->pol_rad(),    ulps)
        && MaRC::almost_equal(f, o2->flattening(), ulps)

        && MaRC::almost_equal(a, o3->eq_rad(),     ulps)
        && MaRC::almost_equal(c, o3->pol_rad(),    ulps)
        && MaRC::almost_equal(f, o3->flattening(), ulps);
}

bool test_centric_radius()
{
    auto o =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               c,
                                               -1 /* flattening*/);

    constexpr double equator    =  0;               // radians
    constexpr double north_pole =  90 * C::degree;  //  pi/2
    constexpr double south_pole = -90 * C::degree;  // -pi/2
    constexpr double latitude   =  73 * C::degree;  // arbitrary

    double const r = o->centric_radius(latitude);

    // Parametric equation for an oblate spheroid disregarding the
    // 'y' component.
    double const x = r * std::cos(latitude);
    double const y = 0;
    double const z = r * std::sin(latitude);

    return
        // Centric radius, e.g. planetocentric, is the same as the
        // equatorial radius at the equator (latitude 0).
            MaRC::almost_equal(a, o->centric_radius(equator),    ulps)
        && !MaRC::almost_equal(a, o->centric_radius(north_pole), ulps)
        && !MaRC::almost_equal(a, o->centric_radius(south_pole), ulps)

        // Centric radius is the same as the polar radius at the poles
        // (latitudes -90 and 90).
        &&  MaRC::almost_equal(c, o->centric_radius(north_pole), ulps)
        &&  MaRC::almost_equal(c, o->centric_radius(south_pole), ulps)
        && !MaRC::almost_equal(c, o->centric_radius(equator),    ulps)

        && r < a
        && r > c

        /*
          Cartesian equation for an oblate spheroid.

           2    2    2
          x  + y    z
          ------- + -- = 1
             2       2
            a       c
        */

        && MaRC::almost_equal((x * x + y * y) / (a * a)
                              + (z * z) / (c * c), 1.0, ulps);
}

bool test_latitudes()
{
    /**
     * @todo Test centric_latitude() and graphic_latitude() members.
     */

    return true;
}

int main()
{
    return
        test_initialization()
        && test_centric_radius()
        && test_latitudes()
        ? 0 : -1;
}
