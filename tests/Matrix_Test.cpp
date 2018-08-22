/**
 * @file Matrix_Test.cpp
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
#include <MaRC/Matrix.h>

#include <algorithm>


bool test_matrix_initialization()
{
    static constexpr std::size_t ROWS = 2;
    static constexpr std::size_t COLUMNS = 2;
    using matrix_type = MaRC::Matrix<int, ROWS, COLUMNS>;

    matrix_type const m1;     // Default initialize all elements to 0.

    matrix_type::value_type const n[] = {2, 3, 5, 7};
    std::initializer_list<std::initializer_list<matrix_type::value_type>>
    matrix_initializer_list{{n[0], n[1]},
                            {n[2], n[3]}};

    matrix_type const m2{matrix_initializer_list};

    matrix_type const m3{m2};

    matrix_type const m4 = matrix_initializer_list;

    matrix_type const m5 = m3;

    std::iterator_traits<matrix_type::iterator>::difference_type const
        element_count = std::distance(std::cbegin(m1), std::cend(m1));

    return
        element_count > 0
        && static_cast<std::remove_const<decltype(ROWS)>::type>(
            element_count) == ROWS * COLUMNS
        && std::find_if_not(
            std::cbegin(m1),
            std::cend(m1),
            [](auto value)
            {
                return value == matrix_type::value_type();
            }) == std::cend(m1)
        && std::equal(std::cbegin(m2), std::cend(m2),
                      std::cbegin(n),  std::cend(n))
        && std::equal(std::cbegin(m3), std::cend(m3),
                      std::cbegin(m2), std::cend(m2))
        && std::equal(std::cbegin(m4), std::cend(m4),
                      std::cbegin(n),  std::cend(n))
        && std::equal(std::cbegin(m5), std::cend(m5),
                      std::cbegin(m3), std::cend(m3));
}

bool test_matrix_element_access()
{
    static constexpr std::size_t ROWS = 2;
    static constexpr std::size_t COLUMNS = 2;
    using matrix_type = MaRC::Matrix<int, ROWS, COLUMNS>;

    static_assert(ROWS + 1 > ROWS && COLUMNS + 1 > COLUMNS,
                  "ROWS and/or COLUMNS is too large for "
                  "element access test.");

    matrix_type::value_type const n[] = {2, 3, 5, 7};
    std::initializer_list<std::initializer_list<matrix_type::value_type>>
    matrix_initializer_list{{n[0], n[1]},
                            {n[2], n[3]}};

    matrix_type const m{matrix_initializer_list};

    bool caught_expected_exception = false;

    try {
        (void) m.at(ROWS + 1, COLUMNS);
    } catch(std::out_of_range const &) {
        caught_expected_exception = true;
    }

    if (caught_expected_exception) {
        try {
            (void) m.at(ROWS, COLUMNS + 1);
            caught_expected_exception = false;
        } catch(std::out_of_range const &) {
            caught_expected_exception = true;
        }
    }

    if (caught_expected_exception) {
        try {
            (void) m.at(ROWS + 1, COLUMNS + 1);
            caught_expected_exception = false;
        } catch(std::out_of_range const &) {
            caught_expected_exception = true;
        }
    }

    return m(0, 0) == n[0] && m.at(0, 0) == n[0]
        && m(0, 1) == n[1] && m.at(0, 1) == n[1]
        && m(1, 0) == n[2] && m.at(1, 0) == n[2]
        && m(1, 1) == n[3] && m.at(1, 1) == n[3]
        && caught_expected_exception;
}

bool test_matrix_comparison()
{
    using matrix_type = MaRC::Matrix<int, 3, 2>;

    matrix_type const m1{{0, 1},
                         {3, 4},
                         {6, 7}};

    matrix_type const m2{m1};

    matrix_type const m3 = m2;

    matrix_type const m4{{2, 3},
                         {5, 7},
                         {11, 13}};
    return m2 == m1 && m3 == m2 && m4 != m1;
}

bool test_matrix_addition()
{
    using matrix_type = MaRC::Matrix<int, 3, 2>;

    matrix_type const m1 {{ 0, 1 },
                          { 3, 4 },
                          { 6, 7 }};

    matrix_type const m2 {{-1, 2 },
                          { 4, 1 },
                          {-2, 0 }};

    matrix_type m3{m1};
    m3 += m2;

    matrix_type const sum{{-1, 3 },
                          { 7, 5 },
                          { 4, 7 }};

    return m3 == sum && m1 + m2 == sum;
}

bool test_matrix_subtraction()
{
    using matrix_type = MaRC::Matrix<int, 3, 2>;

    matrix_type const m1{{ 0, 1 },
                         { 3, 4 },
                         { 6, 7 }};

    matrix_type const m2{{-1, 2 },
                         { 4, 1 },
                         {-2, 0 }};

    matrix_type m3{m1};
    m3 -= m2;

    matrix_type const diff{{ 1, -1 },
                           {-1,  3 },
                           { 8,  7 }};

    return m3 == diff && m1 - m2 == diff;
}

bool test_matrix_multiplication()
{
    using left_matrix_type  = MaRC::Matrix<int, 3, 2>;
    using right_matrix_type = MaRC::Matrix<int, 2, 3>;
    using prod_matrix_type  = MaRC::Matrix<int, 3, 3>;
    using vector_type       = MaRC::Vector<int, 3>;

    left_matrix_type  const  left{{ 0, 1 },
                                  { 3, 4 },
                                  { 6, 7}};

    right_matrix_type const right{{ 2,  3,  5 },
                                  { 7, 11, 13 }};

    prod_matrix_type  const  prod{{  7, 11,  13 },
                                   { 34, 53,  67 },
                                   { 61, 95, 121 }};

    vector_type const     v{   2,   3,     5 };

    vector_type const vprod{ 112, 562,  1012 }; // prod * v

    prod_matrix_type::value_type s = 2;

    prod_matrix_type const sprod{{  14,  22,  26 },
                                 {  68, 106, 134 },
                                 { 122, 190, 242 }}; //  prod * s

    return
        left * right == prod
        && prod * v == vprod
        && prod * s == sprod
        && s * prod == sprod;
}

bool test_matrix_transpose()
{
    using matrix_type = MaRC::Matrix<int, 3, 2>;

    matrix_type const m{{0, 1},
                        {3, 4},
                        {6, 7}};

    matrix_type::transpose_type const expected_t{{0, 3, 6},
                                                 {1, 4, 7}};

    auto const t(MaRC::transpose(m));

    return t == expected_t;
}

int main()
{
    return
        test_matrix_initialization()
        && test_matrix_element_access()
        && test_matrix_comparison()
        && test_matrix_addition()
        && test_matrix_subtraction()
        && test_matrix_multiplication()
        && test_matrix_transpose() ? 0 : -1;
}
