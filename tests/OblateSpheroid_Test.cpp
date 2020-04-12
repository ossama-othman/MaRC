/**
 * @file OblateSpheroid_Test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/OblateSpheroid.h>
#include <marc/Mathematics.h>
#include <marc/Constants.h>

#include <catch2/catch.hpp>

#include <memory>
#include <cmath>


namespace
{
    // Jupiter
    constexpr bool prograde = true;        // Prograde rotation
    constexpr double a      = 71492;       // Equatorial radius
    constexpr double c      = 66854;       // Polar radius

    // "Units in the last place" for floating point equality
    // comparison.
    constexpr int ulps = 2;

    // We require that the equatorial radius is always larger than the
    // polar radius for all the test cases.
    static_assert (a > c,
                   "Test equatorial radius less than or equal "
                   "to polar radius");

#ifdef MARC_USE_CROSS_PRODUCT
    void
    cross_product(MaRC::DVector const & u,
                  MaRC::DVector const & v,
                  MaRC::DVector & cp)
    {
        cp[0] = u[1] * v[2] - u[2] * v[1];
        cp[1] = u[0] * v[2] - u[2] * v[0];
        cp[2] = u[0] * v[1] - u[1] * v[0];
    }
#endif

    double cos_included_angle(MaRC::DVector const & u,
                              MaRC::DVector const & v)
    {
#ifdef MARC_USE_CROSS_PRODUCT
        /**
         * @todo There doesn't appear to be a real need to use this
         *       approach in %MaRC since we only need to know the
         *       cosine of the angle between the vectors, not the
         *       angle itself.  Confirm.
         */
        MaRC::DVector cp;
        cross_product(u, v, cp);

        // Obtaining the angle, theta, between two vectors using the
        // dot product based approach, i.e.:
        //
        //     acos(dot_product(u,v)/(norm(u)*norm(v)))
        //
        // is inaccurate when the angle is small.  Leverage the sine
        // and cosine of the angle via the cross and dot products as
        // arguments to the atan2() function, respectively, to get a
        // better behaved result when the angle is very small:
        //
        //   atan2(norm(cross(u,v)), dot(u,v))
        double const theta =
            std::atan2(cp.magnitude(), MaRC::dot_product(u, v));

        return std::cos(theta);
#else
        return MaRC::dot_product(u, v) / (u.magnitude() * v.magnitude());
#endif
    }
}

/**
 * @test Test MaRC::OblateSpheroid initialization.
 */
TEST_CASE("MaRC::OblateSpheroid initialization", "[oblate spheroid]")
{
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde, a, c);

    // Expected first eccentricity
    double const e = std::sqrt(1 - std::pow(c / a, 2));

    REQUIRE(prograde == o->prograde());
    REQUIRE(MaRC::almost_equal(a, o->eq_rad(),  ulps));
    REQUIRE(MaRC::almost_equal(c, o->pol_rad(), ulps));
    REQUIRE(MaRC::almost_equal(e, o->first_eccentricity(), ulps));
}

/**
 * @test Test the MaRC::OblateSpheroid::centric_radius() method,
 *       i.e. the radius at a given planetocentric latitude.
 */
TEST_CASE("MaRC::OblateSpheroid::centric_radius()", "[oblate spheroid]")
{
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde, a, c);

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

    // Oblate spheroid sanity checks.
    REQUIRE(r <= a);
    REQUIRE(r >= c);

    // Centric radius, e.g. planetocentric, is the same as the
    // equatorial radius at the equator (latitude 0).
    REQUIRE( MaRC::almost_equal(a, o->centric_radius(equator),    ulps));
    REQUIRE(!MaRC::almost_equal(a, o->centric_radius(north_pole), ulps));
    REQUIRE(!MaRC::almost_equal(a, o->centric_radius(south_pole), ulps));

    // Centric radius is the same as the polar radius at the poles
    // (latitudes -90 and 90).
    REQUIRE( MaRC::almost_equal(c, o->centric_radius(north_pole), ulps));
    REQUIRE( MaRC::almost_equal(c, o->centric_radius(south_pole), ulps));
    REQUIRE(!MaRC::almost_equal(c, o->centric_radius(equator),    ulps));

    REQUIRE(MaRC::almost_equal(r, MaRC::hypot(x, y, z), ulps));
}

/**
 * @test Test calculations of planetocentric and planetographic
 *       latitudes in MaRC::OblateSpheroid.
 */
TEST_CASE("MaRC::OblateSpheroid latitudes", "[oblate spheroid]")
{
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde, a, c);

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

    REQUIRE(MaRC::signum(latc) == MaRC::signum(latg));  // Same sign
    REQUIRE(std::abs(latc) < std::abs(latg));  // True if not equator
                                               // or pole
    REQUIRE(!MaRC::almost_equal(latc, latg, ulps));
    REQUIRE( MaRC::almost_equal(latc,
                                o->centric_latitude(latg),
                                ulps));
}

/**
 * @test Test cosine of emission angle (&mu;) calculations in
 *       MaRC::OblateSpheroid.
 */
TEST_CASE("MaRC::OblateSpheroid::mu()", "[oblate spheroid]")
{
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde, a, c);

    constexpr double sub_observ_lat = 42  * C::degree;
    constexpr double sub_observ_lon = 247 * C::degree;
    constexpr double lat            = -75  * C::degree;
    constexpr double lon            = 185 * C::degree;
    constexpr double range          = a * 200;  // Multiple of
                                                // equatorial radius.

    // Cosine of the emission angle.
    double const mu = o->mu(sub_observ_lat,
                            sub_observ_lon,
                            lat,
                            lon,
                            range);

    /**
     * @todo Refactor vector calculations in @c mu(),
     *       @c mu0() and @c cos_phase() method test cases.
     */

    // Vector from the observer (e.g. spacecraft) to the center of the
    // oblate spheroid (e.g. planet) in body coordindates,
    MaRC::DVector const ro =
        {
            range * cos(sub_observ_lat) * cos(sub_observ_lon),
            range * cos(sub_observ_lat) * sin(sub_observ_lon),
            range * sin(sub_observ_lat)
        };

    double const radius = o->centric_radius(lat);

    // Vector from the center of the oblate spheroid to the point on
    // the surface at the given latitude and longitude.
    MaRC::DVector const re = { radius * cos(lat) * cos(lon),
                               radius * cos(lat) * sin(lon),
                               radius * sin(lat) };

    // Vector from the observer to the point on the surface at the
    // given latitude longitude, e.g. the camera optical axis.
    MaRC::DVector const rc = ro - re;

    double const a2 = a * a;
    double const c2 = c * c;

    // Normal vector at (lat, lon) (i.e. grad(f(x, y, z))).
    MaRC::DVector const rn = { 2 * re[0] / a2,
                               2 * re[1] / a2,
                               2 * re[2] / c2 };

    // Cosine of angle between the "camera vector" and the vector
    // normal to the surface at (lat, lon), the emission angle in
    // this case.
    double const mu_2 = cos_included_angle(rc, rn);

    REQUIRE(MaRC::almost_equal(mu, mu_2, ulps));
}

/**
 * @test Test cosine of incidence angle (&mu;<SUB>0</SUB>)
 *       calculations in MaRC::OblateSpheroid.
 */
TEST_CASE("MaRC::OblateSpheroid::mu0()", "[oblate spheroid]")
{
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde, a, c);

    constexpr double sub_solar_lat = -65 * C::degree;
    constexpr double sub_solar_lon = 135 * C::degree;
    constexpr double lat           =  47 * C::degree;
    constexpr double lon           = 330 * C::degree;

    // Cosine of the incidence angle.
    double const mu0 = o->mu0(sub_solar_lat,
                              sub_solar_lon,
                              lat,
                              lon);

    // We assume the Sun is an infinite distance away from the body.

    // Unit vector from the Sun to the center of the oblate spheroid
    // (e.g. planet) in body coordindates.
    MaRC::DVector const rs =
        {
            cos(sub_solar_lat) * cos(sub_solar_lon),
            cos(sub_solar_lat) * sin(sub_solar_lon),
            sin(sub_solar_lat)
        };

    double const radius = o->centric_radius(lat);

    // Vector from the center of the oblate spheroid to the point on
    // the surface at the given latitude and longitude.
    MaRC::DVector const re = { radius * cos(lat) * cos(lon),
                               radius * cos(lat) * sin(lon),
                               radius * sin(lat) };

    double const a2 = a * a;
    double const c2 = c * c;

    // Normal vector at (lat, lon) (i.e. grad(f(x, y, z))).
    MaRC::DVector const rn = { 2 * re[0] / a2,
                               2 * re[1] / a2,
                               2 * re[2] / c2 };

    // Cosine of angle between the Sun and the vector normal to the
    // surface at (lat, lon), the incidence angle in this case.
    double const mu0_2 = cos_included_angle(rs, rn);

    REQUIRE(MaRC::almost_equal(mu0, mu0_2, ulps));
}

/**
 * @test Test cosine of phase angle (cos(&phi;)) calculations in
 *       MaRC::OblateSpheroid.
 */
TEST_CASE("MaRC::OblateSpheroid cos_phase()", "[oblate spheroid]")
{
    auto const o =
        std::make_unique<MaRC::OblateSpheroid>(prograde, a, c);

    constexpr double sub_observ_lat = -67 * C::degree;
    constexpr double sub_observ_lon =  15 * C::degree;
    constexpr double sub_solar_lat  =  31 * C::degree;
    constexpr double sub_solar_lon  = 198 * C::degree;
    constexpr double lat            =  29 * C::degree;
    constexpr double lon            = 330 * C::degree;
    constexpr double range          = a * 300;  // Multiple of
                                                // equatorial radius.

    // Cosine of the phase angle (cos(phi)).
    double const cos_phase = o->cos_phase(sub_observ_lat,
                                          sub_observ_lon,
                                          sub_solar_lat,
                                          sub_solar_lon,
                                          lat,
                                          lon,
                                          range);

    // Vector from the observer (e.g. spacecraft) to the center of the
    // oblate spheroid (e.g. planet) in body coordindates,
    MaRC::DVector const ro =
        {
            range * cos(sub_observ_lat) * cos(sub_observ_lon),
            range * cos(sub_observ_lat) * sin(sub_observ_lon),
            range * sin(sub_observ_lat)
        };

    double const radius = o->centric_radius(lat);

    // Vector from the center of the oblate spheroid to the point on
    // the surface at the given latitude and longitude.
    MaRC::DVector const re = { radius * cos(lat) * cos(lon),
                               radius * cos(lat) * sin(lon),
                               radius * sin(lat) };

    // Vector from the observer to the point on the surface at the
    // given latitude longitude, e.g. the camera optical axis.
    MaRC::DVector const rc = ro - re;

    // We assume the Sun is an infinite distance away from the body.

    // Unit vector from the Sun to the center of the oblate spheroid
    // (e.g. planet) in body coordindates,
    MaRC::DVector const rs =
        {
            cos(sub_solar_lat) * cos(sub_solar_lon),
            cos(sub_solar_lat) * sin(sub_solar_lon),
            sin(sub_solar_lat)
        };

    // Cosine of angle between the Sun and the vector from the
    // observer to the point on the surface at (lat, lon), the phase
    // angle in this case.
    double const cos_phase_2 = cos_included_angle(rc, rs);

    REQUIRE(MaRC::almost_equal(cos_phase, cos_phase_2, ulps));
}
