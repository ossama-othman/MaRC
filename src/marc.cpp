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

#include <MaRC/config.h>
#include <MaRC/Log.h>

#include <string>
#include <iostream>
#include <list>

#include <unistd.h>


extern void yyrestart(FILE * input_file);

int
main(int argc, char *argv[])
{
    MaRC::command_line cl;

    if (!cl.parse(argc, argv))
        return 1;

    int result = 0;

    try {
        MaRC::ParseParameter parse_parameters;

        std::string user_defaults(::getenv("HOME"));
        user_defaults += "/.marc";

        FILE * const defaults = fopen(user_defaults.c_str(), "r");
        if (defaults != 0) {
            ::yyrestart(defaults);

            // For syntax error reporting.
            parse_parameters.filename = user_defaults.c_str();

            // Parse user defaults/MaRC initialization file.
            int const parsed = ::yyparse(parse_parameters);

            /**
             * @bug The "defaults" @c FILE stream is explicitly closed
             *      if an exception is thrown when parsing the
             *      initialization file.  Make exception-safe.
             */
            fclose(defaults);

            if (parsed != 0) {
                MaRC::error("error parsing '{}'.", user_defaults);

                return 1;  // Failure
            }

            // Successful parse
            MaRC::debug("user defaults file '{}' parsed",
                        user_defaults);
        }

        // Parse MaRC input files.
        for (auto const f : cl.files()) {
            FILE * const map_input = fopen(f, "r");
            if (map_input != 0) {
                ::yyrestart(map_input);

                // For syntax error reporting.
                parse_parameters.filename = f;

                // Parse user defaults/MaRC initialization file.
                int const parsed = ::yyparse(parse_parameters);

                /**
                 * @bug The "map_input" @c FILE stream is explicitly
                 *      closed if an exception is thrown when parsing
                 *      the initialization file.  Make
                 *      exception-safe.
                 */
                fclose(map_input);

                if (parsed != 0) {
                    MaRC::error("error parsing '{}'.", f);

                    return 1;  // Failure
                }

                // Successful parse
                MaRC::debug("input file '{}' parsed", f);
            } else {
                MaRC::error("unable to open '{}'.", f);

                return 1;
            }
        }

        // Create the map(s).
        MaRC::ParseParameter::command_list const & commands =
            parse_parameters.commands();

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
