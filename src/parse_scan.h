/**
 * @file parse_scan.h
 *
 * %MaRC Parser and Scanner Declarations
 *
 * Copyright (C) 1999, 2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_PARSE_SCAN_H
#define MARC_PARSE_SCAN_H

#include "calc.h"

#include <marc/optional.h>

#include <list>
#include <memory>

/**
 * @brief Scanner function prototype.
 *
 * A non-default scanner function prototype that allows additional
 * parameters to be passed as part of the means to make the scanner
 * reentrant.
 */
#define YY_DECL int yylex(YYSTYPE * yylval_param,       \
                          YYLTYPE * yylloc_param,       \
                          yyscan_t yyscanner,           \
                          MaRC::ParseParameter & pp)

namespace MaRC
{

    class MapCommand;

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

        /// Map command list type.
        using command_list = std::list<std::unique_ptr<MapCommand>>;

        /// Constructor.
        ParseParameter();

        /// Get list of @c MapCommand objects.
        command_list const & commands() const { return this->commands_; }

        /// Push a Command object on to the list of Commands to
        /// execute.
        void push_command(std::unique_ptr<MapCommand> c);

        /// Return reference to symbol table.
        symrec & sym_table() { return this->sym_table_; }

    public:

        /// Name of file being parsed.
        char const * filename;

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
        MaRC::optional<double> minimum;

        /// Default source image maximum cutoff value.
        MaRC::optional<double> maximum;

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

    /// Sub-solar point latitude and longitude.
    using SubSolar = SubObserv;

    /**
     * @struct Radii
     *
     * @brief Structure containing oblate spheroid radii.
     *
     * Parsed radii will be stored in an instance of this structure.
     * Only two of the three fields are required.
     */
    struct Radii
    {
        /// Equatorial radius.
        double eq_rad;

        /// Polar radius.
        double pol_rad;

        /// Flattening (a-c/a)
        double flattening;

        /**
         * Make sure at least two of the three fields are set, and
         * verify that the values are suitable for use by %MaRC.
         */
        void validate();

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
