// -*- C++ -*-
/**
 * @file Log.h
 *
 * @brief %MaRC logging interface.
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
 *
 * @bug We really shouldn't be exposing <MaRC/config.h> to the user
 *      since it contains preprocessor symbols that pollute the global
 *      namespace.
 */

#include "marc/config.h"  // For NDEBUG

// See https://github.com/fmtlib/fmt/issues/947
#ifdef __GNUG__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
/// Avoid having to link the fmt library.
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#ifdef __GNUG__
# pragma GCC diagnostic pop
#endif

/**
 * @def MARC_DEBUG_ARGS(x)
 *
 * @brief Mark MaRC::debug() parameters as unused as needed.
 *
 * The MaRC::debug() function parameters are only used when debugging
 * is enabled (@c NDEBUG preprocessor symbol is undefined), or when
 * Doxygen documentation is being generated.  Mark as unused them by the
 * standard C++ method of doing so when debugging is disabled, i.e. by
 * preventing the parameter name @a x from expanding in that case.
 *
 * @param x MaRC::debug() parameter name.
 *
 * @note @c DXGEN is defined when building MaRC's Doxygen based
 *       documentation.  We want the MaRC::debug() arguments to be
 *       expanded in that case so that Doxygen generates documentation
 *       for them.
 *
 * @note This is an internal macro that is not meant for use outside
 *       of %MaRC.
 */
#if defined(NDEBUG) && !defined(DXGEN)
# define MARC_DEBUG_ARGS(x)
#else
# define MARC_DEBUG_ARGS(x) x
#endif  // NDEBUG


namespace MaRC
{
    namespace details
    {
        /**
         * @name Internal logging functions.
         */
        //@{
        void debug(char const * format, fmt::format_args args);
        void info(char const * format, fmt::format_args args);
        void warn(char const * format, fmt::format_args args);
        void error(char const * format, fmt::format_args args);
        void critical(char const * format, fmt::format_args args);
        //@}
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
