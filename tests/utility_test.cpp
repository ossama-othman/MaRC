/**
 * @file utility_test.cpp
 *
 * Copyright (C) 2018 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/utility.h>
#include <marc/config.h>  // For NDEBUG.

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
