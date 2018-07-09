/**
 * @file marc.cpp
 *
 * Copyright (C) 1996-1999, 2004, 2017-2018  Ossama Othman
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

#include "command_line.h"
#include "parse_scan.h"
#include "parse.hh"
#include "lexer.hh"
#include "MapCommand.h"

#include <MaRC/config.h>
#include <MaRC/Log.h>

#include <string>
#include <iostream>
#include <list>
#include <cerrno>
#include <cstring>

#include <unistd.h>


namespace MaRC
{

    namespace {
        /**
         * @brief Return error string for XSI-compliant case.
         *
         * @internal Not part of the MaRC API.
         */
        inline char const * strerror_helper(int /* result */,
                                            char const * buf)
        {
            /**
             * @note We lose the ability to report specific errors in
             *       the underlying C library  @c ::strerror_r() call
             *       itself with this approach, but we're not too
             *       worried about that in this case since we're only
             *       using actual @c errno values, meaning
             *       @c ::strerror_r() should never fail.
             */
            return buf;
        }

        /**
         * @brief Return error string for the GNU case.
         *
         * @internal Not part of the MaRC API.
         */
        inline char const * strerror_helper(char const * result,
                                            char const * /* buf */)
        {
            return result;
        }

        /**
         * @brief Get error description for the given error number.
         *
         * Obtain a short description of the error corresponding to
         * the @c errno value @a errnum.  This function exists to work
         * around the case where the GNU C library version of the
         * @c strerror_r() function ends up being used instead of the
         * XSI-compliant version, since both of have the same name and
         * parameter types.  In the GNU case, the @a buf argument is
         * not populated, and the error description string is found in
         * the return value.  Implement that behavior for
         * XSI-compliant platforms as well.
         *
         * @param[in]     errnum The error number, i.e. an @c errno
         *                       value.
         * @param[in,out] buf    Buffer that may end up containing the
         *                       description corresponding to the
         *                       error number @a errnum.  Whether or
         *                       not this buffer is populated depends
         *                       on the platform.  Do not rely on this
         *                       buffer being populated.  Use the
         *                       return value of this function
         *                       instead.
         * @param[in]     buflen The length of the buffer @a buf
         *                       passed to this function.
         *
         * @return Null terminated string containing the error
         *         description.
         *
         * @internal Not part of the MaRC API.
         */
        inline char const * strerror(int errnum,
                                     char * buf,
                                     std::size_t buflen)
        {
            auto const result = ::strerror_r(errnum, buf, buflen);

            return strerror_helper(result, buf);
        }
    }

    // --------------------------------------------------------------

    /**
     * @brief @c FILE stream close functor.
     *
     * This functor closes a @c FILE stream.  It is meant for use as
     * the @c std::unique_ptr<> @c Deleter template parameter.
     */
    struct FILE_closer
    {
        /**
         * @param[in] stream Pointer to @c FILE stream.
         */
        void operator()(FILE * stream) const
        {
            fclose(stream);
        }
    };

    /**
     * @typedef file_unique_ptr
     *
     * Type alias of @c std::unique_ptr<> that automatically closes
     * the a @c FILE stream up on exiting the scope in which an
     * instance of this @c file_unique_ptr resides.
     */
    using FILE_unique_ptr = std::unique_ptr<FILE, FILE_closer>;

    // --------------------------------------------------------------

    /**
     * @brief @c Flex reentrant scanner destroyer.
     *
     * This functor destroys a reentrant Flex scanner.  It is meant
     * for use as the @c std::unique_ptr<> @c Deleter template
     * parameter.
     */
    struct scanner_destroyer
    {
        /**
         * @param[in] stream Pointer to @c FILE stream.
         */
        void operator()(yyscan_t * scanner) const
        {
            yylex_destroy(*scanner);
        }
    };

    /**
     * @typedef file_unique_ptr
     *
     * Type alias of @c std::unique_ptr<> that automatically closes
     * the a @c FILE stream up on exiting the scope in which an
     * instance of this @c file_unique_ptr resides.
     */
    using scanner_unique_ptr =
        std::unique_ptr<yyscan_t, scanner_destroyer>;

    // --------------------------------------------------------------

    bool parse_file(char const * filename, MaRC::ParseParameter & pp)
    {
        MaRC::FILE_unique_ptr const file(fopen(filename, "r"));

        if (!file) {
#ifndef NDEBUG
            constexpr std::size_t BUFLEN = 80;
            char buf[BUFLEN] = {};

            char const * const error =
                MaRC::strerror(errno, buf, sizeof(buf) / sizeof(buf[0]));

            MaRC::debug("Unable to open input file '{}': {}",
                        filename,
                        error);
#endif

            return false;
        }

        yyscan_t scanner;

        if (yylex_init(&scanner) != 0)
            return 1;

        MaRC::scanner_unique_ptr safe_scanner(&scanner);

        yyset_in(file.get(), scanner);

        // For syntax error reporting.
        pp.filename = filename;

        // Parse user defaults/MaRC initialization file.
        int const parsed = ::yyparse(scanner, pp);

        if (parsed != 0) {
            MaRC::error("error parsing '{}'.", filename);

            return false;  // Failure
        }

        // Successful parse
        MaRC::debug("MaRC input file '{}' parsed", filename);

        return true;
    }
}


int main(int argc, char *argv[])
{
    MaRC::command_line cl;

    if (!cl.parse(argc, argv))
        return 1;

    int result = 0;

    try {
        MaRC::ParseParameter parse_parameter;

        std::string user_defaults(::getenv("HOME"));
        user_defaults += "/.marc";

        MaRC::parse_file(user_defaults.c_str(), parse_parameter);

        // Parse MaRC input files give on command line.
        for (auto const filename : cl.files())
            if (!MaRC::parse_file(filename, parse_parameter))
                return -1;;

        // Create the map(s).
        auto const & commands =
            parse_parameter.commands();

        for (auto & p : commands) {
            if (p->execute() != 0) {
                MaRC::error("problem during creation of map '{}'",
                            p->filename());
                result = 1;
            }
        }
    } catch (const std::exception & e) {
        MaRC::error(e.what());

        return -1;
    }

    return result;
}
