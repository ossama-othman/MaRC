/**
 * @file parse_scan.cpp
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

#include "parse_scan.h"

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>


MaRC::ParseParameter::ParseParameter (void)
    : lat_interval(10)
    , lon_interval(10)
    , minimum(std::numeric_limits<decltype(this->minimum)>::lowest())
    , maximum(std::numeric_limits<decltype(this->maximum)>::max())
    , nibble_left  (0)
    , nibble_right (0)
    , nibble_top   (0)
    , nibble_bottom(0)
    , commands_()
    , sym_table_()
      // , lexer_()
{
}

void
MaRC::ParseParameter::push_command(std::unique_ptr<MapCommand> c)
{
    this->commands_.push_back(std::move(c));
}

// -------------------------------------------------------------------

// This is a wrapper function for the yyFlexLexer (the lexical
// analyzer) class.
// int
// yylex (YYSTYPE * lvalp, YYLTYPE * /* llocp */, MaRC::ParseParameter & pp)
// {
//   FlexLexer & lexer = pp.lexer ();

//   const int token = lexer.yylex ();

//   const char * symbuf = nullptr;
//   MaRC::sym_entry *s = nullptr;

//   switch (token)
//     {
//     case NUM:
//       // Numbers will be handled in double precision
//       lvalp->val = ::strtod (lexer.YYText (), 0);
//       break;

//     case VAR:
//       symbuf = lexer.YYText ();
//       s = pp.sym_table ().getsym (symbuf);

//       if (s == nullptr)
//         {
//           /*
//             This forces parsing of variables to be shut off.
//             Functions may still be used.  Until the inability of the
//             parser to handle two or more seperate expressions in a row
//             is resolved the "return UNMATCHED" should remain.
//             e.g.: m = 2  m+2
//           */

//           return UNMATCHED;

//           // s = pp.symrec ()->putsym (symbuf, VAR);
//         }

//       lvalp->tptr = s;

//       // return s->type;  // This returns either FNCT or VAR
//       return FNCT;

//     case _STRING:
//       lvalp->sval = strdup (lexer.YYText ());
//       break;

//     default:
//       return token;  /* Any other character is a token by itself. */
//     }

//   return token;
// }

void
yyerror (YYLTYPE * /* locp */,
         MaRC::ParseParameter & /* pp */,
         char const * msg)
{
    /**
     * @todo Pull location from @a locp argument.
     */
    std::cerr << msg << std::endl;
}
