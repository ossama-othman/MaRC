// -*- C++ -*-
/**
 * @file MapCommand_T.h
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
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

#ifndef MARC_MAP_COMMAND_T_H
#define MARC_MAP_COMMAND_T_H

#include "MapCommand.h"


namespace MaRC
{

    /**
     * @class MapCommand_T
     *
     * @brief Type-specific implementation of MapCommand.
     *
     * Implements type-specific MapCommand operations, such as map
     * creation, etc.  Note that the template type is only visible
     * within the implementation, not within the interface, meaning
     * that execution of this type-specific MapCommand may be invoked
     * polymorphically from a pointer or reference to the
     * non-templatized base MapCommand class.  This simplifies client
     * code that executes MapCommands.
     */
    template <typename T>
    class MapCommand_T : public MapCommand
    {
    public:

        /// Constructor.
        MapCommand_T(std::string filename,
                     std::string body_name,
                     std::unique_ptr<MapFactory> factory,
                     long samples,
                     long lines);

    private:

        using MapCommand::comment_list_type;

        /**
         * @name Private  @c MapCommand Methods.
         *
         * Private methods required by the @c MapCommand interface.
         */
        //@{
        virtual void initialize_FITS_image(fitsfile * fptr, int & status);
        virtual void make_map_planes(fitsfile * fptr, int & status);
        //@}

    };

}


#include "MapCommand_T.cpp"

#endif  /* MARC_MAP_COMMAND_T_H */
