/**
 * @file NullInterpolation.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "NullInterpolation.h"


bool
MaRC::NullInterpolation::interpolate(double const *,
                                     double,
                                     double,
                                     double &) const
{
    return true;
}
