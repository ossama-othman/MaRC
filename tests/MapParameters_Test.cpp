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

#include <fitsio.h>


namespace
{
    constexpr int ulps = 2;
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

    // Attempt to decrease integer BITPIX.
    p.bitpix(SHORT_IMG);
    if (p.bitpix() == SHORT_IMG)  // There should be no change.
        return false;

    // Increasing floating point BITPIX.
    p.bitpix(FLOAT_IMG);
    if (p.bitpix() != FLOAT_IMG)
        return false;

    p.bitpix(DOUBLE_IMG);
    if (p.bitpix() != DOUBLE_IMG)
        return false;

    // Attempt to decrease floating point BITPIX.
    p.bitpix(FLOAT_IMG);
    if (p.bitpix() == FLOAT_IMG)  // There should be no change.
        return false;

    // Attempt to decrease floating point to integer BITPIX.
    p.bitpix(SHORT_IMG);
    if (p.bitpix() != SHORT_IMG)  // Integer should have overriden.
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

bool test_comments()
{
    MaRC::MapParameters p;

    if (!p.comments().empty())
        return false;

    using comment_type =
        MaRC::MapParameters::comment_list_type::value_type;

    comment_type const to_push[] = { "Foo", "Bar" };

    for (auto const & c : to_push)
        p.push_comment(c);

    auto const & comments = p.comments();

    return std::equal(std::begin(comments),
                      std::end(comments),
                      std::begin(to_push));
}

bool test_xcomments()
{
    MaRC::MapParameters p;

    if (!p.xcomments().empty())
        return false;

    using comment_type =
        MaRC::MapParameters::comment_list_type::value_type;

    comment_type const to_push[] = { "Foo", "Bar" };

    for (auto const & c : to_push)
        p.push_xcomment(c);

    auto const & xcomments = p.xcomments();

    return std::equal(std::begin(xcomments),
                      std::end(xcomments),
                      std::begin(to_push));
}

bool test_merge()
{
    /**
     * @todo Implement this test.
     */

    return true;
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
