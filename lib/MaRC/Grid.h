// -*- C++ -*-

//==========================================================================
/**
 * @file Grid.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
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
//==========================================================================


#ifndef MARC_GRID_H
#define MARC_GRID_H

#include <MaRC/Image.h>

namespace MaRC
{
  typedef Image<unsigned char> GridParent;

  /**
   * @class Grid
   *
   * @brief Representation of a generated grid image.
   *
   * This class merely encapsulates a generated grid image, and
   * exposes a simple interface that includes data element access
   * operators.  It also eases memory management of the underlying
   * data array.
   */
  class Grid : public GridParent
  {
  public:

    typedef GridParent::data_type data_type;

    /// Constructor.
    Grid (unsigned int samples, unsigned int lines);

  };

}

#endif
