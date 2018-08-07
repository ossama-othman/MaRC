/**
 * @file optional_test.cpp
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

#include <MaRC/optional.h>

#include <string>
#include <type_traits>
#include <iostream>


bool test_bad_optional_access()
{
    try {
        constexpr MaRC::optional<int> no_value;

        (void) no_value.value();

        return false;  // Exception should have been thrown!
    } catch(MaRC::bad_optional_access const & e) {
        std::cout << e.what() << " thrown as expected.\n";
    }

    return true;
}

bool test_optional()
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

    return
        !no_value.has_value() && !no_value && v1 == no_value.value_or(v1)
        && no_value != ov1
        && no_value <   v1 && no_value <=  v1
        && no_value <  ov1 && no_value <= ov1

        && v1 == ov1.value() &&  v1 == *ov1 &&  v1 == ov1
        && v1 <= ov1         &&  v1 >=  ov1
        && v1 != ov2         && ov2 !=   v1 && ov1 != ov2


        && ov1 <  ov2 && ov1 <  v2 && v1 <  ov2
        && ov1 <= ov2 && ov1 <= v2 && v1 <= ov2

        && ov2 >  ov1 && ov2 >  v1 && v2 >  ov1
        && ov2 >= ov1 && ov2 >= v1 && v2 >= ov1

        && !ov3->empty()

        && !ov4 && ov5 && ov5 == ov1
        && !ov6 && ov7 && ov7 == ov2

        && !ov8

        && ov9 == v2

        && !ov10 && !ov11
        && ov10 == MaRC::nullopt && ov11 == MaRC::nullopt
        && no_value == MaRC::nullopt && MaRC::nullopt == no_value
        &&   ov1 != MaRC::nullopt  &&   MaRC::nullopt != ov1
        && !(ov1 <  MaRC::nullopt) &&   MaRC::nullopt <  ov1
        && !(ov1 <= MaRC::nullopt) &&   MaRC::nullopt <= ov1
        &&   ov1 >  MaRC::nullopt  && !(MaRC::nullopt >  ov1)
        &&   ov1 >= MaRC::nullopt  && !(MaRC::nullopt >= ov1);
}

int main()
{
    return test_bad_optional_access() && test_optional() ? 0 : -1;
}