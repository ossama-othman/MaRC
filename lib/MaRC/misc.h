//  -*- C++ -*-
//
// Copyright © 1996-1999  Ossama Othman
//
// All Rights Reserved
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef MARC_MISC_H
#define MARC_MISC_H

#include <MaRC/config.h>


#ifndef HAVE_RINT
# include <cmath>
double rint (double x) { return ::floor (x + 0.5); }
#endif


namespace MaRC
{
  namespace flags
  {
    /// Set feature flags
    inline void
    set (unsigned long & bitset, unsigned long flags)
    {
      bitset |= flags;
    }

    /// Unset feature flags
    inline void
    unset (unsigned long & bitset, unsigned long flags)
    {
      bitset &= ~flags;  // Notice the "~" !!!
    }

    /// Check if flags are set
    inline bool
    check (const unsigned long & bitset, unsigned long flags)
    {
      return (bitset & flags);
    }
  }  // flags
} // MaRC

#endif  /* MARC_MISC_H */
