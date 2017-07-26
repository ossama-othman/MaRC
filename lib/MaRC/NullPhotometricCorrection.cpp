/**
 * @file NullPhotometricCorrection.cpp
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
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

#include "NullPhotometricCorrection.h"


MaRC::NullPhotometricCorrection::~NullPhotometricCorrection (void)
{
}

int
MaRC::NullPhotometricCorrection::correct (
  const BodyData & /* body */,
  double /* sub_observ_lat */,
  double /* sub_observ_lon */,
  double /* sub_solar_lat */,
  double /* sub_solar_lon */,
  double /* lat */,
  double /* lon */,
  double /* range */,
  double & /* data */)
{
  return 0;
}

MaRC::PhotometricCorrection *
MaRC::NullPhotometricCorrection::clone (void) const
{
  // Stateless so no need to copy construct.
  return new NullPhotometricCorrection;
}
