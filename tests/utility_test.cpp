/**
 * @file utility_test.cpp
 *
 * Copyright (C) 2018 Ossama Othman
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

#include <marc/utility.h>

#include <vector>
#include <cassert>


/**
 * @test Test the MaRC::invert_samples() function.
 */
bool test_invert_samples()
{
    constexpr std::size_t samples = 5;
    constexpr std::size_t lines   = 2;

    std::vector<int> image { 1, 2, 3, 4, 5,
                             6, 7, 8, 9, 10 };

    std::vector<int> const inverted {  5, 4, 3, 2, 1,
                                       10, 9, 8, 7, 6 };

    assert(image != inverted);

    MaRC::invert_samples(image, samples, lines);

    return image == inverted;
}

/**
 * @test Test the MaRC::invert_lines() function.
 */
bool test_invert_lines()
{
    constexpr std::size_t samples = 2;
    constexpr std::size_t lines   = 5;

    std::vector<int> image { 1, 6,
                             2, 7,
                             3, 8,
                             4, 9,
                             5, 10 };

    std::vector<int> const inverted { 5, 10,
                                      4, 9,
                                      3, 8,
                                      2, 7,
                                      1, 6 };

    assert(image != inverted);

    MaRC::invert_lines(image, samples, lines);

    return image == inverted;
}

/// The canonical main entry point.
int main()
{
    return test_invert_samples() && test_invert_lines() ? 0 : -1;
}
