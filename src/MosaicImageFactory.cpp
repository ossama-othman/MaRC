/**
 * @file MosaicImageFactory.cpp
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

#include "MosaicImageFactory.h"

#include <memory>


MaRC::MosaicImageFactory::MosaicImageFactory (const list_type & factories,
                                              MosaicImage::average_type type)
  : factories_ (factories),
    average_type_ (type)
{
}

MaRC::SourceImage *
MaRC::MosaicImageFactory::make (void)
{
  MosaicImage::list_type photos;

  list_type::const_iterator end = this->factories_.end ();
  for (list_type::iterator i = this->factories_.begin (); i != end; ++i)
    {
      std::unique_ptr<MaRC::PhotoImageFactory::return_type>
        source ((*i).make ());

      const PhotoImage & photo =
#ifndef MARC_HAS_COVARIANT_RETURN_TYPES
        dynamic_cast<PhotoImage &>
#endif  /* MARC_HAS_COVARIANT_RETURN_TYPES */
        (*source);

      photos.push_back (photo);
    }

  return new MosaicImage (photos, this->average_type_);
}

MaRC::ImageFactory *
MaRC::MosaicImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new MosaicImageFactory (*this);
}
