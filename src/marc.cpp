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
#include "strerror.h"
#include "parse.hh"
#include "lexer.hh"
#include "MapCommand.h"

#include <marc/config.h>
#include <marc/Log.h>
#include <marc/utility.h>

#include <string>
#include <cstdio>
#include <cerrno>
#include <cstdlib>


namespace MaRC
{
    /**
     * @brief @c FILE stream close functor.
     *
     * This functor closes a @c FILE stream.  It is meant for use as
     * the @c std::unique_ptr @c Deleter template parameter.
     */
    struct FILE_closer
    {
        /**
         * @param[in] stream Pointer to @c FILE stream.
         */
        void operator()(std::FILE * stream) const
        {
            std::fclose(stream);
        }
    };

    /**
     * Type alias of @c std::unique_ptr that automatically closes
     * the a @c FILE stream up on exiting the scope in which an
     * instance of this @c file_unique_ptr resides.
     */
    using FILE_unique_ptr = std::unique_ptr<std::FILE, FILE_closer>;

    // --------------------------------------------------------------

    /**
     * @brief @c Flex reentrant scanner destroyer.
     *
     * This functor destroys a reentrant Flex scanner.  It is meant
     * for use as the @c std::unique_ptr @c Deleter template
     * parameter.
     */
    struct scanner_destroyer
    {
        /**
         * @param[in] scanner Pointer to reentrant scanner object.
         */
        void operator()(yyscan_t * scanner) const
        {
            yylex_destroy(*scanner);
        }
    };

    /**
     * Type alias of @c std::unique_ptr that automatically destroys
     * a reentrant Flex scanner up on exiting the scope in which an
     * instance of this @c scanner_unique_ptr resides.
     */
    using scanner_unique_ptr =
        std::unique_ptr<yyscan_t, scanner_destroyer>;

    // --------------------------------------------------------------

    /**
     * @brief Parse a %MaRC configuration or input file.
     *
     * @param[in]     filename %MaRC configuration or input file to
     *                         parse.
     * @param[in,out] pp       %MaRC configuration parameters.
     *
     * @retval  0 Parsing succeded.
     * @retval  1 Parsing failed due to invalid input.
     * @retval  2 Parsing failed due to memory exhaustion.
     * @retval -1 Error occurred.  Check @c errno.
     */
    int parse_file(char const * filename, MaRC::ParseParameter & pp)
    {
        errno = 0;
        MaRC::FILE_unique_ptr const file(std::fopen(filename, "r"));

        if (!file) {
#ifndef NDEBUG
            constexpr std::size_t BUFLEN = 80;
            char buf[BUFLEN] = {};

            char const * const error =
                MaRC::strerror(errno, buf, MaRC::size(buf));

            MaRC::debug("Unable to open input file '{}': {}",
                        filename,
                        error);
#endif

            return -1;
        }

        yyscan_t scanner;

        if (yylex_init(&scanner) != 0)
            return -1;

        MaRC::scanner_unique_ptr safe_scanner(&scanner);

        yyset_in(file.get(), scanner);

        // For syntax error reporting.
        pp.filename = filename;

        // Parse user defaults/MaRC initialization file.
        int const parsed = ::yyparse(scanner, pp);

        if (parsed == 0) {
            // Successful parse
            MaRC::debug("MaRC input file '{}' parsed", filename);
        }

        return parsed;
    }

    /**
     * @brief Get the %MaRC configuration filename.
     *
     * Get the %MaRC configuration filename, conforming to the XDG
     * Base Directory specification.  The %MaRC configuration file
     * will be @c ~/.config/marc by default, assuming the user hasn't
     * changed the %MaRC package name at build-time.
     *
     * @see https://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
     */
    std::string get_config_filename()
    {
        // Get XDG Base Directory Specification conforming
        // configuration file.
        std::string filename;

        char const * const config_dir = std::getenv("XDG_CONFIG_HOME");
        char const * const home_dir   = std::getenv("HOME");

        if (config_dir != nullptr) {
            filename = std::string(config_dir) + "/" PACKAGE;
        } else {
            if (home_dir == nullptr)
                return filename;  // No home directory!

            constexpr char const tmp[] = "/.config/" PACKAGE;

            filename = std::string(home_dir) + tmp;
        }

        // -----------------------------------------------------
        // Warn of existence of old configuration file.
        // -----------------------------------------------------
        if (home_dir != nullptr) {
            std::string const old_config =
                std::string(home_dir) + "/.marc";

            if (access(old_config.c_str(), F_OK) == 0) {
                MaRC::warn("old configuration file `{}' exists",
                           old_config);
                MaRC::warn("expected: `{}'.", filename);
            }
        }
        // -----------------------------------------------------

        return filename;
    }

}

/// The canonical main entry point to the %MaRC process.
int main(int argc, char *argv[])
{
    int result = 0;

    try {
        MaRC::ParseParameter parse_parameter;

        std::string const config_file(MaRC::get_config_filename());

        // Parse user configuration file, if it exists.
        if (MaRC::parse_file(config_file.c_str(), parse_parameter) != 0
            && errno != ENOENT) {
            // Scanner or parser error.
            return -1;
        }

        /*
          Parse command line options, overriding corresponding
          configuration file parameters if any exist.
        */
        MaRC::command_line cl;

        if (!cl.parse(argc, argv))
            return 1;

        // Parse MaRC input files given on command line.
        for (auto const filename : cl.files())
            if (MaRC::parse_file(filename, parse_parameter) != 0)
                return -1;

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
    } catch (std::exception const & e) {
        /*
          Fall back on fprintf() to avoid potentially throwing another
          exception through the underlying C++ logging framework.
        */
        std::fprintf(stderr,
                     "[" PACKAGE "][error] %s\n",
                     e.what());

        return -1;
    }

    return result;
}
