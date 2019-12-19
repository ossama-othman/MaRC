/**
 * @file SourceImageFactory.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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
    : minimum_(std::numeric_limits<double>::signaling_NaN())
    , maximum_(std::numeric_limits<double>::signaling_NaN())
{
}

void
MaRC::SourceImageFactory::minimum(double m)
{
    if (std::isnan(m) || this->maximum_ < m)
        throw std::invalid_argument("invalid source image minimum");

    this->minimum_ = m;
}

void
MaRC::SourceImageFactory::maximum(double m)
{
    if (std::isnan(m) || this->minimum_ > m)
        throw std::invalid_argument("invalid source image maximum");

    this->maximum_ = m;
}
