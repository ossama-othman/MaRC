// -*- C++ -*-
/**
 * @file Notifier.h
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

#ifndef MARC_NOTIFIER_H
#define MARC_NOTIFIER_H

#include <marc/Export.h>
#include <marc/Observer.h>

#include <memory>
#include <vector>


namespace MaRC
{
    namespace Progress
    {

        /**
         * @class Notifier
         *
         * @brief Map progress notifier.
         *
         * This class provides a means for "observers" to receive map
         * progress notifications.  It is an implementation of the
         * "Subject" component of the Observer design pattern.
         */
        class MARC_API Notifier
        {
        public:

            /// Convenience alias for the progress observer type.
            using observer_type = std::unique_ptr<Observer>;

            /// Constructor
            Notifier();

            // Disallow copying.
            Notifier(Notifier const &) = delete;
            Notifier & operator=(Notifier const &) = delete;

            /// Destructor.
            ~Notifier() = default;

            /**
             * @brief Subscribe to map progress notifications.
             *
             * @param[in,out] observer Object that will receive map
             *                         progress notifications.
             *                         Ownership will be relinquished
             *                         from the caller.
             */
            void subscribe(observer_type observer);

            /**
             * @brief Inform all observers of a new progress update.
             *
             * Notify all observers that a point was plotted in the
             * map of size @a map_size.
             *
             * @param[in] map_size The number of elements in the map
             *                     array.
             */
            void notify_plotted(std::size_t map_size);

            /**
             * @brief Inform all observers that mapping is done.
             *
             * Some map projections will only plot a portion of the
             * map, in which case the underlying plot count in this
             * @c Notifier will never reach @a map_size, meaning
             * subscribed observers may not know that mapping was
             * completed.  Explicitly notify all observers that
             * mapping in a map of size @a map_size is done by passing
             * a plot count equal to @a map_size.
             *
             * @param[in] map_size The number of elements in the map
             *                     array.
             */
            void notify_done(std::size_t map_size);

        private:

            /**
             * @brief Observer notification count.
             *
             * The number of times observers have been notified
             * corresponds the number of elements in a map that have
             * been plotted, i.e. @c plot_count_ out of @c map_size_
             * elements.  Values are always the range [0,
             * @c map_size_].
             *
             * @todo Once parallelization of mapping is supported make
             *       this an atomic variable to address a potential
             *       race condition.
             */
            std::size_t plot_count_;

            /**
             * @brief List of subscribed map progress observers.
             *
             * @todo Access to this container should be synchronized
             *       once parallelized mapping is supported.
             */
            std::vector<observer_type> observers_;

        };

    }  // Progress
}  // MaRC


#endif  // MARC_NOTIFIER_H
