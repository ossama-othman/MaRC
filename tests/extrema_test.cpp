/**
 * @file extrema_test.cpp
 *
 * Copyright (C) 2019, 2024  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/extrema.h>
#include <marc/config.h>  // For NDEBUG.

#include <limits>
#include <cassert>

namespace{
    /// Primary extremum type alias used by the overall test.
    using extremum_type = int;

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
}

/**
 * @test Test the MaRC::extrema<> handling of bad values.
 */
template <typename T>
bool test_bad_extrema(T minimum, T maximum)
{
    try {
        MaRC::extrema<int> e(minimum, maximum);
        (void) e; // Unused, and should not be reached!
        throw std::logic_error("extreama Constructor didn't throw.");
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
    MaRC::extrema<extremum_type> e1;
    MaRC::extrema<extremum_type> e2(good_min, good_max);
    MaRC::extrema<extremum_type> e3(d_min, d_max);
    MaRC::extrema<extremum_type> e4(good_min, good_min);

    // Copy construction and copy assignment.
    MaRC::extrema<extremum_type> e5(e2);
    MaRC::extrema<extremum_type> e6 = e2;

    // Converting copy construction and copy assignment.
    MaRC::extrema<double> e7(d_min, d_max);
    MaRC::extrema<extremum_type> e8(e7);
    MaRC::extrema<extremum_type> e9 = e7;

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
        && e4.minimum() == e4.maximum()

        && e5.minimum() == e2.minimum()
        && e5.maximum() == e2.maximum()

        && e6.minimum() == e2.minimum()
        && e6.maximum() == e2.maximum()

        && e8.minimum() == std::numeric_limits<extremum_type>::lowest()
        && e8.maximum() == std::numeric_limits<extremum_type>::max()

        && e9.minimum() == std::numeric_limits<extremum_type>::lowest()
        && e9.maximum() == std::numeric_limits<extremum_type>::max();
}

/**
 * @test Test MaRC::make_extrema().
 */
bool test_make_extrema()
{
    auto const e1 = MaRC::make_extrema(good_min, good_max);
    auto const e2 = MaRC::make_extrema<extremum_type>(d_min, d_max);

    return e1.minimum() == good_min && e1.maximum() == good_max
        && e2.minimum() == std::numeric_limits<extremum_type>::lowest()
        && e2.maximum() == std::numeric_limits<extremum_type>::max();
}

/**
 * @test Test MaRC::extrema<>::in_range().
 */
bool test_in_range()
{
    auto const e = MaRC::make_extrema(good_min, good_max);

    // Use midpoint as in-range value, being careful to avoid integer
    // overflow.
    constexpr auto good = good_min + ((good_max / 2) - (good_min / 2));

    auto const bad1 = *e.minimum() - 1;
    auto const bad2 = *e.maximum() + 1;

    return e.in_range(good)
        && e.in_range(good_min)
        && e.in_range(good_max)
        && !e.in_range(bad1)
        && !e.in_range(bad2);
}

/**
 * @test Test the MaRC::extrema<> swap.
 */
bool test_swap()
{
    constexpr extremum_type min1 = good_min;
    constexpr extremum_type max1 = good_max;
    constexpr extremum_type min2 = 18;
    constexpr extremum_type max2 = 27;

    auto e1 = MaRC::make_extrema(min1, max1);
    auto e2 = MaRC::make_extrema(min2, max2);

    swap(e1, e2);

    return e1.minimum() == min2 && e1.maximum() == max2
        && e2.minimum() == min1 && e2.maximum() == max1;
}

/**
 * @test Test the MaRC::extrema<>::update() from datum.
 */
bool test_update(MaRC::extrema<extremum_type> & e,
                 extremum_type datum,
                 std::optional<extremum_type> expected_minimum,
                 std::optional<extremum_type> expected_maximum)
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
                 std::optional<extremum_type> expected_minimum,
                 std::optional<extremum_type> expected_maximum)
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
        && test_make_extrema()
        && test_in_range()
        && test_swap()
        && test_update()
        && test_reset()
        ? 0 : -1;
}
