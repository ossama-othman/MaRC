// -*- C++ -*-
/**
 * @file matrix_formatter.h
 *
 * %MaRC matrix class formatting.
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MATRIX_FORMATTER_H
#define MARC_MATRIX_FORMATTER_H

#include "marc/Matrix.h"
#include "marc/config.h"  // For FMT_HEADER_ONLY

#include <fmt/format.h>


/**
 * @brief Formatter for @c MaRC::Matrix.
 *
 * This template specialization of @c fmt::formatter<> allows a
 * @c MaRC::Matrix to be directly formatted in a fmtlib call.  For
 * example:
 * @code{.cpp}
 *     MaRC::Matrix<double, 3. 3> m{ ... };
 *     fmt::print("Matrix is: {}", m);
 * @endcode
 * will yield output such as:
 * @code{.txt}
 * Matrix is: (3 x 3)
 * ⎡   -0.8896157352145879 -1.1758004881892886e-16   -0.4567098024551349 ⎤
 * ⎢  -0.12770016341784404      0.9601139602111619   0.24874455103719437 ⎥
 * ⎣    0.4384934571024569      0.2796089830596282   -0.8541324866030424 ⎦
 * @endcode
 */
template <typename T, std::size_t M, std::size_t N>
struct fmt::formatter<MaRC::Matrix<T, M, N>>
    : formatter<T>
{
    template <typename FormatContext>
    auto format(MaRC::Matrix<T, M, N> const & m,
                FormatContext & ctx) -> decltype(ctx.out())
    {
        format_to(ctx.out(), "({} x {})\n", M, N);
        format_to(ctx.out(), "⎡");

        for (std::size_t row = 0; row < M; ++row) {
            constexpr auto last_row = M - 1;

            if (row == last_row)
                format_to(ctx.out(), "⎣");
            else if (row != 0 && row != last_row)
                format_to(ctx.out(), "⎢");

            for (std::size_t col = 0; col < N; ++col)
                format_to(ctx.out(), " {:24}", m(row, col));

            if (row == 0)
                format_to(ctx.out(), " ⎤\n");
            else if (row != last_row)
                format_to(ctx.out(), " ⎥\n");
        }

        return format_to(ctx.out(), " ⎦\n");
    }
};


#endif  /* MARC_MATRIX_FORMATTER_H */
