/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     VAR = 259,
     FNCT = 260,
     _STRING = 261,
     NEG = 262,
     _ALBEQV1 = 263,
     _ALBEQV2 = 264,
     _LAMCNF1 = 265,
     _LAMCNF2 = 266,
     _LAMCYLEQ = 267,
     _LAMPOLEQ = 268,
     _MERCATOR = 269,
     _ORTHO = 270,
     _P_STEREO = 271,
     _PERSPECTIVE = 272,
     _SIMPLE_C = 273,
     _SINUSOID = 274,
     _IDENTITY = 275,
     _MAP = 276,
     AUTHOR = 277,
     ORIGIN = 278,
     _COMMENT = 279,
     XCOMMENT = 280,
     _DATA_TYPE = 281,
     DATA_OFFSET = 282,
     DATA_SCALE = 283,
     DATA_BLANK = 284,
     GRID = 285,
     GRID_INTERVAL = 286,
     LAT_GRID_INTERVAL = 287,
     LON_GRID_INTERVAL = 288,
     MAP_TYPE = 289,
     SAMPLES = 290,
     LINES = 291,
     BODY = 292,
     PLANE = 293,
     DATA_MIN = 294,
     DATA_MAX = 295,
     PROGRADE = 296,
     RETROGRADE = 297,
     FLATTENING = 298,
     AVERAGING = 299,
     NONE = 300,
     WEIGHTED = 301,
     UNWEIGHTED = 302,
     OPTIONS = 303,
     EQ_RAD = 304,
     POL_RAD = 305,
     ROTATION = 306,
     _IMAGE = 307,
     _PHOTO = 308,
     _MU = 309,
     _MU0 = 310,
     _PHASE = 311,
     PLANES = 312,
     LO_LAT = 313,
     HI_LAT = 314,
     LO_LON = 315,
     HI_LON = 316,
     LATITUDE = 317,
     LONGITUDE = 318,
     LATITUDE_TYPE = 319,
     CENTRIC = 320,
     GRAPHIC = 321,
     LAT_AT_CENTER = 322,
     LON_AT_CENTER = 323,
     SAMPLE_OA = 324,
     LINE_OA = 325,
     STD_LAT = 326,
     STD_LAT_1 = 327,
     STD_LAT_2 = 328,
     MAX_LAT = 329,
     POLE = 330,
     NIBBLE = 331,
     NIBBLE_LEFT = 332,
     NIBBLE_RIGHT = 333,
     NIBBLE_TOP = 334,
     NIBBLE_BOTTOM = 335,
     INVERT = 336,
     HORIZONTAL = 337,
     VERTICAL = 338,
     BOTH = 339,
     _INTERPOLATE = 340,
     SAMPLE_CENTER = 341,
     LINE_CENTER = 342,
     FLAT_FIELD = 343,
     MINNAERT = 344,
     AUTO = 345,
     TABLE = 346,
     GEOM_CORRECT = 347,
     _EMI_ANG_LIMIT = 348,
     TERMINATOR = 349,
     SUPERSAMPLE_VERIFY = 350,
     SUB_OBSERV_LAT = 351,
     SUB_OBSERV_LON = 352,
     POSITION_ANGLE = 353,
     SUB_SOLAR_LAT = 354,
     SUB_SOLAR_LON = 355,
     RANGE = 356,
     _REMOVE_SKY = 357,
     FOCAL_LENGTH = 358,
     PIXEL_SCALE = 359,
     ARCSEC_PER_PIX = 360,
     KM_PER_PIXEL = 361,
     BYTE_DATA = 362,
     SHORT_DATA = 363,
     LONG_DATA = 364,
     FLOAT_DATA = 365,
     DOUBLE_DATA = 366,
     CW = 367,
     CCW = 368,
     YES = 369,
     NO = 370,
     UNMATCHED = 371,
     AU = 372,
     KM = 373
   };
#endif
#define NUM 258
#define VAR 259
#define FNCT 260
#define _STRING 261
#define NEG 262
#define _ALBEQV1 263
#define _ALBEQV2 264
#define _LAMCNF1 265
#define _LAMCNF2 266
#define _LAMCYLEQ 267
#define _LAMPOLEQ 268
#define _MERCATOR 269
#define _ORTHO 270
#define _P_STEREO 271
#define _PERSPECTIVE 272
#define _SIMPLE_C 273
#define _SINUSOID 274
#define _IDENTITY 275
#define _MAP 276
#define AUTHOR 277
#define ORIGIN 278
#define _COMMENT 279
#define XCOMMENT 280
#define _DATA_TYPE 281
#define DATA_OFFSET 282
#define DATA_SCALE 283
#define DATA_BLANK 284
#define GRID 285
#define GRID_INTERVAL 286
#define LAT_GRID_INTERVAL 287
#define LON_GRID_INTERVAL 288
#define MAP_TYPE 289
#define SAMPLES 290
#define LINES 291
#define BODY 292
#define PLANE 293
#define DATA_MIN 294
#define DATA_MAX 295
#define PROGRADE 296
#define RETROGRADE 297
#define FLATTENING 298
#define AVERAGING 299
#define NONE 300
#define WEIGHTED 301
#define UNWEIGHTED 302
#define OPTIONS 303
#define EQ_RAD 304
#define POL_RAD 305
#define ROTATION 306
#define _IMAGE 307
#define _PHOTO 308
#define _MU 309
#define _MU0 310
#define _PHASE 311
#define PLANES 312
#define LO_LAT 313
#define HI_LAT 314
#define LO_LON 315
#define HI_LON 316
#define LATITUDE 317
#define LONGITUDE 318
#define LATITUDE_TYPE 319
#define CENTRIC 320
#define GRAPHIC 321
#define LAT_AT_CENTER 322
#define LON_AT_CENTER 323
#define SAMPLE_OA 324
#define LINE_OA 325
#define STD_LAT 326
#define STD_LAT_1 327
#define STD_LAT_2 328
#define MAX_LAT 329
#define POLE 330
#define NIBBLE 331
#define NIBBLE_LEFT 332
#define NIBBLE_RIGHT 333
#define NIBBLE_TOP 334
#define NIBBLE_BOTTOM 335
#define INVERT 336
#define HORIZONTAL 337
#define VERTICAL 338
#define BOTH 339
#define _INTERPOLATE 340
#define SAMPLE_CENTER 341
#define LINE_CENTER 342
#define FLAT_FIELD 343
#define MINNAERT 344
#define AUTO 345
#define TABLE 346
#define GEOM_CORRECT 347
#define _EMI_ANG_LIMIT 348
#define TERMINATOR 349
#define SUPERSAMPLE_VERIFY 350
#define SUB_OBSERV_LAT 351
#define SUB_OBSERV_LON 352
#define POSITION_ANGLE 353
#define SUB_SOLAR_LAT 354
#define SUB_SOLAR_LON 355
#define RANGE 356
#define _REMOVE_SKY 357
#define FOCAL_LENGTH 358
#define PIXEL_SCALE 359
#define ARCSEC_PER_PIX 360
#define KM_PER_PIXEL 361
#define BYTE_DATA 362
#define SHORT_DATA 363
#define LONG_DATA 364
#define FLOAT_DATA 365
#define DOUBLE_DATA 366
#define CW 367
#define CCW 368
#define YES 369
#define NO 370
#define UNMATCHED 371
#define AU 372
#define KM 373




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 185 "parse.yy"
typedef union YYSTYPE {
  char * sval;
  double val;        // For returning numbers
  bool bval;         // For returning boolean values
  MaRC::sym_entry * tptr;  // For returning symbol-table pointers
  MaRC::SubObserv sub_observ_data; // Sub-observation point.
  MaRC::SubSolar sub_solar_data;   // Sub-solar point.
  MaRC::Radii radii_data;          // Oblate spheroid radii.
} YYSTYPE;
/* Line 1275 of yacc.c.  */
#line 283 "parse.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




