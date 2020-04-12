/**
 * @file optional_test.cpp
 *
 * Copyright (C) 2018, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/optional.h>

#include <catch2/catch.hpp>

#include <string>
#include <type_traits>
#include <iostream>


/**
 * @test Test the MaRC::bad_optional_access exception.
 */
TEST_CASE("MaRC::bad_optional_access exception", "[optional]")
{
    constexpr MaRC::optional<int> no_value;

    REQUIRE_THROWS_AS((void) no_value.value(), MaRC::bad_optional_access);
}

/**
 * @test Test the MaRC::optional class.
 */
TEST_CASE("MaRC::optional behavior", "[optional]")
{
    constexpr MaRC::optional<int> no_value;
    constexpr int v1 = 62;
    constexpr int v2 = v1 + 1;
    static std::string const v3{"foo"};

    static_assert(v1 < v2, "v1 should be less than v2 for this test.");

    auto const ov1 = MaRC::make_optional(v1);
    auto const ov2 = MaRC::make_optional(v2);
    auto const ov3 = MaRC::make_optional(v3);

    auto ov4{ov1};
    MaRC::optional<decltype(ov4)::value_type> ov5;
    ov5.swap(ov4);

    auto ov6 = ov2;
    MaRC::optional<decltype(ov4)::value_type> ov7;
    std::swap(ov6, ov7);

    MaRC::optional<std::decay_t<decltype(v1)>> ov8{v1};
    ov8.reset();

    MaRC::optional<decltype(v2)> ov9 = v2;

    MaRC::optional<decltype(v1)> const ov10{MaRC::nullopt};
    MaRC::optional<decltype(v1)> const ov11 = MaRC::nullopt;

    REQUIRE(!no_value.has_value());
    REQUIRE(!no_value);
    REQUIRE(v1 == no_value.value_or(v1));
    REQUIRE(no_value != ov1);
    REQUIRE(no_value <   v1);
    REQUIRE(no_value <=  v1);
    REQUIRE(no_value <  ov1);
    REQUIRE(no_value <= ov1);

    REQUIRE(v1 == ov1.value());
    REQUIRE(v1 == *ov1);
    REQUIRE(v1 ==  ov1);
    REQUIRE(v1 <=  ov1);
    REQUIRE(v1 >=  ov1);
    REQUIRE(v1 !=  ov2);
    REQUIRE(ov2 != v1);
    REQUIRE(ov1 != ov2);

    REQUIRE(ov1 <  ov2);
    REQUIRE(ov1 <  v2);
    REQUIRE(v1  <  ov2);
    REQUIRE(ov1 <= ov2);
    REQUIRE(ov1 <= v2);
    REQUIRE(v1  <= ov2);

    REQUIRE(ov2 >  ov1);
    REQUIRE(ov2 >  v1);
    REQUIRE(v2  >  ov1);
    REQUIRE(ov2 >= ov1);
    REQUIRE(ov2 >= v1);
    REQUIRE(v2  >= ov1);

    REQUIRE(!ov3->empty());

    REQUIRE(!ov4);
    REQUIRE(ov5);
    REQUIRE(ov5 == ov1);
    REQUIRE(!ov6);
    REQUIRE(ov7);
    REQUIRE(ov7 == ov2);

    REQUIRE(!ov8);

    REQUIRE(ov9 == v2);

    REQUIRE(!ov10);
    REQUIRE(!ov11);
    REQUIRE(ov10 == MaRC::nullopt);
    REQUIRE(ov11 == MaRC::nullopt);
    REQUIRE(no_value == MaRC::nullopt);
    REQUIRE(MaRC::nullopt == no_value);
    REQUIRE(ov1 != MaRC::nullopt);
    REQUIRE(MaRC::nullopt != ov1);
    REQUIRE(!(ov1 <  MaRC::nullopt));
    REQUIRE(MaRC::nullopt <  ov1);
    REQUIRE(!(ov1 <= MaRC::nullopt));
    REQUIRE(MaRC::nullopt <= ov1);
    REQUIRE(ov1 >  MaRC::nullopt);
    REQUIRE(!(MaRC::nullopt >  ov1));
    REQUIRE(ov1 >= MaRC::nullopt);
    REQUIRE(!(MaRC::nullopt >= ov1));
}
