/**
 * @file first_read.cpp
 *
 * Copyright (C) 2003-2004, 2017, 2020-2021  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "first_read.h"


int
MaRC::first_read::composite(list_type const & images,
                            double lat,
                            double lon,
                            double & data) const
{
    // Return the first found datum.
    for (auto const & i : images)
        if (i->read_data(lat, lon, data))
            return 1;

    return 0;
}
