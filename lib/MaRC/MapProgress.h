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
    class MARC_API MapProgressObserver
    {
    public:

        /// Constructor.
        ~MapProgressObserver();

        // Disallow copying.
        MapProgressObserver(MapProgressObserver const &) = delete;
        void operator=(MapProgressObserver const &) = delete;

        /// Destructor.
        virtual ~MapProgressObserver() = default;

        /// Notify observer of progress update.
        virtual void notify(double ) const;

    };

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

        /**
         * @brief Constructor
         *
         * @param[in] map_size The number of elements in map array.
         */
        MapProgress(std::size_t map_size);

        // Disallow copying.
        MapProgress(MapProgress const &) = delete;
        MapProgress & operator=(MapProgress const &) = delete;

        /// Destructor.
        ~MapProgress() = default;

        /**
         * Subscribe @a observer for map progress notifications.
         *
         * @param[in,out] observer Object that will receive map
         *                         progress notifications.  Ownership
         *                         will be relinquished from the
         *                         caller.
         */
        void subscribe(observer_type observer);

        /// Inform all subscribed observers of a new progress update.
        void notify_observers();

    private:

        /// The number of elements in map array.
        std::size_t const map_size_;

        /**
         * @brief Observer notification count.
         *
         * The number of times observers have been notified
         * corresponds the number of elements in a map that have been
         * plotted, i.e. @c plot_count_ out of @c map_size_ elements.
         * Values are always the range [0, map_size_].
         *
         * @todo Once parallelization of mapping is supported make
         *       this an atomic variable to address a potential race
         *       condition.
         */
        std::size_t plot_count_;

        /**
         * @brief List of subscribed map progress observers.
         *
         * @todo Access to this container should be synchronized once
         *       parallelized mapping is supported.
         */
        std::vector<observer_type> observers_;

    };

}


#endif
