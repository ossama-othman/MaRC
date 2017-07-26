/**
 * @file NaN.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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

#include "NaN.h"
#include "config.h"

#if defined (MARC_HAS_STL_QUIET_NAN_SUPPORT)
# include <limits>

double const MaRC::NaN =
  std::numeric_limits<double>::quiet_NaN ();

#elif defined (MARC_C_LIB_QUIET_NAN) && defined (_ISOC99_SOURCE)

# include <cmath>
# include <cstdlib>

double const MaRC::NaN = MARC_C_LIB_QUIET_NAN;

#else
# error MaRC requires quiet NaN support!
#endif
