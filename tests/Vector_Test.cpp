/**
 * @file Vector_Test.cpp
 *
 * Copyright (C) 2017 Ossama Othman
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

#include <MaRC/Vector.h>


namespace
{
    // "Units in the last place" for floating point equality
    // comparison.
    constexpr int ulps = 4;
}


bool test_vector_initialization()
{
    static constexpr std::size_t ROWS = 3;
    using vector_type = MaRC::Vector<int, ROWS>;

    vector_type const v1;  // Default initialize all elements to 0.

    vector_type::value_type const n[] = { 2, 3, 5 };
    std::initializer_list<vector_type::value_type>
    vector_initializer_list{n[0], n[1], n[2]};

    vector_type const v2{vector_initializer_list};

    vector_type const v3{v2};

    vector_type const v4 = vector_initializer_list;

    vector_type const v5 = v3;

    std::iterator_traits<vector_type::iterator>::difference_type const
        row_count = std::distance(std::cbegin(v1), std::cend(v1));

    return
        row_count > 0
        && static_cast<std::remove_const<decltype(ROWS)>::type>(
            row_count) == ROWS
        && std::find_if_not(
            std::cbegin(v1),
            std::cend(v1),
            [](auto value)
            {
                return value == vector_type::value_type();
            }) == std::cend(v1)
        && std::equal(std::cbegin(v2), std::cend(v2),
                      std::cbegin(n),  std::cend(n))
        && std::equal(std::cbegin(v3), std::cend(v3),
                      std::cbegin(v2), std::cend(v2))
        && std::equal(std::cbegin(v4), std::cend(v4),
                      std::cbegin(n),  std::cend(n))
        && std::equal(std::cbegin(v5), std::cend(v5),
                      std::cbegin(v3), std::cend(v3));
}

bool test_vector_comparison()
{
    using vector_type = MaRC::Vector<int, 3>;

    vector_type const v1{{ 2, 3, 5 }};
    vector_type const v2{v1};
    vector_type const v3 = v2;
    vector_type const v4{{ 7, 11, 13 }};

    return v2 == v1 && v3 == v2 && v4 != v1;
}

bool test_vector_addition()
{
    using vector_type = MaRC::Vector<int, 3>;

    vector_type const v1 {{  2, 3, 5 }};
    vector_type const v2 {{ -1, 4, 0 }};
    vector_type v3{v1};
    v3 += v2;

    vector_type const sum{{ 1, 7, 5 }};

    return v3 == sum && v1 + v2 == sum;
}

bool test_vector_subtraction()
{
    using vector_type = MaRC::Vector<int, 3>;

    vector_type const v1 {{  2, 3, 5 }};
    vector_type const v2 {{ -1, 4, 0 }};
    vector_type v3{v1};
    v3 -= v2;

    vector_type const diff{{ 3, -1, 5 }};

    return v3 == diff && v1 - v2 == diff;
}

bool test_vector_multiplication()
{
    using vector_type = MaRC::Vector<int, 3>;

    vector_type const v1 {{ 2, 3,  5 }};

    vector_type::value_type s = 2;

    vector_type v2{v1};
    v2 *= s;

    vector_type const prod {{ 4, 6, 10 }};

    return v2 == prod && v1 * s == prod && s * v1 == prod;
}


bool test_vector_magnitude()
{
    using vector_type = MaRC::Vector<int, 3>;
    vector_type const v{ 3, 4, 5 };

    double const mag =
        std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    return MaRC::almost_equal(v.magnitude(), mag, ulps);
}

bool test_unit_vector()
{
    using vector_type = MaRC::Vector<double, 3>;
    vector_type v{ 3, 4, 5 };
    v.to_unit_vector();

    // Unit vector magnitude is always 1.
    constexpr double unit_mag = 1;

    return
           std::abs(v[0]) <= unit_mag
        && std::abs(v[1]) <= unit_mag
        && std::abs(v[2]) <= unit_mag
        && MaRC::almost_equal(v.magnitude(),
                              unit_mag,
                              ulps);
}

bool test_dot_product()
{
    using vector_type = MaRC::Vector<int, 3>;

    vector_type a{ 2, 3, 5   };
    vector_type b{ 7, 11, 13 };

    auto const dp = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];

    return MaRC::dot_product(a, b) == dp;
}

int main()
{
    return
        test_vector_initialization()
        && test_vector_comparison()
        && test_vector_addition()
        && test_vector_subtraction()
        && test_vector_magnitude()
        && test_unit_vector()
        && test_dot_product()
        ? 0 : -1;
}
