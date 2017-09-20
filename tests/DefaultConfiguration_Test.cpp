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

#include <MaRC/DefaultConfiguration.h>
#include <MaRC/Mathematics.h>


/**
 * @todo The built-in @c operator<=() and operator>=() for floating
 *       point may not be suitable to check for equality due to
 *       limitations in floating point types themselves.  We may want
 *       to use something like MaRC::almost_equal() instead.
 */

bool test_latitude_configuration()
{
    using namespace MaRC::default_configuration;

    // Largest latitude range that is valid is [-90, 90].

    return latitude_low   >= -90
        && latitude_high  <=  90
        && latitude_range <=  180;
}

bool test_longitude_configuration()
{
    using namespace MaRC::default_configuration;

    // Largest longitude ranges that are valid are [-180, 180] and
    // [0, 360].

    return longitude_low   >= -180
        && longitude_high  <=  360
        && longitude_range <=  360;
}

bool test_cosine_configuration()
{
    using namespace MaRC::default_configuration;

    // Largest cosine range that is valid is [-1, 1].

    return mu_low   >= -1
        && mu_high  <=  1

        && mu0_low   >= -1
        && mu0_high  <=  1

        && cos_phase_low   >= -1
        && cos_phase_high  <=  1;
}

int main()
{
    return
        test_latitude_configuration()
        && test_longitude_configuration()
        && test_cosine_configuration()
        ? 0 : -1;
}
