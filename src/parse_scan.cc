// $Id: parse_scan.cc,v 1.8 2004/11/08 10:26:24 othman Exp $


#include "parse_scan.h"

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>




MaRC::ParseParameter::ParseParameter (void)
  : lat_interval (10),
    lon_interval (10),
    minimum (-std::numeric_limits<double>::max ()),
    maximum (std::numeric_limits<double>::max ()),
    nibble_left   (0),
    nibble_right  (0),
    nibble_top    (0),
    nibble_bottom (0),
    commands_ (),
    sym_table_ ()// ,
//     lexer_ ()
{
}

void
MaRC::ParseParameter::push_command (const MaRC::ValuePtr<MapCommand> & c)
{
  this->commands_.push_back (c);
}

// -------------------------------------------------------------------

// This is a wrapper function for the yyFlexLexer (the lexical
// analyzer) class.
// int
// yylex (YYSTYPE * lvalp, YYLTYPE * /* llocp */, MaRC::ParseParameter & pp)
// {
//   FlexLexer & lexer = pp.lexer ();

//   const int token = lexer.yylex ();

//   const char * symbuf = 0;
//   MaRC::sym_entry *s = 0;

//   switch (token)
//     {
//     case NUM:
//       // Numbers will be handled in double precision
//       lvalp->val = ::strtod (lexer.YYText (), 0);
//       break;

//     case VAR:
//       symbuf = lexer.YYText ();
//       s = pp.sym_table ().getsym (symbuf);

//       if (s == 0)
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
  std::cerr << msg << std::endl;
}
