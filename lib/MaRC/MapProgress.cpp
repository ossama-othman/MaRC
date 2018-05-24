/**
 * @file MapProgress.cpp
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

#include "MapProgress.h"

#include <cassert>


MaRC::MapProgress::MapProgress(std::size_t map_size)
    : map_size_(map_size)
    , plot_count_(0)
    , observers_()
{
    assert(map_size > 0);
}

void
MaRC::MapProgress::subscribe(observer_type observer)
{
    /**
     * @todo Access to this container should be synchronized once
     *       parallelized mapping is supported.
     */
    this->observers_.push_back(std::move(observer));
}

void
MaRC::MapProgress::notify_observers()
{
    assert(this->plot_count_ < this->map_size_);

    ++this->plot_count_;

    /**
     * @todo Access to the observers container should be synchronized
     *       once parallelized mapping is supported.
     */
    for (auto const & o : this->observers_)
        o->notify(this->map_size_, this->plot_count_);
}
