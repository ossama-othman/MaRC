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

#include "parse_scan.h"

#include <MaRC/config.h>
#include <MaRC/Version.h>
#include <MaRC/Log.h>

/**
 * @todo Should this be an equivalent MaRC include directive?
 */
#include <spdlog/fmt/ostr.h>

#include <string>
#include <iostream>
#include <list>
#include <cstdio>

#include <unistd.h>

extern void yyrestart(FILE * input_file);

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "USAGE:   marc inputfile1 [inputfile2 ...]\n\n";

        return 1;  // Failure
    }

    std::cout
        << PACKAGE_NAME " -- Built on " << __DATE__ << " at " << __TIME__"\n"
        << "\t" PACKAGE_NAME " Binary  Version " PACKAGE_VERSION  "\n"
        << "\t" PACKAGE_NAME " Library Version " << MaRC::library_version()
        << "\n\n"
        << "Copyright (C) 1996-1998, 2003-2004, 2017-2018  Ossama Othman\n"
        << "All Rights Reserved\n\n"
        << PACKAGE_NAME " comes with ABSOLUTELY NO WARRANTY.\n"
        << "This is free software, and you are welcome to redistribute it\n"
        << "under certain conditions.\n\n";

    try {
        MaRC::ParseParameter parse_parameters;

        std::string user_defaults(::getenv("HOME"));
        user_defaults += "/.marc";

        FILE * const defaults = fopen(user_defaults.c_str(), "r");
        if (defaults != 0) {
            ::yyrestart(defaults);

            // Parse user defaults/MaRC initialization file.
            int const parsed = ::yyparse(parse_parameters);

            /**
             * @bug The "defaults" @c FILE stream is explicitly closed
             *      if an exception is thrown when parsing the
             *      initialization file.  Make exception-safe.
             */
            fclose(defaults);

            if (parsed == 0) {
                // Successful parse
                std::cout << "MaRC user defaults file parsed.\n";
            } else {
                MaRC::error("Error parsing '{}'.", user_defaults);

                return 1;  // Failure
            }
        }

        // Parse MaRC input files.
        for (int i = 1; i < argc; ++i) {
            FILE * const map_input = fopen(argv[i], "r");
            if (map_input != 0) {
                ::yyrestart(map_input);

                // Parse user defaults/MaRC initialization file.
                int const parsed = ::yyparse(parse_parameters);

                /**
                 * @bug The "map_input" @c FILE stream is explicitly
                 *      closed if an exception is thrown when parsing
                 *      the initialization file.  Make
                 *      exception-safe.
                 */
                fclose(map_input);

                if (parsed == 0) {
                    // Successful parse
                    std::cout << "MaRC input file '"
                              << argv[i]
                              << "' parsed.\n";
                } else {
                    MaRC::error("Error parsing " PACKAGE_NAME
                                " input file '{}'.",
                                argv[i]);

                  return 1;  // Failure
                }
            } else {
                MaRC::error("Unable to open " PACKAGE_NAME
                            " input file '{}'.",
                            argv[i]);

                return 1;
            }
        }

        // Create the map(s).
        MaRC::ParseParameter::command_list const & commands =
            parse_parameters.commands();

        for (auto & p : commands)
            (void) p->execute();
    } catch (const std::exception & e) {
        std::cerr << "MaRC: " << e.what() << '\n';
        return -1;
    }

    return 0;  // Success
}
