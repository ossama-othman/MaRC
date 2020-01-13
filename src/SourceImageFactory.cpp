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
MaRC::SourceImageFactory::minimum(double datum)
{
    // Only set once to prevent override of user-specified value.
    if (!this->extrema_.minimum())
        this->extrema_.minimum(datum);
}

void
MaRC::SourceImageFactory::maximum(double datum)
{
    // Only set once to prevent override of user-specified value.
    if (!this->extrema_.maximum())
        this->extrema_.maximum(datum);
}
