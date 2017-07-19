//   -*- C++ -*-

//==========================================================================
/**
 *  @file parse_scan.h
 *
 *  $Id: parse_scan.h,v 1.10 2004/11/08 10:26:24 othman Exp $
 *
 *  MaRC Parser and Scanner Declarations
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================


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
   * @brief Parameter passed to GNU Bison generated reentrant parser.
   *
   * This is the type passed to the GNU Bison generated reentrant
   * parser.
   */
  class ParseParameter
  {
  public:

    typedef std::list<std::unique_ptr<MapCommand>> command_list;

    /// Constructor.
    ParseParameter (void);

    command_list const & commands(void) const { return this->commands_; }

    /// Push a Command object on to the list of Commands to execute.
    void push_command (std::unique_ptr<MapCommand> c);

    /// Return reference to symbol table.
    symrec & sym_table (void) { return this->sym_table_; }

    /// Return reference to scanner.
    //     FlexLexer & lexer (void) { return this->lexer_; }

    /// Change
    void input_stream (std::istream & i);

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

    /// Default source image left   side "nibble" value.
    unsigned int nibble_left;

    /// Default source image right  side "nibble" value.
    unsigned int nibble_right;

    /// Default source image top    side "nibble" value.
    unsigned int nibble_top;

    /// Default source image bottom side "nibble" value.
    unsigned int nibble_bottom;
    //@}

  private:

    /// List of commands to execute.
    /**
     * The command list is constructed while parsing the user's defaults
     * and map input files.
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

#define YY_DECL int yylex (YYSTYPE * yylval_param, YYLTYPE * yylloc_param, MaRC::ParseParameter & pp)

YY_DECL;

int yyparse (MaRC::ParseParameter & pp);
void yyerror (YYLTYPE * locp, MaRC::ParseParameter & pp, char const * msg);

#endif
