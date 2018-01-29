// -*- C++ -*-
/**
 * @file Log.h
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

#include <MaRC/Export.h>

#include <spdlog/spdlog.h>


namespace MaRC
{
    using logger_type = std::shared_ptr<spdlog::logger>;

    /**
     * @internal Pointer to the underlying MaRC logger;
     *
     * @todo This isn't a good way to expose the MaRC logger
     *       instance.
     *
     * @attention Not to be confused with @c std::log()!
     *
     */
    extern logger_type const _logger;

    template <typename ... Args>
    void
    trace(Args const & ... args)
    {
        _logger->trace(args ...);
    }

    template <typename ... Args>
    void
    debug(Args const & ... args)
    {
        _logger->debug(args ...);
    }

    template <typename ... Args>
    void
    info(Args const & ... args)
    {
        _logger->info(args ...);
    }

    template <typename ... Args>
    void
    warn(Args const & ... args)
    {
        _logger->warn(args ...);
    }

    template <typename ... Args>
    void
    error(Args const & ... args)
    {
        _logger->error(args ...);
    }

    template <typename ... Args>
    void
    critical(Args const & ... args)
    {
        _logger->critical(args ...);
    }

}
