/**
 * @file DefaultConfiguration_Test.cpp
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

#include <marc/DefaultConfiguration.h>
#include <marc/Mathematics.h>


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
bool test_latitude_configuration()
{
    using namespace MaRC::default_configuration;

    return latitude_low   >= -90
        && latitude_high  <=  90
        && latitude_range <=  180
        && MaRC::almost_equal(latitude_range,
                              latitude_high - latitude_low,
                              ulps);
}

/**
 * @test Test default longitude configuration.  The largest longitude
 *       ranges that are valid are [-180, 180] and [0, 360].
 */
bool test_longitude_configuration()
{
    using namespace MaRC::default_configuration;

    return longitude_low  >= -180
        && longitude_low  <  longitude_high
        && longitude_high <= longitude_low + 360
        && longitude_range <= 360
        && MaRC::almost_equal(longitude_range,
                              longitude_high - longitude_low,
                              ulps);
}

/**
 * @test Test default &mu;, &mu;<SUB>0</SUB> and cos(&phi;) range
 *       configuration, i.e. they must be all be in the range
 *       [-1, 1].
 */
bool test_cosine_configuration()
{
    using namespace MaRC::default_configuration;

    return mu_low   >= -1
        && mu_high  <=  1

        && mu0_low  >= -1
        && mu0_high <=  1

        && cos_phase_low   >= -1
        && cos_phase_high  <=  1;
}

/// The canonical main entry point.
int main()
{
    return
        test_latitude_configuration()
        && test_longitude_configuration()
        && test_cosine_configuration()
        ? 0 : -1;
}
