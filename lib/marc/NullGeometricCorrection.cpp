/**
 * @file NullGeometricCorrection.cpp
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "NullGeometricCorrection.h"


void
MaRC::NullGeometricCorrection::image_to_object(
    double & /* line */,
    double & /* sample */) const
{
    // No correction.
}

void
MaRC::NullGeometricCorrection::object_to_image(
    double & /* line */,
    double & /* sample */) const
{
    // No correction.
}
