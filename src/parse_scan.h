/**
 * @file parse_scan.h
 *
 * MaRC Parser and Scanner Declarations
 *
 * Copyright (C) 1999, 2004, 2017  Ossama Othman
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

#ifndef MARC_PARSE_SCAN_H
#define MARC_PARSE_SCAN_H

#include "calc.h"
#include "MapCommand.h"

#include <list>
#include <memory>

// The following #undef in necessary to make it possible to use GNU
// Flex 2.5.31 generated C++ scanners without "redefinition"
// compile-time errors.  *sigh*
// #undef yyFlexLexer
// #include <FlexLexer.h>


namespace MaRC
{

    /**
     * @class ParseParameter
     *
     * @brief Parameter passed to reentrant parser.
     *
     * This is the type passed to the GNU Bison generated reentrant
     * parser.
     */
    class ParseParameter
    {
    public:

        typedef std::list<std::unique_ptr<MapCommand>> command_list;

        /// Constructor.
        ParseParameter();

        command_list const & commands() const { return this->commands_; }

        /// Push a Command object on to the list of Commands to
        /// execute.
        void push_command(std::unique_ptr<MapCommand> c);

        /// Return reference to symbol table.
        symrec & sym_table() { return this->sym_table_; }

        /// Return reference to scanner.
        //     FlexLexer & lexer() { return this->lexer_; }

        /// Change
        void input_stream(std::istream & i);

    public:

        /**
         * @name User Defined Defaults
         *
         * Each of these values also has a program defined default.
         */
        //@{
        /// Default latitude grid line interval.
        double lat_interval;

        /// Default longitude grid line interval.
        double lon_interval;

        /// Default source image minimum cutoff value.
        double minimum;

        /// Default source image maximum cutoff value.
        double maximum;

        /// Default source image left side "nibble" value.
        unsigned int nibble_left;

        /// Default source image right side "nibble" value.
        unsigned int nibble_right;

        /// Default source image top side "nibble" value.
        unsigned int nibble_top;

        /// Default source image bottom side "nibble" value.
        unsigned int nibble_bottom;
        //@}

    private:

        /// List of commands to execute.
        /**
         * The command list is constructed while parsing the user's
         * defaults and map input files.
         */
        command_list commands_;

        /// Calculator symbol table.
        symrec sym_table_;

        /// C++ scanner instance.
        //     yyFlexLexer lexer_;

    };

    /**
     * @struct SubObserv
     *
     * @brief Structure containing sub-observer point latitude and
     *        longitude.
     *
     * Parsed sub-observer latitudes and longitudes will be stored in an
     * instance of this structure.
     */
    struct SubObserv
    {
        /// Sub-observer latitude in degrees.
        double lat;

        /// Sub-observer longitude in degrees.
        double lon;
    };

    typedef SubObserv SubSolar;

    /**
     * @struct Radii
     *
     * @brief Structure containing oblate spheroid radii.
     *
     * Parsed radii will be stored in an instance of this structure.
     * Only two of the three fields are required..
     */
    struct Radii
    {
        /// Equatorial radius.
        double eq_rad;

        /// Polar radius.
        double pol_rad;

        /// Flattening (a-c/a)
        double flattening;
    };

}


#include "parse.hh"

#define YY_DECL int yylex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param, MaRC::ParseParameter & pp)

YY_DECL;

void yyerror (YYLTYPE * locp, MaRC::ParseParameter & pp, char const * msg);


#endif


/*
  Local Variables:
  mode: c++
  c-basic-offset: 4
  indent-tabs-mode: nil
  End:
*/
