// -*- C++ -*-
/**
 * @file Version.h
 *
 * MaRC library version information.
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

#ifndef MARC_LIB_VERSION_H
#define MARC_LIB_VERSION_H

#include "marc/Export.h"


namespace MaRC
{
    /**
     * @brief Get MaRC library version.
     *
     * @todo Remove this function.  It doesn't serve a useful purpose,
     *       particularly library versioning is introduced.
     *
     * @deprecated This function will be removed soon.  Do not use
     *             it.
     *
     * @return String containing the MaRC library version.
     */
    [[deprecated]] MARC_API char const * library_version();
}


#endif  /* MARC_LIB_VERSION_H */
