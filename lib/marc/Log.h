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
 * @todo Ideally we shouldn't have to include this header since the
 *       underlying MaRC logger instance shouldn't be exposed to the
 *       user.
 */
#include "marc/Export.h"  // For MARC_API

/**
 * @bug We really shouldn't be exposing <MaRC/config.h> to the user
 *      since it contains preprocessor symbols that pollute the global
 *      namespace.
 */
#include "marc/config.h"  // For NDEBUG

/**
 * Enable debug and trace level logging in spdlog.
 *
 * @note @c NDEBUG should be defined on the command line if debug
 *       logging should be disabled so that including "MaRC/config.h"
 *       and exposing as part of the user interface can be avoided.
 */
#ifdef NDEBUG
# define MARC_DEBUG_ARGS(x)
#else
# define SPDLOG_DEBUG_ON
# define MARC_DEBUG_ARGS(x) x
#endif  // NDEBUG

#include <spdlog/spdlog.h>


namespace MaRC
{
    /**
     * @typedef Underlying logger type.
     *
     * @note Not meant for use outside of MaRC.
     */
    using logger_type = std::shared_ptr<spdlog::logger>;

    /**
     * @internal Pointer to the underlying MaRC logger.
     *
     * @todo This isn't a good way to expose the MaRC logger
     *       instance since it isn't meant to be part of the MaRC
     *       API.
     */
    extern MARC_API logger_type const _logger;

    /**
     * @brief Log debugging messages.
     *
     * @param[in] args Log arguments formatted according to the @c fmt
     *                 library.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    debug(Args const & ... MARC_DEBUG_ARGS(args))
    {
        SPDLOG_DEBUG(_logger, args ...);
    }

    /**
     * @brief Log information messages.
     *
     * @param[in] args Log arguments formatted according to the @c fmt
     *                 library.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    info(Args const & ... args)
    {
        _logger->info(args ...);
    }

    /**
     * @brief Log warning messages.
     *
     * @param[in] args Log arguments formatted according to the @c fmt
     *                 library.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    warn(Args const & ... args)
    {
        _logger->warn(args ...);
    }

    /**
     * @brief Log error messages.
     *
     * @param[in] args Log arguments formatted according to the @c fmt
     *                 library.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    error(Args const & ... args)
    {
        _logger->error(args ...);
    }

    /**
     * @brief Log critical messages.
     *
     * @param[in] args Log arguments formatted according to the @c fmt
     *                 library.
     *
     * @see http://fmtlib.net/latest/index.html
     */
    template <typename ... Args>
    void
    critical(Args const & ... args)
    {
        _logger->critical(args ...);
    }
}
