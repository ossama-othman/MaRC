/**
 * @file SourceImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "SourceImageFactory.h"

#include <limits>
#include <cmath>
#include <stdexcept>


MaRC::SourceImageFactory::SourceImageFactory()
    : extrema_()
{
}

void
MaRC::SourceImageFactory::update_minmax(double datum)
{
    this->extrema_.update(datum);
}
