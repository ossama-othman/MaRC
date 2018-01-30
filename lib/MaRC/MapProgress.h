// -*- C++ -*-
/**
 * @file MapProgress.h
 *
 * Copyright (C) 2018  Ossama Othman
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

#ifndef MARC_MAP_PROGRESS_H
#define MARC_MAP_PROGRESS_H

#include <MaRC/Export.h>

#include <memory>
#include <vector>


namespace MaRC
{

    /**
     * @class MapProgress
     *
     * @brief Map progress notifier.
     *
     * This class provides a means for "observers" to receive map
     * progress notifications.  It is an implementation of the
     * "Subject" component of the Observer design pattern.
     */
    class MARC_API MapProgress
    {
    public:
        using observer_type = std::unique_ptr<MapProgressObserver>;

        /// Constructor.
        MapProgress();

        // Disallow copying.
        MapProgress(MapProgress const &) = delete;
        MapProgress & operator=(MapProgress const &) = delete;

        /// Destructor.
        ~MapProgress() = default;

    private:

        std::vector<observer_type> observers_;

    };

    class MARC_API MapProgressObserver
    {
    public:

        /// Constructor.
        ~MapProgressObserver();

        // Disallow copying.
        MapProgressObserver(MapProgressObserver const &) = delete;
        void operator=(MapProgressObserver const &) = delete;

        /// Destructor.
        virtual ~MapProgress() = default;
    };

}


#endif
