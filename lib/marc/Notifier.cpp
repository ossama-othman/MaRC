/**
 * @file Notifier.cpp
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "Notifier.h"
#include "Observer.h"
#include "config.h"  // For NDEBUG.

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
MaRC::Progress::Notifier::notify_plotted(std::size_t map_size)
{
    assert(map_size > 0);
    assert(this->plot_count_ < map_size);

    /**
     * @bug This assumes that all points in the map will be plotted.
     *      That isn't true for all map projections.
     */
    ++this->plot_count_;

    /**
     * @todo Access to the observers container should be synchronized
     *       once parallelized mapping is supported.
     */
    for (auto const & o : this->observers_)
        o->notify(map_size, this->plot_count_);
}

void
MaRC::Progress::Notifier::notify_done(std::size_t map_size)
{
    assert(map_size > 0);

    auto const plot_count = map_size;

    /**
     * @todo Access to the observers container should be synchronized
     *       once parallelized mapping is supported.
     */
    for (auto const & o : this->observers_) {
        o->notify(map_size, plot_count);
        o->reset();
    }

    this->plot_count_ = 0;
}
