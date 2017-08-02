//  -*- C++ -*-
/**
 * @file misc.h
 *
 * Copyright (C) 1996-1999, 2017  Ossama Othman
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

#ifndef MARC_MISC_H
#define MARC_MISC_H

#include <MaRC/config.h>


#ifndef HAVE_RINT
# include <cmath>
double rint (double x) { return ::floor (x + 0.5); }
#endif


namespace MaRC
{
    /**
     * @namespace flags
     *
     * @brief MaRC feature flag operations.
     */
    namespace flags
    {
        /// Set feature flags
        inline void
        set(unsigned long & bitset, unsigned long flags)
        {
            bitset |= flags;
        }

        /// Unset feature flags
        inline void
        unset(unsigned long & bitset, unsigned long flags)
        {
            bitset &= ~flags;  // Notice the "~" !!!
        }

        /// Check if flags are set
        inline bool
        check(const unsigned long & bitset, unsigned long flags)
        {
            return (bitset & flags);
        }
    }  // flags
} // MaRC


#endif  /* MARC_MISC_H */
