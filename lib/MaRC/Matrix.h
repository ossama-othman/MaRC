// -*- C++ -*-
/**
 * @file Matrix.h
 *
 * MaRC matrix class and operations.
 *
 * Copyright (C) 2017  Ossama Othman
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

#ifndef MARC_MATRIX_H
#define MARC_MATRIX_H

#include "MaRC/config.h"

#ifdef MARC_HAS_EIGEN3
# include "MaRC/details/eigen3_matrix.hpp"
#else
# include "MaRC/details/marc_matrix.hpp"
#endif  // MARC_HAS_EIGEN3

#endif  /* MARC_MATRIX_H */
