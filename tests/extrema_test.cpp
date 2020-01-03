/**
 * @file extrema_test.cpp
 *
 * Copyright (C) 2019 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/extrema.h>
#include <marc/config.h>  // For NDEBUG.

#include <cassert>


namespace
{
    using extremum_type = int;
}

template <typename T>
bool test_bad_extrema(T minimum, T maximum)
{
    try {
        MaRC::extrema<int> e(minimum, maximum);

        return !e.is_valid();  // Unreached
    } catch(std::invalid_argument &) {
        return true;
    } catch(...) {
        return false;
    }

    return false;
}

bool test_initialization()
{
    constexpr extremum_type good_min = -1;
    constexpr extremum_type good_max = 42;

    constexpr auto bad_min = good_max;
    constexpr auto bad_max = good_min;

    // For testing extrema clipping.
    constexpr auto d_min = std::numeric_limits<double>::lowest();
    constexpr auto d_max = std::numeric_limits<double>::max();

    static_assert(d_min < std::numeric_limits<extremum_type>::lowest()
                  && d_max > std::numeric_limits<extremum_type>::max(),
                  "Incorrect values for extrema clipping test.");

    MaRC::extrema<int> e1;
    MaRC::extrema<int> e2(good_min, good_max);
    MaRC::extrema<int> e3(d_min, d_max);

    return !e1.is_valid()
        && e2.is_valid()
        && e3.is_valid()

        && test_bad_extrema(bad_min, bad_max)
        && test_bad_extrema(std::numeric_limits<double>::quiet_NaN(),
                            d_max)

        && e3.minimum() == std::numeric_limits<extremum_type>::lowest()
        && e3.maximum() == std::numeric_limits<extremum_type>::max();
}

bool test_update()
{
    constexpr extremum_type a = -1000;
    constexpr extremum_type b = -100;
    constexpr extremum_type c = a;

    MaRC::extrema<extremum_type> e1(a, b);
    MaRC::extrema<extremum_type> e2(a, c);

    return e1.is_valid()
        && e2.is_valid()

        && e1.minimum() == a
        && e1.maximum() == b
        && e2.minimum() == e2.maximum();
}

bool test_reset()
{
    constexpr extremum_type a = -1000;
    constexpr extremum_type b = 0;

    MaRC::extrema<extremum_type> e(a, b);

    assert(e.is_valid());

    e.reset();

    return !e.is_valid();
}


/// The canonical main entry point.
int main()
{
    return test_initialization()
        && test_update()
        && test_reset()
        ? 0 : -1;
}
