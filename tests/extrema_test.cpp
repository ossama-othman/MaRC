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
    /// Primary extremum type alias used by the overall test.
    using extremum_type = int;
}

/**
 * @test Test the MaRC::extrema<> handling of bad values.
 */
template <typename T>
bool test_bad_extrema(T minimum, T maximum)
{
    try {
        MaRC::extrema<int> e(minimum, maximum);
    } catch(std::invalid_argument &) {
        return true;
    } catch(...) {
        return false;
    }

    return false;
}

/**
 * @test Test the MaRC::extrema<> initialization.
 */
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
    MaRC::extrema<int> e4(good_min, good_min);

    return !e1.is_valid()
        && e2.is_valid()
        && e3.is_valid()
        && e4.is_valid()

        && test_bad_extrema(bad_min, bad_max)
        && test_bad_extrema(std::numeric_limits<double>::quiet_NaN(),
                            d_max)

        && e2.minimum() == good_min
        && e2.maximum() == good_max

        && e3.minimum() == std::numeric_limits<extremum_type>::lowest()
        && e3.maximum() == std::numeric_limits<extremum_type>::max()

        && e4.minimum() == good_min
        && e4.minimum() == e4.maximum();
}

/**
 * @test Test the MaRC::extrema<>::update() from datum.
 */
bool test_update(MaRC::extrema<extremum_type> & e,
                 extremum_type datum,
                 extremum_type expected_minimum,
                 extremum_type expected_maximum)
{
    e.update(datum);

    return e.is_valid()
        && e.minimum() == expected_minimum
        && e.maximum() == expected_maximum;
}

/**
 * @test Test the MaRC::extrema<>::update() from other extrema<>.
 */
bool test_update(MaRC::extrema<extremum_type> & to,
                 MaRC::extrema<extremum_type> const & from,
                 extremum_type expected_minimum,
                 extremum_type expected_maximum)
{
    to.update(from);

    return to.is_valid()
        && to.minimum() == expected_minimum
        && to.maximum() == expected_maximum;
}

/**
 * @test Test the MaRC::extrema<>::update() methods.
 */
bool test_update()
{
    constexpr extremum_type a = -1000;
    constexpr extremum_type b = -100;
    constexpr extremum_type c =  0;
    constexpr extremum_type d =  10;

    MaRC::extrema<extremum_type> e1;
    MaRC::extrema<extremum_type> e2(b, c);
    MaRC::extrema<extremum_type> e3(a, c);
    MaRC::extrema<extremum_type> e4(b, d);
    MaRC::extrema<extremum_type> e5(b, c);

    return test_update(e1, b, b, b)  // First update
        && test_update(e1, b, b, b)  // No change
        && test_update(e1, a, a, b)  // Minimum updated
        && test_update(e1, b, a, b)  // No change
        && test_update(e1, d, a, d)  // Maximum updated
        && test_update(e1, c, a, d)  // No change

        && test_update(e1,            // "a" and "d"
                       e1,            // "b" and "c"
                       e1.minimum(),  // No change
                       e1.maximum())  // No change
        && test_update(e1,            // "a" and "d"
                       e2,            // "b" and "c"
                       e1.minimum(),
                       e1.maximum())  // No change
        && test_update(e2,            // "b" and "c"
                       e3,            // "a" and "c"
                       e3.minimum(),
                       e2.maximum())  // e2 minimum updated
        && test_update(e3,            // "a" and "c"
                       e4,            // "b" and "d"
                       e3.minimum(),
                       e4.maximum())  // e3 maximum updated
        && test_update(e5,            // "b" and "c"
                       e3,            // "a" and "d"
                       e3.minimum(),
                       e3.maximum()); // e5 minimum and maximum updated
}

/**
 * @test Test the MaRC::extrema<>::reset() method.
 */
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
