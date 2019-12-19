// -*- C++ -*-
/**
 * @file Log.cpp
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

/**
 * @todo Ideally we shouldn't have to include this header since the
 *       underlying %MaRC logger shouldn't be exposed to the user.
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
