/**
 * @file Matrix_Test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/Vector.h>
#include <marc/Matrix.h>

#include <catch2/catch.hpp>

#include <algorithm>


/**
 * @test Test MaRC::Matrix initialization.
 */
TEST_CASE("MaRC::Matrix initialization", "[matrix]")
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

    REQUIRE(element_count > 0);
    REQUIRE(static_cast<std::remove_const<decltype(ROWS)>::type>(
                element_count) == ROWS * COLUMNS);
    REQUIRE(std::find_if_not(
                std::cbegin(m1),
                std::cend(m1),
                [](auto value)
                {
                    return value == matrix_type::value_type();
                }) == std::cend(m1));
    REQUIRE(std::equal(std::cbegin(m2), std::cend(m2),
                       std::cbegin(n),  std::cend(n)));
    REQUIRE(std::equal(std::cbegin(m3), std::cend(m3),
                       std::cbegin(m2), std::cend(m2)));
    REQUIRE(std::equal(std::cbegin(m4), std::cend(m4),
                       std::cbegin(n),  std::cend(n)));
    REQUIRE(std::equal(std::cbegin(m5), std::cend(m5),
                       std::cbegin(m3), std::cend(m3)));
}

/**
 * @test Test MaRC::Matrix element access.
 */
TEST_CASE("MaRC::Matrix element access", "[matrix]")
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

    matrix_type const m{matrix_initializer_list};\

    REQUIRE(m(0, 0) == n[0]);
    REQUIRE(m.at(0, 0) == n[0]);

    REQUIRE(m(0, 1) == n[1]);
    REQUIRE(m.at(0, 1) == n[1]);

    REQUIRE(m(1, 0) == n[2]);
    REQUIRE(m.at(1, 0) == n[2]);

    REQUIRE(m(1, 1) == n[3]);
    REQUIRE(m.at(1, 1) == n[3]);

    REQUIRE_THROWS_AS((void) m.at(ROWS + 1, COLUMNS), std::out_of_range);
    REQUIRE_THROWS_AS((void) m.at(ROWS, COLUMNS + 1), std::out_of_range);
    REQUIRE_THROWS_AS((void) m.at(ROWS + 1, COLUMNS + 1),
                      std::out_of_range);
}

/**
 * @test Test MaRC::Matrix comparison.
 */
TEST_CASE("MaRC::Matrix comparison", "[matrix]")
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
    REQUIRE(m2 == m1);
    REQUIRE(m3 == m2);
    REQUIRE(m4 != m1);
}

/**
 * @test Test MaRC::Matrix addition.
 */
TEST_CASE("MaRC::Matrix addition", "[matrix]")
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

    REQUIRE(m3      == sum);
    REQUIRE(m1 + m2 == sum);
}

/**
 * @test Test MaRC::Matrix subtraction.
 */
TEST_CASE("MaRC::Matrix subtraction", "[matrix]")
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

    REQUIRE(m3      == diff);
    REQUIRE(m1 - m2 == diff);
}

/**
 * @test Test MaRC::Matrix multiplication by scalars and vectors.
 */
TEST_CASE("MaRC::Matrix multiplication", "[matrix]")
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

    REQUIRE(left * right == prod);
    REQUIRE(prod * v == vprod);
    REQUIRE(prod * s == sprod);
    REQUIRE(s * prod == sprod);
}

/**
 * @test Test the MaRC::transpose() function.
 */
TEST_CASE("MaRC::Matrix transpose", "[matrix]")
{
    using matrix_type = MaRC::Matrix<int, 3, 2>;

    matrix_type const m{{0, 1},
                        {3, 4},
                        {6, 7}};

    matrix_type::transpose_type const expected_t{{0, 3, 6},
                                                 {1, 4, 7}};

    auto const t(MaRC::transpose(m));

    REQUIRE(t == expected_t);
}
