/**
 * @file command_line.cpp
 *
 * Copyright (C) 2018  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "command_line.h"

#include <marc/config.h>

#include <cassert>

#ifdef HAVE_ARGP
# include <argp.h>
#else
# include <algorithm>
# include <iostream>
# include <cstring>
# include <cstdlib>
# ifdef HAVE_SYSEXITS_H
#   include <sysexits.h>
# else
/// Exit code that signifies incorrect invocation of a given command.
#   define EX_USAGE 64
# endif
#endif  // HAVE_ARGP


namespace
{
    constexpr char const args_doc[] = "file...";
    constexpr char const doc[] =
        "Create map projections based on information in given input files.";

#ifdef HAVE_ARGP
    error_t
    parse_opt(int key, char * /* arg */, argp_state * state)
    {
        auto const f =
            static_cast<MaRC::command_line::arguments *>(state->input);

        assert(f != nullptr);

        switch(key) {
        case ARGP_KEY_ARGS:
            f->args(state->argc - state->next,
                    state->argv + state->next);
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
MaRC::command_line::arguments::args(int argc,
                                    char const * const * argv)
{
    assert(argc > 0);
    assert(argv != nullptr);

    this->argc_ = argc;
    this->argv_ = argv;
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
            /**
             * @bug This command line option parser doesn't correctly
             *      handle multiple short options grouped as one,
             *      e.g. "-a -c" grouped as "-ac".  This will become a
             *      problem if %MaRC supports multiple short options
             *      in the future.
             */

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
                          << "[-?V] [--help] [--usage] [--version] "
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
                std::cerr
                    << argv[0]
                    << ": unrecognized option '" << *arg << "'\n"
                    << try_message;

                exit(EX_USAGE);
            }

            end = std::rotate(arg, arg + 1, end);
        }
    }

    // No input files provided.
    if (begin == end) {
        std::cerr << "Usage: " PACKAGE " [OPTION...] "
                  << args_doc << '\n'
                  << try_message;

        exit(EX_USAGE);
    }

    this->files_.args(end - begin, begin);

    return true;
#endif  // HAVE_ARGP
}
