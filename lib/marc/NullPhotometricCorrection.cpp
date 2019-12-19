/**
 * @file NullPhotometricCorrection.cpp
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "NullPhotometricCorrection.h"


bool
MaRC::NullPhotometricCorrection::correct(ViewingGeometry const &,
                                         double & /* data */)
{
    return true;
}
