// -*- C++ -*-
/**
 * @file progress_console_fancy.h
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PROGRESS_CONSOLE_FANCY_H
#define MARC_PROGRESS_CONSOLE_FANCY_H

#include <marc/Observer.h>

#include <progressbar/progressbar.h>

#include <string>


namespace MaRC
{
    namespace Progress
    {

        /**
         * @class console_fancy
         *
         * @brief Log fancy map progress updates to the console.
         *
         * Use the progressbar library to log map progress updates to
         * the console, including the estimated time to completion.
         * The progress bar will also take into account the terminal
         * size.
         */
        class console_fancy final : public Observer
        {
        public:

            /// Constructor
            console_fancy(int plane_count,
                          std::size_t num_planes,
                          std::size_t map_size);

            /// Destructor
            ~console_fancy() override;

            // Disallow copying.
            console_fancy(console_fancy const &) = delete;
            void operator=(console_fancy const &) = delete;

            /**
             * @brief Notify observer of progress update.
             *
             * @see MaRC::MapProgress::notify()
             */
            void notify(std::size_t map_size,
                        std::size_t plot_count) override;

        private:

            /// Progress bar label (e.g. "Plane 1 / 4: ").
            std::string const label_;

            /// The underlying progress tracking object.
            progressbar *const bar_;

        };

    }  // Progress
}  // MaRC


#endif  // MARC_PROGRESS_CONSOLE_FANCY_H
