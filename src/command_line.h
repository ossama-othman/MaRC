/**
 * @file command_line.h
 *
 * MaRC command line option parsing.
 *
 * Copyright (C) 2018  Ossama Othman
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

#ifndef MARC_COMMAND_LINE_H
#define MARC_COMMAND_LINE_H


namespace MaRC
{
    /**
     * @class command_line
     *
     * @brief Parse MaRC command line arguments.
     *
     * This class parses MaRC command line arguments, and implements
     * the MaRC usage and help messages.
     */
    class command_line
    {
    public:

        /**
         * @class input_files
         *
         * @brief 
         */
        class input_files
        {
        public:

            /// Constructor.
            input_files() : num_files_(0), files_(nullptr) {}

            /// Destructor.
            ~input_files() = default;

            // Disallow copying.
            input_files(input_files const &) = delete;
            void operator=(input_files const &) = delete;

            void num_files(int n);
            int num_files() const { return this->num_files_; }

            void files(char const * const * f);
            char const * const * files() const { return this->files_; }

        private:

            /// Number of input filenames on the command line.
            int num_files_;

            /**
             * @brief Names of MaRC input files to be processed.
             *
             * @note The memory for this array is part of the argument
             *       vector @c argv[] passed to the @c main()
             *       function.  Do not attempt to deallocate its
             *       memory.  As with @c main() @c argv[] argument,
             *       the last element in the array (e.g.
             *       @c argv[argc]) is the @c nullptr.
             */
            char const * const * files_;

        };
    
        /// Constructor.
        command_line() : files_() {}

        /// Destructor.
        ~command_line() = default;

        // Disallow copying.
        command_line(command_line const &) = delete;
        void operator=(command_line const &) = delete;

        /// Parse command line arguments.
        bool parse(int argc, char * argv[]);

        int num_files() const { return this->files_.num_files(); }
        char const * const * files() const { return this->files_.files(); }

    private:

        /// Information about input files provided on the command line.
        input_files files_;

    };

}


#endif


/*
  Local Variables:
  mode: c++
  c-basic-offset: 4
  indent-tabs-mode: nil
  End:
*/
