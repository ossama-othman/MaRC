/**
 * @file MapFactory.cpp
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "MapFactory.h"


MaRC::MapFactory::grid_type
MaRC::MapFactory::make_grid(std::size_t samples,
                            std::size_t lines,
                            double lat_interval,
                            double lon_interval)
{
    /**
     * @todo Verify that @a lat_interval falls within the maximum
     *       latitude range in degrees, i.e. the half-open interval
     *       [0, 180) (always positive!), and similarly for the
     *       @c lon_interval and the longitude range [0, 360).  The
     *       values should be large enough to ensure that data is
     *       always "behind" the grid, otherwise. %MaRC and the %MaRC
     *       library should not allow the invalid lat/lon grid
     *       intervals to be set to begin with.  Such inputs should be
     *       validated beyond the existing negative value checks.
     */

    grid_type grid(samples * lines, grid_type::value_type());

    this->plot_grid(samples,
                    lines,
                    lat_interval,
                    lon_interval,
                    grid);

    return grid;
}
