// -*- C++ -*-
/**
 * @file vector_formatter.h
 *
 * %MaRC vector class and operations.
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_VECTOR_FORMATTER_H
#define MARC_VECTOR_FORMATTER_H

#include "marc/Vector.h"

#include <fmt/format.h>


/**
 * @brief Formatter for @c MaRC::Vector.
 *
 * This template specialization of @c fmt::formatter<> allows a
 * @c MaRC::Vector to be directly formatted in a fmtlib call.  For
 * example:
 * @code{.cpp}
 *     MaRC::Vector<double, 3> v{ ... };
 *     fmt::print("Vector is: {}", v);
 * @endcode
 * will yield output such as:
 * @code{.txt}
 * Vector is: (3)
 * ⎡       0.4384934571024569 ⎤
 * ⎢       0.2796089830596282 ⎥
 * ⎣      -0.8541324866030424 ⎦
 * @endcode
 */
template <typename T, std::size_t M>
struct fmt::formatter<MaRC::Vector<T, M>>
    : formatter<T>
{
    template <typename FormatContext>
    auto format(MaRC::Vector<T, M> const & v,
                FormatContext & ctx) -> decltype(ctx.out())
    {
        format_to(ctx.out(), "({})\n", M);
        format_to(ctx.out(), "⎡");

        for (std::size_t row = 0; row < M; ++row) {
            constexpr auto last_row = M - 1;

            if (row == last_row)
                format_to(ctx.out(), "⎣");
            else if (row != 0 && row != last_row)
                format_to(ctx.out(), "⎢");

            format_to(ctx.out(), " {:24} ", v[row]);

            if (row == 0)
                format_to(ctx.out(), "⎤\n");
            else if (row != last_row)
                format_to(ctx.out(), "⎥\n");
        }

        return format_to(ctx.out(), "⎦\n");
    }
};


#endif  /* MARC_VECTOR_FORMATTER_H */
