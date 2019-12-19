// -*- C++ -*-
/**
 * @file ProgressConsole.h
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PROGRESS_CONSOLE_H
#define MARC_PROGRESS_CONSOLE_H

#include <marc/Observer.h>


namespace MaRC
{
    namespace Progress
    {

        /**
         * @class Console
         *
         * @brief Log map progress updates to the console.
         */
        class Console final : public Observer
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
            ~Console() override = default;

            /**
             * @brief Notify observer of progress update.
             *
             * @see MaRC::MapProgress::notify()
             */
            void notify(std::size_t map_size,
                        std::size_t plot_count) override;

        private:

            /**
             * @todo Synchronize access once mapping in parallel is
             *       supported.
             */
            size_t percent_complete_old_;

        };

    }  // Progress
}  // MaRC


#endif  // MARC_PROGRESS_CONSOLE_H
