// -*- C++ -*-
/**
 * @file ProgressConsole.h
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

#ifndef MARC_PROGRESS_CONSOLE_H
#define MARC_PROGRESS_CONSOLE_H

#include <MaRC/Observer.h>


namespace MaRC
{
    namespace Progress
    {

        /**
         * @class Console
         *
         * @brief Log map progress updates to the console.
         */
        class Console : public Observer
        {
        public:

            /// Constructor
            Console()
                : Observer()
                , percent_complete_old_(0)
            {
            }

            // Disallow copying.
            Console(Console const &) = delete;
            void operator=(Console const &) = delete;

            /// Destructor
            virtual ~Console() = default;

            /**
             * @brief Notify observer of progress update.
             *
             * @see MaRC::MapProgress::notify()
             */
            virtual void notify(std::size_t map_size,
                            std::size_t plot_count);

        private:

            /**
             * @todo Synchronize access once mapping in parallel is
             *       supported.
             */
            int percent_complete_old_;

        };

    }  // Progress
}  // MaRC


#endif  // MARC_PROGRESS_CONSOLE_H
