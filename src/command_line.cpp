/**
 * @file configuration.cpp
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

#include "command_line.h"

#include <MaRC/config.h>

#include <cassert>

#ifdef HAVE_ARGP
# include <argp.h>
#else
# include <algorithm>
# include <iostream>
# include <cstring>
# include <cstdlib>
#endif  // HAVE_ARGP


namespace
{
    constexpr char const args_doc[] = "inputfile1 [inputfile2...]";
    constexpr char const doc[] =
        "Create map projections based on information in given input files.";

#ifdef HAVE_ARGP
    error_t
    parse_opt(int key, char * /* arg */, argp_state * state)
    {
        auto const f =
            static_cast<MaRC::command_line::input_files *>(state->input);

        assert(f != nullptr);

        switch(key) {
        case ARGP_KEY_ARGS:
            f->num_files(state->argc - state->next);
            f->files(state->argv + state->next);
            break;
        case ARGP_KEY_NO_ARGS:
            argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
        };

        return 0;
    }

    argp_option const options[] = {
        { nullptr,  // name
          0,        // key
          nullptr,  // arg
          0,        // flags
          nullptr,  // doc
          0 }       // group
    };

    argp const the_argp = {
        options,   // options
        parse_opt, // parser
        args_doc,  // args_doc
        doc,       // doc
        nullptr,   // children
        nullptr,   // help_filter
        nullptr    // argp_domain
    };

#endif  // HAVE_ARGP
}

// ------------------------------------------------------------

void
MaRC::command_line::input_files::num_files(int n)
{
    assert(n > 0);
    this->num_files_ = n;
}

void
MaRC::command_line::input_files::files(char const * const * f)
{
    assert(f != nullptr);
    this->files_ = f;
}

// ------------------------------------------------------------

bool
MaRC::command_line::parse(int argc, char * argv[])
{
#ifdef HAVE_ARGP
    ::argp_program_version     = PACKAGE_STRING;
    ::argp_program_bug_address = "<" PACKAGE_BUGREPORT ">";

    return argp_parse(&the_argp,
                      argc,
                      argv,
                      0,          // flags
                      nullptr,    // arg_index
                      &this->files_) == 0;
#else
    // No Argp support.  Fall back on basic argument parsing loop.

    constexpr char const try_message[] =
        "Try `" PACKAGE " --help' or `"
        PACKAGE " --usage' for more information.\n";

    auto const begin = argv + 1;        // Ignore the program name.
    auto       end   = argv + argc;

    // No remaining options. "--" encountered on command line.
    bool nropts = false;

    for (auto arg = begin; arg != end && *arg != nullptr; ++arg) {
        if ((*arg)[0] == '-' && !nropts) {
            if (strcmp(*arg, "--") == 0) {
                /*
                  "--" encountered on command line, meaning the user
                  has explicitly marked all remaining command line
                  arguments as non-options.  This is the usual
                  getopt() behavior.
                */
                nropts = true;
            } else if (strcmp(*arg, "--usage") == 0) {
                /**
                 * @todo Hardcoding the command line options in the
                 *       usage message like this is awful.  Lose the
                 *       hardcoding.
                 */

                // Dump full usage message.
                std::cout << "Usage: " PACKAGE " "
                          << "[-?V] [--help] [--usage] [--version]"
                          << args_doc << '\n';

                exit(EXIT_SUCCESS);
            } else if (strcmp(*arg, "--help") == 0
                       || strcmp(*arg, "-?") == 0) {
                // Dump full command line argument details.
                std::cout << "Usage: " PACKAGE " [OPTION...] "
                          << args_doc << '\n'
                          << doc << "\n\n"
                          << "  -?, --help\t\tGive this help list\n"
                             "      --usage\t\tGive a short usage message\n"
                             "  -V, --version\t\tPrint program version\n\n"
                    " Report bugs to < " PACKAGE_BUGREPORT ">.\n";

                    exit(EXIT_SUCCESS);
            } else if (strcmp(*arg, "--version") == 0
                       || strcmp(*arg, "-V") == 0) {
                // Dump MaRC program version.
                std::cout << PACKAGE_STRING "\n";

                exit(EXIT_SUCCESS);
            } else {
                std::cout
                    << argv[0]
                    << ": unrecognized option '" << *arg << "'\n"
                    << try_message;

                exit(EXIT_FAILURE);
            }

            end = std::rotate(arg, arg + 1, end);
        }
    }

    // No input files provided.
    if (begin == end) {
        std::cout << "Usage: " PACKAGE " [OPTION...] "
                  << args_doc << '\n'
                  << try_message;

        exit(EXIT_FAILURE);
    }

    this->files_.num_files(end - begin);
    this->files_.files(begin);

    return true;
#endif  // HAVE_ARGP
}