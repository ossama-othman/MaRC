/**
 * @file MapParameters_Test.cpp
 *
 * Copyright (C) 2019-2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "../src/MapParameters.h"

#include <marc/Mathematics.h>
#include <marc/config.h>  // For NDEBUG.

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

#define MARC_TEST_STRING_PARAM(parameter)        \
bool test_ ## parameter(MaRC::MapParameters & p) \
{                                                \
    if (!p.parameter().empty())                  \
        return false;                            \
    p.parameter(#parameter);                     \
    return p.parameter() == #parameter;          \
}

#define MARC_TEST_REAL_PARAM(parameter)          \
bool test_ ## parameter(MaRC::MapParameters & p) \
{                                                \
    if (p.parameter().has_value())               \
        return false;                            \
    constexpr double x = __LINE__;               \
    p.parameter(x);                              \
    auto const y = p.parameter().value();        \
    return MaRC::almost_equal(x, y, ulps);       \
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

bool test_bitpix(MaRC::MapParameters & p)
{
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

bool test_blank(MaRC::MapParameters & p)
{
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

bool test_comments(MaRC::MapParameters & p)
{
    auto const & push =
        [&p](comments_type::value_type const & c)
        {
            p.push_comment(c);
        };

    return test_comments_impl(push, p.comments());
}

bool test_xcomments(MaRC::MapParameters & p)
{
    auto const & push =
        [&p](comments_type::value_type const & c)
        {
            p.push_xcomment(c);
        };

    return test_comments_impl(push, p.xcomments());
}

bool test_parameters()
{
    MaRC::MapParameters p;

    return test_author(p)
        && test_bitpix(p)
        && test_blank(p)
        && test_bscale(p)
        && test_bunit(p)
        && test_bzero(p)
        && test_datamax(p)
        && test_datamin(p)
        && test_equinox(p)
        && test_instrument(p)
        && test_object(p)
        && test_observer(p)
        && test_origin(p)
        && test_reference(p)
        && test_telescope(p)
        && test_comments(p)
        && test_xcomments(p);
}

#define MARC_TEST_MERGE_STRING(parameter)               \
bool test_merge_ ## parameter(MaRC::MapParameters & u,  \
                              MaRC::MapParameters & p1, \
                              MaRC::MapParameters & p2) \
{                                                       \
    constexpr char const uv[] = #parameter;             \
    constexpr char const pv[] = #parameter "_plane";    \
    u.parameter(uv);                                    \
    p2.parameter(pv);                                   \
    return p1.merge(p2) && p1.parameter() == pv         \
        && u.merge(p1)  && u.parameter()  == uv;        \
}

#define MARC_TEST_MERGE_REAL(parameter)                   \
bool test_merge_ ## parameter(MaRC::MapParameters & u,    \
                              MaRC::MapParameters & p1,   \
                              MaRC::MapParameters & p2)   \
{                                                         \
    constexpr double uv = __LINE__;                       \
    constexpr double pv = uv + 2;                         \
    assert(!MaRC::almost_equal(uv, pv, ulps));            \
    u.parameter(uv);                                      \
    p2.parameter(pv);                                     \
    return p1.merge(p2)                                   \
        && MaRC::almost_equal(*p1.parameter(), pv, ulps)  \
        && u.merge(p1)                                    \
        && MaRC::almost_equal(*u.parameter(), uv,  ulps); \
}

MARC_TEST_MERGE_STRING(author)
MARC_TEST_MERGE_STRING(bunit)
MARC_TEST_MERGE_STRING(instrument)
MARC_TEST_MERGE_STRING(object)
MARC_TEST_MERGE_STRING(observer)
MARC_TEST_MERGE_STRING(origin)
MARC_TEST_MERGE_STRING(reference)
MARC_TEST_MERGE_STRING(telescope)

MARC_TEST_MERGE_REAL(bscale)
MARC_TEST_MERGE_REAL(bzero)
MARC_TEST_MERGE_REAL(datamax)
MARC_TEST_MERGE_REAL(datamin)
MARC_TEST_MERGE_REAL(equinox)

bool test_merge_bitpix(MaRC::MapParameters & u,
                       MaRC::MapParameters & p1,
                       MaRC::MapParameters & p2)
{
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

    // Decreasing floating point BITPIX
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

bool test_merge_blank(MaRC::MapParameters & u,
                      MaRC::MapParameters & p1,
                      MaRC::MapParameters & p2)
{
    constexpr MaRC::blank_type uv(__LINE__);
    constexpr MaRC::blank_type pv(uv.value() * 2);
    u.blank(uv);
    p2.blank(pv);
    return p1.merge(p2) && p1.blank() == pv
        && u.merge(p1)  && u.blank()  == uv;
}

/*
bool test_merge_comments_impl(
    std::function<void(comments_type::value_type const &)> const & u_push,
    comments_type const & comments)
{
    if (!comments.empty())
        return false;

    using comment_type = comments_type::value_type;

    comment_type const p2c[] = { "p2: 1", "p2: 2" };

    assert(!u.comments().empty());
    assert(!p1.comments().empty());
    assert(!p2.comments().empty());

    // Pre-merge copies.
    auto u_comments  = u.comments();
    auto p1_comments = p1.comments();

    // Expected contents of p1 comment list after merge.
    for (auto const & c: p2c)
        p1_comments.push_back(c);

    // Add comments for merge testing.
    for (auto const & c : p2c)
        p2.push_comment(c);

    // Merge and test.
    return p1.merge(p2) && p1.comments() == p1_comments  // Updated
        && u.merge(p1) && u.comments() == user_comments; // No change
}
*/

bool test_merge_comments(MaRC::MapParameters & u,
                         MaRC::MapParameters & p1,
                         MaRC::MapParameters & p2)
{
    std::string const p2c[] = { "p2: 1", "p2: 2" };

    assert(!u.comments().empty());
    assert(!p1.comments().empty());
    assert(!p2.comments().empty());

    // Pre-merge copies.
    auto u_comments  = u.comments();
    auto p1_comments = p1.comments();

    // Expected contents of p1 comment list after merge.
    for (auto const & c: p2c)
        p1_comments.push_back(c);

    // Add comments for merge testing.
    for (auto const & c : p2c)
        p2.push_comment(c);

    // Merge and test.
    return p1.merge(p2) && p1.comments() == p1_comments  // Updated
        && u.merge(p1)  && u.comments()  == u_comments;  // No change
}

bool test_merge_xcomments(MaRC::MapParameters & u,
                          MaRC::MapParameters & p1,
                          MaRC::MapParameters & p2)
{
    std::string const p2c[] = { "p2: 1", "p2: 2" };

    assert(!u.xcomments().empty());
    assert(!p1.xcomments().empty());
    assert(!p2.xcomments().empty());

    // Pre-merge copies.
    auto u_comments  = u.xcomments();
    auto p1_comments = p1.xcomments();

    // Expected contents of p1 comment list after merge.
    for (auto const & c: p2c)
        p1_comments.push_back(c);

    // Add comments for merge testing.
    for (auto const & c : p2c)
        p2.push_xcomment(c);

    // Merge and test.
    return p1.merge(p2) && p1.xcomments() == p1_comments  // Updated
        && u.merge(p1)  && u.xcomments()  == u_comments;  // No change
}

bool test_merge()
{
    int plane = 1;

    MaRC::MapParameters u;
    MaRC::MapParameters p1(plane++);
    MaRC::MapParameters p2(plane);

    return test_merge_author(u, p1, p2)
        && test_merge_bitpix(u, p1, p2)
        && test_merge_blank(u, p1, p2)
        && test_merge_bscale(u, p1, p2)
        && test_merge_bunit(u, p1, p2)
        && test_merge_bzero(u, p1, p2)
        && test_merge_datamax(u, p1, p2)
        && test_merge_datamin(u, p1, p2)
        && test_merge_equinox(u, p1, p2)
        && test_merge_instrument(u, p1, p2)
        && test_merge_object(u, p1, p2)
        && test_merge_observer(u, p1, p2)
        && test_merge_origin(u, p1, p2)
        && test_merge_reference(u, p1, p2)
        && test_merge_telescope(u, p1, p2)
        && test_merge_comments(u, p1, p2)
        && test_merge_xcomments(u, p1, p2);
}

/// The canonical main entry point.
int main()
{
    return test_initialization()
        && test_parameters()
        && test_merge()
        ? 0 : -1;
}
