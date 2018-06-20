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

#include "Notifier.h"
#include "Observer.h"

#include <cassert>


MaRC::Progress::Notifier::Notifier()
    : plot_count_(0)
    , observers_()
{
}

void
MaRC::Progress::Notifier::subscribe(observer_type observer)
{
    /**
     * @todo Access to this container should be synchronized once
     *       parallelized mapping is supported.
     */
    this->observers_.push_back(std::move(observer));
}

void
MaRC::Progress::Notifier::notify_observers(size_t map_size)
{
    assert(map_size > 0);
    assert(this->plot_count_ < map_size);

    ++this->plot_count_;

    /**
     * @todo Access to the observers container should be synchronized
     *       once parallelized mapping is supported.
     */
    for (auto const & o : this->observers_)
        o->notify(map_size, this->plot_count_);
}
