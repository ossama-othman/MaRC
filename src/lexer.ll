/*  -*- C++ -*-
Copyright © 1996-1999  Ossama Othman

All Rights Reserved


This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


*/

/*
Scanner for MaRC input files
Requires GNU Flex 2.5.4a or greater
*/

%{

#include "parse_scan.h"
#include "parse.hh" // Parser definitions

int yycolumn = 1;

#define YY_USER_ACTION {                             \
        yylloc->first_line = yylineno;               \
        yylloc->last_line = yylineno;                \
        yylloc->first_column = yycolumn;             \
        yylloc->last_column = yycolumn + yyleng - 1; \
 	yycolumn += yyleng; }
%}

%option noyywrap
%option case-insensitive
%option bison-bridge bison-locations
%option yylineno
%option nounput
%s comment_init
%x comment
%s keyword_token
%x expression
%s string
%s pole

%%
<comment_init>[:]	{
			  BEGIN(comment); return ':';
			  /* This colon is seperated from <comment> so
			     colons may be placed in side of comments */
			}
<comment>[^\n]*		{
  BEGIN(INITIAL); yylval->sval = strdup (yytext); return _STRING;
  /* Comments must end with a carriage return */
			}
<keyword_token>[:]	{ BEGIN(INITIAL); return ':'; }
<string>{
	[:]		return ':';
	[[:graph:]]+	BEGIN(INITIAL); yylval->sval = strdup (yytext) ;return _STRING;
}
<pole>{
        [:]             return ':';
	N               BEGIN(INITIAL); return 'N';
	"NORTH"         BEGIN(INITIAL); return 'N';
	S               BEGIN(INITIAL); return 'S';
	"SOUTH"         BEGIN(INITIAL); return 'S';
}

[:]			{ BEGIN(expression); return ':'; }
<expression>{
	#[^\n]*		/* eat up one line text comments in MaRC input file */
	[ \t]+		/* eat up whitespace except for carriage return */
	"AUTO"                          { BEGIN(INITIAL); return AUTO; }
	"TABLE"                         { BEGIN(INITIAL); return TABLE; }
	\=				{ return '='; }
	\+				{ return '+'; }
	\-				{ return '-'; }
	\*				{ return '*'; }
	\/				{ return '/'; }
	\^				{ return '^'; }
	\(				{ return '('; }
	\)				{ return ')'; }
	C				{ return 'C'; }
	G				{ return 'G'; }
	N				{ return 'N'; }
	S				{ return 'S'; }
	E				{ return 'E'; }
	W				{ return 'W'; }
	"AU"				{ return AU;  }
	"KM"				{ return KM;  }
	"CW"                            { return CW;  }
	"CCW"                           { return CCW; }
        [[:digit:]]*("."[[:digit:]]*)?(E[-+]?[[:digit:]]{1,3})?          {
            // Numbers will be handled in double precision
          yylval->val = strtod (yytext, 0);
            return NUM;
          }
	[[:alpha:]]+[[:digit:]]*          {
            MaRC::sym_entry * const s = pp.sym_table ().getsym (yytext);

            if (s == 0)
              {
                /*
                  This forces parsing of variables to be shut off.
                  Functions may still be used.  Until the inability of
                  the parser to handle two or more seperate
                  expressions in a row is resolved the "return
                  UNMATCHED" should remain.
                  e.g.: m = 2  m+2
                */

                return UNMATCHED;

                // s = pp.symrec ()->putsym (symbuf, VAR);
              }

            yylval->tptr = s;

            return s->type;  // This returns either FNCT or VAR.
          }
	.               {
	                        BEGIN(INITIAL); return UNMATCHED;
				/* any character (byte) except newline */
	                }
	\n		{
				BEGIN(INITIAL);
				/* Mathematical expressions
                                   must end with a carriage return */
			}
}

"ALBEQV1"	{ return _ALBEQV1; }
"ALBEQV2"	{ return _ALBEQV2; }
"IDENTITY"      { return _IDENTITY; }
"LAMCNF1"	{ return _LAMCNF1; }
"LAMCNF2"	{ return _LAMCNF2; }
"LAMCYLEQ"	{ return _LAMCYLEQ; }
"LAMPOLEQ"	{ return _LAMPOLEQ; }
"MERCATOR"	{ return _MERCATOR; }
"ORTHO"		{ return _ORTHO; }
"P_STEREO"	{ return _P_STEREO; }
"PERSPECTIVE"   { return _PERSPECTIVE; }
"SIMPLE_C"	{ return _SIMPLE_C; }
"SINUSOID"	{ return _SINUSOID; }

"MAP"		{ BEGIN(string); return _MAP; }
"AUTHOR"        { BEGIN(comment_init); return AUTHOR; }
"ORIGIN"        { BEGIN(comment_init); return ORIGIN; }
"COMMENT"	{ BEGIN(comment_init); return _COMMENT; }
"XCOMMENT"	{ BEGIN(comment_init); return XCOMMENT; }
"DATA_OFFSET"	{ return DATA_OFFSET; }
"DATA_SCALE"	{ return DATA_SCALE; }
"DATA_TYPE"	{ BEGIN(keyword_token);  return _DATA_TYPE; }
"DATA_BLANK"    { return DATA_BLANK; }
"GRID"		{ BEGIN(keyword_token); return GRID; }
"GRID_INTERVAL"	{ return GRID_INTERVAL; }
"LAT_GRID_INTERVAL"	{ return LAT_GRID_INTERVAL; }
"LON_GRID_INTERVAL"	{ return LON_GRID_INTERVAL; }
"TYPE"		{ BEGIN(keyword_token); return MAP_TYPE; }
"PROJECTION"	{ BEGIN(keyword_token); return MAP_TYPE; }
"SAMPLES"	{ return SAMPLES; }
"LINES"		{ return LINES; }
"BODY"		{ BEGIN(string); return BODY; }
"AVERAGING"     { BEGIN(keyword_token); return AVERAGING; }
"NONE"          { return NONE; }
"WEIGHTED"      { return WEIGHTED; }
"UNWEIGHTED"    { return UNWEIGHTED; }
"PLANES"	{ return PLANES; }
"PROGRADE"	{ return PROGRADE; }
"RETROGRADE"	{ return RETROGRADE; }
"FLATTENING"	{ return FLATTENING; }
"OPTIONS"	{ BEGIN(keyword_token);  return OPTIONS; }
"EQ_RAD"	{ return EQ_RAD; }
"POL_RAD"	{ return POL_RAD; }
"ROTATION"	{ BEGIN(keyword_token); return ROTATION; }
"IMAGE"		{ BEGIN(string); return _IMAGE; }
"PHOTO"		{ return _PHOTO; }
"MU"		{ BEGIN(keyword_token); return _MU; }
"MU0"		{ BEGIN(keyword_token); return _MU0; }
"PHASE"		{ BEGIN(keyword_token); return _PHASE; }
"PLANE"		{ return PLANE; }
"DATA_MIN"	{ return DATA_MIN; }
"DATA_MAX"	{ return DATA_MAX; }
"LO_LAT"	{ return LO_LAT; }
"HI_LAT"	{ return HI_LAT; }
"LO_LON"	{ return LO_LON; }
"HI_LON"	{ return HI_LON; }
"LATITUDE_TYPE" { BEGIN(keyword_token); return LATITUDE_TYPE; }
"CENTRIC_LAT"|"CENTRIC"	{ return CENTRIC; }
"GRAPHIC_LAT"|"GRAPHIC"	{ return GRAPHIC; }
"LATITUDE"      { BEGIN(keyword_token); return LATITUDE; }
"LONGITUDE"     { return LONGITUDE; }
"POLE"          { BEGIN(pole); return POLE; }
"MAX_LAT"       { return MAX_LAT; }
"STD_LAT"       { return STD_LAT; }
"STD_LAT_1"     { return STD_LAT_1; }
"STD_LAT_2"     { return STD_LAT_2; }
"INTERPOLATE"	{ BEGIN(keyword_token); return _INTERPOLATE; }
"REMOVE_SKY"    { BEGIN(keyword_token); return _REMOVE_SKY; }
"NIBBLE"	{ return NIBBLE; }
"NIBBLE_LEFT"	{ return NIBBLE_LEFT; }
"NIBBLE_RIGHT"	{ return NIBBLE_RIGHT; }
"NIBBLE_TOP"	{ return NIBBLE_TOP; }
"NIBBLE_BOTTOM"	{ return NIBBLE_BOTTOM; }
"INVERT"        { BEGIN(keyword_token); return INVERT; }
"VERTICAL"      { return VERTICAL; }
"HORIZONTAL"    { return HORIZONTAL; }
"BOTH"          { return BOTH; }
"SAMPLE_CENTER"	{ return SAMPLE_CENTER; }
"LINE_CENTER"	{ return LINE_CENTER; }
"LAT_AT_CENTER"	{ return LAT_AT_CENTER; }
"LON_AT_CENTER"	{ return LON_AT_CENTER; }
"SAMPLE_OA"     { return SAMPLE_OA; }
"LINE_OA"       { return LINE_OA; }
"FLAT_FIELD"	{ BEGIN(string); return FLAT_FIELD; }
"MINNAERT"      { return MINNAERT; }
"AUTO"          { return AUTO; }
"TABLE"         { return TABLE; }
"GEOM_CORRECT"	{ BEGIN(keyword_token); return GEOM_CORRECT; }
"EMI_ANG_LIMIT" { return _EMI_ANG_LIMIT; }
"TERMINATOR"    { BEGIN(keyword_token); return TERMINATOR; }
"SUB_OBSERV_LAT"	{ return SUB_OBSERV_LAT; }
"SUB_OBSERV_LON"	{ return SUB_OBSERV_LON; }
"POSITION_ANGLE"	{ return POSITION_ANGLE; }
"SUB_SOLAR_LAT"		{ return SUB_SOLAR_LAT; }
"SUB_SOLAR_LON"		{ return SUB_SOLAR_LON; }
"RANGE"		{ return RANGE; }
"FOCAL_LENGTH"	{ return FOCAL_LENGTH; }
"PIXEL_SCALE"	{ return PIXEL_SCALE; }
"ARCSEC_PER_PIX"	{ return ARCSEC_PER_PIX; }
"KM_PER_PIXEL"	{ return KM_PER_PIXEL; }
"BYTE"		{ return BYTE_DATA; }
"SHORT"		{  return SHORT_DATA; }
"LONG"		{ return LONG_DATA; }
"FLOAT"		{ return FLOAT_DATA; }
"DOUBLE"	{ return DOUBLE_DATA; }
"YES"|"Y"	{ return YES; }
"NO"|"N"	{ return NO; }
#[^\n]*		/* eat up one line text comments in MaRC input file */
[ \t\n]+		/* eat up whitespace */
<*>.		{ return UNMATCHED; }
