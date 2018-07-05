// -*- C++ -*-
/**
 * @file ProgressConsole.cpp
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

#include "ProgressConsole.h"

#include <iostream>


void
MaRC::Progress::Console::notify(std::size_t map_size,
                                std::size_t plot_count)
{
    /**
     * @bug This calculation can overflow for maps of size greater
     *      than @c std::numeric_limits<std::size_t>::max()/100.
     */
    auto const percent_complete =
        static_cast<int>(plot_count * 100 / map_size);

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
