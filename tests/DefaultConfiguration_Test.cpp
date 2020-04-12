/**
 * @file DefaultConfiguration_Test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/DefaultConfiguration.h>
#include <marc/Mathematics.h>

#include <catch2/catch.hpp>


namespace
{
    // "Units in the last place" for floating point equality
    // comparison.
    constexpr int ulps = 4;
}

/**
 * @todo The built-in @c operator<=() and operator>=() for floating
 *       point may not be suitable to check for equality due to
 *       limitations in floating point types themselves.  We may want
 *       to use something like MaRC::almost_equal() instead.
 */

/**
 * @test Test default latitude configuration.  The largest latitude
 *       range that is valid is [-90, 90].
 */
TEST_CASE("Default latitude range", "[default configuration]")
{
    using namespace MaRC::default_configuration;

    REQUIRE(latitude_low   >= -90);
    REQUIRE(latitude_high  <=  90);
    REQUIRE(latitude_range <=  180);
    REQUIRE(MaRC::almost_equal(latitude_range,
                               latitude_high - latitude_low,
                               ulps));
}

/**
 * @test Test default longitude configuration.  The largest longitude
 *       ranges that are valid are [-180, 180] and [0, 360].
 */
TEST_CASE("Default longitude range", "[default configuration]")
{
    using namespace MaRC::default_configuration;

    REQUIRE(longitude_low  >= -180);
    REQUIRE(longitude_low  <  longitude_high);
    REQUIRE(longitude_high <= longitude_low + 360);
    REQUIRE(longitude_range <= 360);
    REQUIRE(MaRC::almost_equal(longitude_range,
                               longitude_high - longitude_low,
                               ulps));
}

/**
 * @test Test default &mu;, &mu;<SUB>0</SUB> and cos(&phi;) range
 *       configuration, i.e. they must be all be in the range
 *       [-1, 1].
 */
TEST_CASE("Default cosines of emission, incidence and phase angles",
          "[default configuration]")
{
    using namespace MaRC::default_configuration;

    REQUIRE(mu_low         >= -1);
    REQUIRE(mu_high        <=  1);
    REQUIRE(mu0_low        >= -1);
    REQUIRE(mu0_high       <=  1);
    REQUIRE(cos_phase_low  >= -1);
    REQUIRE(cos_phase_high <=  1);
}
