// -*- C++ -*-
/**
 * @file progress_console_fancy.cpp
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "progress_console_fancy.h"

#include <fmt/core.h>


MaRC::Progress::console_fancy::console_fancy(int plane_count,
                                             std::size_t num_planes,
                                             std::size_t map_size)
    : Observer()
    , label_(fmt::format("Plane {} / {}:", plane_count, num_planes))
    , bar_(progressbar_new(this->label_.c_str(), map_size))
{
}

MaRC::Progress::console_fancy::~console_fancy()
{
    progressbar_finish(this->bar_);
}

void
MaRC::Progress::console_fancy::notify(std::size_t /* map_size */,
                                      std::size_t plot_count)
{
    progressbar_update(this->bar_, plot_count);
}
