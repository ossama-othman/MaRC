/**
 * @file MapFactory.cpp
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
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

#include "MapFactory.h"


MaRC::MapFactory::grid_type
MaRC::MapFactory::make_grid(std::size_t samples,
                            std::size_t lines,
                            double lat_interval,
                            double lon_interval)
{
    grid_type grid(samples * lines, grid_type::value_type());

    this->plot_grid(samples,
                    lines,
                    lat_interval,
                    lon_interval,
                    grid);

    return grid;
}
