// -*- C++ -*-
/**
 * @file Log.cpp
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

#include "Log.h"
#include "config.h"


namespace {
    auto init_marc_logger()
    {
        auto logger = spdlog::stdout_color_mt(PACKAGE);

        // No need for a date and time stamp for console logging at
        // this point in time.
        // [logger name] [log level] text to log
        logger->set_pattern("[%n] [%l] %v");

#ifndef NDEBUG
        logger->set_level(spdlog::level::debug);
#endif  // NDEBUG

        return logger;
    }
}

namespace MaRC
{
    logger_type const _logger = init_marc_logger();
}
