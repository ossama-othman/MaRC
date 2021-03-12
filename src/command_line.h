/**
 * @file command_line.h
 *
 * %MaRC command line option parsing.
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
     * @brief Parse %MaRC command line arguments.
     *
     * This class parses %MaRC command line arguments, and implements
     * the %MaRC usage and help messages.
     */
    class command_line
    {
    public:

        /**
         * @class arguments
         *
         * @brief Command line argument container.
         */
        class arguments
        {
        public:

            /// STL style iterator type.
            using const_iterator = char const * const *;

            /// Constructor.
            arguments() : argc_(0), argv_(nullptr) {}

            /// Destructor.
            ~arguments() = default;

            // Disallow copying.
            arguments(arguments const &) = delete;
            void operator=(arguments const &) = delete;

            /**
             * @brief Set argument count and vector.
             *
             * @param[in] argc Argument count.
             * @param[in] argv Argument vector.
             *
             * @note This method is meant to be used internally by
             *       @c command_line.
             */
            void args(int argc, char const * const * argv);

            /**
             * @name Iterators
             *
             * @brief STL style iterator support.
             *
             * These iterator methods allow the argument vector to be
             * accessed in the same way data held by other containers
             * is accessed, including STL algorithms, ranged-for
             * loops, etc.
             *
             * @note Only read-only (@c const) iterators are exposed
             *       by this class since only the command line parsing
             *       code is allowed to alter the command line
             *       argument vector.
             */
            //@{
            const_iterator begin() const noexcept { return this->argv_; }
            const_iterator cbegin() const noexcept { return this->begin(); }
            const_iterator end() const noexcept
            {
                return this->argv_ + this->argc_;
            }
            const_iterator cend() const noexcept { return this->end(); }
            //@}

        private:

            /// Command line argument count.
            int argc_;

            /**
             * @brief Command line argument vector.
             *
             * @note The memory for this array is part of the argument
             *       vector @c argv[] passed to the @c main()
             *       function.  Do not attempt to deallocate its
             *       memory.  As with @c main() @c argv[] argument,
             *       the last element in the array (e.g.
             *       @c argv[argc]) is the @c nullptr.
             */
            char const * const * argv_;

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

        /// Get container of %MaRC input filenames.
        auto const & files() const { return this->files_; }

    private:

        /**
         * @brief Names of input files to be processed by %MaRC.
         */
        arguments files_;

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
