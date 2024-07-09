/**
 * @file log_test.cpp
 *
 * Copyright (C) 2020, 2021  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/Log.h>
#include <marc/vector_formatter.h>
#include <marc/matrix_formatter.h>

#include <limits>

/// The canonical main entry point.
int main()
{
    // Exercise the available MaRC log message types.
    MaRC::debug("debug log message 1");
    MaRC::debug("debug log message {}", 2);

    MaRC::info("info log message 1");
    MaRC::info("info log message {}", 2);

    MaRC::warn("warn log message 1");
    MaRC::warn("warn log message {}", 2);

    MaRC::error("error log message 1");
    MaRC::error("error log message {}", 2);

    MaRC::critical("critical log message 1");
    MaRC::critical("critical log message {}", 2);

    // ----------------------------------------------

    using int_type   = intmax_t;
    using uint_type  = uintmax_t;
    using float_type = long double;

    // ----------------------------------------------

    using int_vector_type = MaRC::Vector<int_type, 5>;
    int_vector_type iv{
        std::numeric_limits<int_vector_type::value_type>::lowest(),
        11,
        13,
        17,
        std::numeric_limits<int_vector_type::value_type>::max()
    };

    using float_vector_type = MaRC::Vector<float_type, 4>;
    float_vector_type fv{
        std::numeric_limits<float_vector_type::value_type>::lowest(),
        1e3,
        -1e6,
        std::numeric_limits<float_vector_type::value_type>::max()
    };

    MaRC::info("formatted integer vector: {}", iv);
    MaRC::info("formatted floating point vector: {}", fv);

    // ----------------------------------------------

    using uint_matrix_type = MaRC::Matrix<uint_type, 3, 2>;
    uint_matrix_type const uim{
        {std::numeric_limits<uint_matrix_type::value_type>::lowest(), 1},
        {3, 4},
        {6, std::numeric_limits<uint_matrix_type::value_type>::max()}
    };

    using float_matrix_type = MaRC::Matrix<float_type, 3, 3>;
    float_matrix_type const fm{
        {std::numeric_limits<float_matrix_type::value_type>::lowest(), 1, 0},
        {3, 4, -23},
        {6, std::numeric_limits<float_matrix_type::value_type>::max(), 24e5}
    };

    MaRC::info("formatted integer matrix: {}", uim);
    MaRC::info("formatted floating point matrix: {}", fm);

    return 0;
}
