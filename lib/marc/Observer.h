// -*- C++ -*-
/**
 * @file Observer.h
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_OBSERVER_H
#define MARC_OBSERVER_H

#include <marc/Export.h>

#include <cstddef>


namespace MaRC
{
    namespace Progress
    {

        /**
         * @class Observer Observer.h <marc/Observer.h>
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

            // Disallow moving.
            Observer(Observer &&) = delete;
            Observer & operator=(Observer &&) = delete;

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

            /// Reset progress to initial (zero) state.
            virtual void reset() = 0;

        };

    }  // Progress
}  // MaRC


#endif  // MARC_OBSERVER_H
