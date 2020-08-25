/**
 * @file log_test.cpp
 *
 * Copyright (C) 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/Log.h>


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

    return 0;
}
