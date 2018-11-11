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

/**
 * @todo Ideally we shouldn't have to include this header since the
 *       underlying MaRC logger shouldn't be exposed to the user.
 */
#include "Export.h"  // For MARC_API
#include "Log.h"


namespace
{
    inline void marc_log(char const * level,
                         char const * format,
                         fmt::format_args args)
    {
        fmt::print("[" PACKAGE "][{}] ", level);
        fmt::vprint(format, args);
        fmt::print("\n");
    }
}

// --------------------------------------------------------------

MARC_API void
MaRC::details::debug(char const * format, fmt::format_args args)
{
    marc_log("debug", format, args);
}

MARC_API void
MaRC::details::info(char const * format, fmt::format_args args)
{
    marc_log("info", format, args);
}

MARC_API void
MaRC::details::warn(char const * format, fmt::format_args args)
{
    marc_log("warn", format, args);
}

MARC_API void
MaRC::details::error(char const * format, fmt::format_args args)
{
    marc_log("error", format, args);
}

MARC_API void
MaRC::details::critical(char const * format, fmt::format_args args)
{
    marc_log("critical", format, args);
}
