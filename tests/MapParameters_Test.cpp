/**
 * @file MapParameters_Test.cpp
 *
 * Copyright (C) 2019 Ossama Othman
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

#include "../src/MapParameters.h"

#include <marc/Mathematics.h>

#include <functional>
#include <cassert>

#include <fitsio.h>


namespace
{
    // "Units in the last place" for floating point equality
    // comparison.
    constexpr int ulps = 2;

    using comments_type = MaRC::MapParameters::comment_list_type;
}

bool test_initialization()
{
    MaRC::MapParameters a;

    constexpr int plane = 1;
    MaRC::MapParameters b(plane);

    try {
        constexpr int invalid_plane = -1;
        MaRC::MapParameters c(invalid_plane);
    } catch(std::invalid_argument &) {
        // Expected.

        return true;
    }

    return false;
}

#define MARC_TEST_STRING_PARAM(parameter)  \
bool test_ ## parameter()                  \
{                                          \
    MaRC::MapParameters p;                 \
    if (!p.parameter().empty())            \
        return false;                      \
    p.parameter(#parameter);               \
    return p.parameter() == #parameter;    \
}

#define MARC_TEST_REAL_PARAM(parameter)     \
bool test_ ## parameter()                   \
{                                           \
    MaRC::MapParameters p;                  \
    if (p.parameter().has_value())          \
        return false;                       \
    constexpr double x = __LINE__;          \
    p.parameter(x);                         \
    auto const y = p.parameter().value();   \
    return MaRC::almost_equal(x, y, ulps);  \
}

MARC_TEST_STRING_PARAM(author)
MARC_TEST_STRING_PARAM(bunit)
MARC_TEST_STRING_PARAM(instrument)
MARC_TEST_STRING_PARAM(object)
MARC_TEST_STRING_PARAM(observer)
MARC_TEST_STRING_PARAM(origin)
MARC_TEST_STRING_PARAM(reference)
MARC_TEST_STRING_PARAM(telescope)

MARC_TEST_REAL_PARAM(bscale)
MARC_TEST_REAL_PARAM(bzero)
MARC_TEST_REAL_PARAM(datamax)
MARC_TEST_REAL_PARAM(datamin)
MARC_TEST_REAL_PARAM(equinox)

bool test_bitpix()
{
    MaRC::MapParameters p;

    // Increasing integer BITPIX.
    p.bitpix(BYTE_IMG);
    if (p.bitpix() != BYTE_IMG)
        return false;

    p.bitpix(SHORT_IMG);
    if (p.bitpix() != SHORT_IMG)
        return false;

    p.bitpix(LONG_IMG);
    if (p.bitpix() != LONG_IMG)
        return false;

    p.bitpix(LONGLONG_IMG);
    if (p.bitpix() != LONGLONG_IMG)
        return false;

    // Increasing floating point BITPIX.
    p.bitpix(FLOAT_IMG);
    if (p.bitpix() != FLOAT_IMG)
        return false;

    p.bitpix(DOUBLE_IMG);
    if (p.bitpix() != DOUBLE_IMG)
        return false;

    try {
        constexpr int bad = -1;

        static_assert(bad != BYTE_IMG
                      && bad != SHORT_IMG
                      && bad != LONG_IMG
                      && bad != LONGLONG_IMG
                      && bad != FLOAT_IMG
                      && bad != DOUBLE_IMG,
                      "Bitpix value is unexpectedly valid.");

        p.bitpix(bad);
    } catch(std::invalid_argument &) {
        // Expected.

        return true;
    }

    return false;
}

bool test_blank()
{
    MaRC::MapParameters p;

    if (p.blank().has_value())
        return false;  // Should not be set yet!

    MaRC::blank_type b(__LINE__);
    p.blank(b);

    return p.blank() == b;
}

bool test_comments_impl(
    std::function<void(comments_type::value_type const &)> const & push,
    comments_type const & comments)
{
    if (!comments.empty())
        return false;

    using comment_type = comments_type::value_type;

    comment_type const to_push[] = { "Foo", "Bar" };

    for (auto const & c : to_push)
        push(c);

    return std::equal(std::begin(comments),
                      std::end(comments),
                      std::begin(to_push));
}

bool test_comments()
{
    MaRC::MapParameters p;

    auto const & push =
        [&p](comments_type::value_type const & c)
        {
            p.push_comment(c);
        };

    return test_comments_impl(push, p.comments());
}

bool test_xcomments()
{
    MaRC::MapParameters p;

    auto const & push =
        [&p](comments_type::value_type const & c)
        {
            p.push_xcomment(c);
        };

    return test_comments_impl(push, p.xcomments());
}

bool test_merge_bitpix()
{
    int plane = 1;

    MaRC::MapParameters u;
    MaRC::MapParameters p1(plane++);
    MaRC::MapParameters p2(plane);

    // ---------------------
    // Integer BITPIX Checks
    // ---------------------

    // User supplied BITPIX
    u.bitpix(BYTE_IMG);
    p1.bitpix(SHORT_IMG);

    assert(u.bitpix() < p1.bitpix());   // Sanity check.

    if (!u.merge(p1)
        || u.bitpix() != BYTE_IMG)   // Override should NOT have
                                     // occurred.
        return false;

    // Same integer BITPIX
    p1.bitpix(SHORT_IMG);
    p2.bitpix(p1.bitpix());

    assert(p1.bitpix() == p2.bitpix()); // Sanity check.

    if (!p1.merge(p2)
        || p1.bitpix() != SHORT_IMG)    // Override should NOT have
                                        // occurred.
        return false;

    // Increasing integer BITPIX
    p1.bitpix(SHORT_IMG);
    p2.bitpix(LONG_IMG);

    assert(p1.bitpix() < p2.bitpix());  // Sanity check.

    if (!p1.merge(p2)
        || p1.bitpix() != LONG_IMG)     // Override should have
                                        // occurred.
        return false;

    // Decreasing integer BITPIX
    p1.bitpix(LONG_IMG);
    p2.bitpix(SHORT_IMG);

    assert(p1.bitpix() > p2.bitpix());  // Sanity check.

    if (!p1.merge(p2)
        || p1.bitpix() != LONG_IMG)     // Override should NOT have
                                        // occurred.
        return false;

    // ----------------------------
    // Floating point BITPIX Checks
    // ----------------------------

    // Same floating point BITPIX
    p1.bitpix(FLOAT_IMG);
    p2.bitpix(p1.bitpix());

    assert(p1.bitpix() == p2.bitpix()); // Sanity check.

    if (!p1.merge(p2)
        || p1.bitpix() != FLOAT_IMG)    // Override should NOT have
                                        // occurred.
        return false;

    // Increasing floating point BITPIX
    p1.bitpix(FLOAT_IMG);
    p2.bitpix(DOUBLE_IMG);

    assert(p1.bitpix() > p2.bitpix());  // Sanity check.
                                        // FLOAT_IMG > DOUBLE_IMG

    if (!p1.merge(p2)
        || p1.bitpix() != DOUBLE_IMG)   // Override should have
                                        // occurred.
        return false;

    // Decreasing integer BITPIX
    p1.bitpix(DOUBLE_IMG);
    p2.bitpix(FLOAT_IMG);

    assert(p1.bitpix() < p2.bitpix());  // Sanity check.
                                        // FLOAT_IMG > DOUBLE_IMG

    if (!p1.merge(p2)
        || p1.bitpix() != DOUBLE_IMG)   // Override should NOT have
                                        // occurred.
        return false;

    // Override floating point BITPIX with integer BITPIX
    /**
     * @todo Verify that the integer @c BITPIX override of a floating
     *       point @c BITPIX is really needed and/or useful.
     */
    p1.bitpix(DOUBLE_IMG);
    p2.bitpix(SHORT_IMG);

    assert(p1.bitpix() < 0 && p2.bitpix() > 0);  // Sanity check.

    if (!p1.merge(p2)
        || p1.bitpix() != SHORT_IMG)    // Override should have
                                        // occurred.
        return false;

    return true;
}


bool test_merge()
{
    return test_merge_bitpix();
}

/// The canonical main entry point.
int main()
{
    return
        test_initialization()
        && test_author()
        && test_bitpix()
        && test_blank()
        && test_bscale()
        && test_bunit()
        && test_bzero()
        && test_datamax()
        && test_datamin()
        && test_equinox()
        && test_instrument()
        && test_object()
        && test_observer()
        && test_origin()
        && test_reference()
        && test_telescope()
        && test_comments()
        && test_xcomments()
        && test_merge()
        ? 0 : -1;
}
