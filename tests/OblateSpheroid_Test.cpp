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

    // We require that the equatorial radius is always larger than the
    // polar radius for all the test cases.
    static_assert (a > c,
                   "Test equatorial radius less than or equal "
                   "to polar radius");
}

bool test_initialization()
{
    // Equatorial and polar radii given.
    auto const o1 =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               c,
                                               -1 /* flattening*/);

    // Equatorial radius and flattening given.
    auto const o2 =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               -1,  // polar radius
                                               f);

    // Polar radius and flattening given.
    auto const o3 =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               -1,  // equatorial radius
                                               c,
                                               f);


    // Insufficient number of radii or flattening provided.
    try {
        auto const o4 =
            std::make_unique<MaRC::OblateSpheroid>(prograde,
                                                   -1,  // equatorial radius
                                                   -1,  // polar radius
                                                   f);

        // The OblateSpheroid constructor should have thrown an
        // exception!
        return false;
    } catch(std::invalid_argument &) {
        // Expected exception thrown.
    }

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
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               c,
                                               -1 /* flattening*/);

    constexpr double equator    =  0;               // radians
    constexpr double north_pole =  90 * C::degree;  //  pi/2
    constexpr double south_pole = -90 * C::degree;  // -pi/2
    constexpr double latitude   = -23 * C::degree;  // arbitrary
    constexpr double longitude  =  0;               // no 'y' component

    double const r = o->centric_radius(latitude);

    // Polar coordinate based equation for a spheroid.
    double const x = r * std::cos(latitude) * std::cos(longitude);
    double const y = r * std::cos(latitude) * std::sin(longitude);;
    double const z = r * std::sin(latitude);

    return
        // Oblate spheroid sanity checks.
           r <= a
        && r >= c

        // Centric radius, e.g. planetocentric, is the same as the
        // equatorial radius at the equator (latitude 0).
        &&  MaRC::almost_equal(a, o->centric_radius(equator),    ulps)
        && !MaRC::almost_equal(a, o->centric_radius(north_pole), ulps)
        && !MaRC::almost_equal(a, o->centric_radius(south_pole), ulps)

        // Centric radius is the same as the polar radius at the poles
        // (latitudes -90 and 90).
        &&  MaRC::almost_equal(c, o->centric_radius(north_pole), ulps)
        &&  MaRC::almost_equal(c, o->centric_radius(south_pole), ulps)
        && !MaRC::almost_equal(c, o->centric_radius(equator),    ulps)

        && MaRC::almost_equal(r, MaRC::hypot(x, y, z), ulps);
}

bool test_latitudes()
{
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               c,
                                               -1 /* flattening*/);

    // Arbitrary latitude that isn't the equator or a pole.
    constexpr double latc = 27 * C::degree;

    /**
     * @todo Prior to running the below latitude tests, test
     *       centric_latitude() and graphic_latitude() methods
     *       independently of each other.
     */

    double const latg = o->graphic_latitude(latc);

    // For oblate spheroid latitudes not at the equator or the
    // poles, graphic latitudes will always be greater than the
    // centric latitude counterparts.   They will also have the same
    // sign.
    return
        MaRC::signum(latc) == MaRC::signum(latg)  // Same sign
        && std::abs(latc) < std::abs(latg)  // True if not equator or pole
        && !MaRC::almost_equal(latc, latg, ulps)
        &&  MaRC::almost_equal(latc,
                               o->centric_latitude(latg),
                               ulps);
}

bool test_mu()
{
    /**
     * @todo Test @c OblateSpheroid::mu() method.
     */
#if 0
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde,
                                               a,
                                               c,
                                               -1 /* flattening*/);

    constexpr double sub_observ_lat = 42  * C::degree;
    constexpr double sub_observ_lon = 247 * C::degree;
#endif

    return true;
}

bool test_mu0()
{
    /**
     * @todo Test @c OblateSpheroid::mu0() method.
     */

    return true;
}

bool test_cos_phase()
{
    /**
     * @todo Test @c OblateSpheroid::cos_phase() method.
     */

    return true;
}

int main()
{
    return
        test_initialization()
        && test_centric_radius()
        && test_latitudes()
        && test_mu()
        && test_mu0()
        && test_cos_phase()
        ? 0 : -1;
}
