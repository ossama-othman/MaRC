// -*- C++ -*-
/**
 * @file Log.h
 *
 * MaRC logging interface.
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

/**
 * @Todo Fix or at least report this issue to upstream fmt libary
 *       maintainers.
 */
#ifdef __GNUG__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#ifdef __GNUG__
# pragma GCC diagnostic push
#endif

/**
 * @bug We really shouldn't be exposing <MaRC/config.h> to the user
 *      since it contains preprocessor symbols that pollute the global
 *      namespace.
 */
#include "marc/config.h"  // For NDEBUG

#ifdef NDEBUG
# define MARC_DEBUG_ARGS(x)
#else
# define MARC_DEBUG_ARGS(x) x
#endif  // NDEBUG

namespace MaRC
{
    namespace details
    {
        void debug(char const * format, fmt::format_args args);
        void info(char const * format, fmt::format_args args);
        void warn(char const * format, fmt::format_args args);
        void error(char const * format, fmt::format_args args);
        void critical(char const * format, fmt::format_args args);
    }

    /**
     * @brief Log debugging messages.
     *
     * @param[in] format Format of the text being logged, where the
     *                   syntax is defined by the @c fmt library.
     * @param[in] args   Argument values corresponding to placeholder
     *                   in the @a format string.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    debug(char const * MARC_DEBUG_ARGS(format),
          Args const & ... MARC_DEBUG_ARGS(args))
    {
#ifndef NDEBUG
        details::debug(format, fmt::make_format_args(args...));
#endif  // NDEBUG
    }

    /**
     * @brief Log information messages.
     *
     * @param[in] format Format of the text being logged, where the
     *                   syntax is defined by the @c fmt library.
     * @param[in] args   Argument values corresponding to placeholder
     *                   in the @a format string.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    info(char const * format, Args const & ... args)
    {
        details::info(format, fmt::make_format_args(args...));
    }

    /**
     * @brief Log warning messages.
     *
     * @param[in] format Format of the text being logged, where the
     *                   syntax is defined by the @c fmt library.
     * @param[in] args   Argument values corresponding to placeholder
     *                   in the @a format string.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    warn(char const * format, Args const & ... args)
    {
        details::warn(format, fmt::make_format_args(args...));
    }

    /**
     * @brief Log error messages.
     *
     * @param[in] format Format of the text being logged, where the
     *                   syntax is defined by the @c fmt library.
     * @param[in] args   Argument values corresponding to placeholder
     *                   in the @a format string.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    error(char const * format, Args const & ... args)
    {
        details::error(format, fmt::make_format_args(args...));
    }

    /**
     * @brief Log critical messages.
     *
     * @param[in] format Format of the text being logged, where the
     *                   syntax is defined by the @c fmt library.
     * @param[in] args   Argument values corresponding to placeholder
     *                   in the @a format string.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    critical(char const * format, Args const & ... args)
    {
        details::critical(format, fmt::make_format_args(args...));
    }
}
