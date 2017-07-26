/**
 * @file MuImageFactory.cpp
 *
 * Copyright (C) 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#include "MuImageFactory.h"

#include "MaRC/MuImage.h"

MaRC::MuImageFactory::MuImageFactory (const OblateSpheroid & body,
                                      double sub_observ_lat,
                                      double sub_observ_lon,
                                      double range)
  : body_ (body),
    sub_observ_lat_ (sub_observ_lat),
    sub_observ_lon_ (sub_observ_lon),
    range_ (range)
{
}

MaRC::SourceImage *
MaRC::MuImageFactory::make (void)
{
  return new MaRC::MuImage (this->body_,
                            this->sub_observ_lat_,
                            this->sub_observ_lon_,
                            this->range_);
}

MaRC::ImageFactory *
MaRC::MuImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new MuImageFactory (*this);
}
