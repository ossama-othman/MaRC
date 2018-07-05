// -*- C++ -*-
/**
 * @file Observer.h
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

#ifndef MARC_OBSERVER_H
#define MARC_OBSERVER_H

#include <MaRC/Export.h>

#include <cstddef>


namespace MaRC
{
    namespace Progress
    {

        /**
         * @class Observer
         *
         * @brief Map progress observer.
         *
         * This declares the interface (an abstract base class) that
         * all map progress "observers" must implement.  It
         * corresponds to "Observer" component of the Observer design
         * pattern.
         *
         * @see Notifier
         */        
        class MARC_API Observer
        {
        public:

            /// Constructor
            Observer() = default;

            // Disallow copying.
            Observer(Observer const &) = delete;
            void operator=(Observer const &) = delete;

            /// Destructor.
            virtual ~Observer() = default;

            /**
             * @brief Notify observer of progress update.
             *
             * @param[in] map_size   The number of elements in map
             *                       array.
             * @param[in] plot_count Observer notification count.
             *
             * @note While @a plot_count is guaranteed to increase
             *       between calls to this method, there is no
             *       guarantee that it will increase by 1 each time
             *       this method is called.  Implementations should be
             *       able to handle arbitrarily sized gaps in the
             *       @a plot_count between @c notify() calls.
             */
            virtual void notify(std::size_t map_size,
                                std::size_t plot_count) = 0;

        };

    }  // Progress
}  // MaRC


#endif  // MARC_OBSERVER_H
