// -*- C++ -*-
/**
 * @file ProgressConsole.cpp
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "ProgressConsole.h"
#include "config.h"  // For NDEBUG.

#include <iostream>
#include <cassert>


MaRC::Progress::Console::Console()
    : Observer()
    , percent_complete_old_(0)
{
}

void
MaRC::Progress::Console::notify(std::size_t map_size,
                                std::size_t plot_count)
{
    // Compute percentage of map completion, being careful to avoid
    // integer overflow.
    auto const percent_complete =
        static_cast<std::size_t>(
            static_cast<double>(plot_count) / map_size * 100);

    assert(percent_complete <= 100);

    if (percent_complete > this->percent_complete_old_) {

        // Round to the next even integer.
        auto p = this->percent_complete_old_ + 1;
        p += p % 2;

        // Loop to fill in any gaps greater than 2% between the
        // previous progress dump and this one.
        for ( ; p <= percent_complete; p += 2) {
            // Print the percent complete at increments of 20, a
            // period '.' at increments of 2, and nothing otherwise.
            if (p == 100)
                std::cout << "100%\n";
            else if (p % 20 == 0)
                std::cout << p << std::flush;
            else if (p % 2 == 0)
                std::cout << '.' << std::flush;
        }

        this->percent_complete_old_ = percent_complete;
    }
}

void
MaRC::Progress::Console::reset()
{
    this->percent_complete_old_ = 0;
}
