/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1



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
     SUB_OBSERV_LAT = 350,
     SUB_OBSERV_LON = 351,
     POSITION_ANGLE = 352,
     SUB_SOLAR_LAT = 353,
     SUB_SOLAR_LON = 354,
     RANGE = 355,
     _REMOVE_SKY = 356,
     FOCAL_LENGTH = 357,
     PIXEL_SCALE = 358,
     ARCSEC_PER_PIX = 359,
     KM_PER_PIXEL = 360,
     BYTE_DATA = 361,
     SHORT_DATA = 362,
     LONG_DATA = 363,
     FLOAT_DATA = 364,
     DOUBLE_DATA = 365,
     CW = 366,
     CCW = 367,
     YES = 368,
     NO = 369,
     UNMATCHED = 370,
     AU = 371,
     KM = 372
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
#define SUB_OBSERV_LAT 350
#define SUB_OBSERV_LON 351
#define POSITION_ANGLE 352
#define SUB_SOLAR_LAT 353
#define SUB_SOLAR_LON 354
#define RANGE 355
#define _REMOVE_SKY 356
#define FOCAL_LENGTH 357
#define PIXEL_SCALE 358
#define ARCSEC_PER_PIX 359
#define KM_PER_PIXEL 360
#define BYTE_DATA 361
#define SHORT_DATA 362
#define LONG_DATA 363
#define FLOAT_DATA 364
#define DOUBLE_DATA 365
#define CW 366
#define CCW 367
#define YES 368
#define NO 369
#define UNMATCHED 370
#define AU 371
#define KM 372




/* Copy the first part of user declarations.  */
#line 11 "parse.yy"

  // BodyData strategies
#include <MaRC/OblateSpheroid.h>

  // SourceImage factories
#include "PhotoImageFactory.h"
#include "MosaicImageFactory.h"
#include "MuImageFactory.h"
#include "Mu0ImageFactory.h"
#include "CosPhaseImageFactory.h"
#include "LatitudeImageFactory.h"
#include "LongitudeImageFactory.h"

  // SourceImage strategies
#include <MaRC/PhotoImage.h>
#include <MaRC/MosaicImage.h>
#include <MaRC/MuImage.h>
#include <MaRC/Mu0Image.h>
#include <MaRC/CosPhaseImage.h>
#include <MaRC/LatitudeImage.h>
#include <MaRC/LongitudeImage.h>

  // Geometric correction strategies
#include <MaRC/NullGeometricCorrection.h>
#include <MaRC/GLLGeometricCorrection.h>

// Photometric correction strategies
#include <MaRC/NullPhotometricCorrection.h>
  //#include <MaRC/MinnaertPhotometricCorrection.h>

// Map Strategies
#include <MaRC/Mercator.h>
#include <MaRC/Orthographic.h>
#include <MaRC/PolarStereographic.h>
#include <MaRC/SimpleCylindrical.h>

#include "MaRC/ValuePtr.h"

#include <MaRC/Constants.h>
#include <MaRC/NaN.h>

#include "parse_scan.h"
#include "calc.h"
#include "MapCommand.h"
#include "MapCommand_T.h"

#include <stdexcept>
#include <cstring>
#include <cmath>
#include <limits>
#include <cassert>

  std::string map_filename;

  std::string body_name;
  MaRC::ValuePtr<MaRC::OblateSpheroid> oblate_spheroid;

  std::string map_author;
  std::string map_origin;

  std::list<std::string> comment_list;
  std::list<std::string> xcomment_list;

  typedef enum { BYTE, SHORT, LONG, FLOAT, DOUBLE } data_type;

  data_type map_data_type;

  std::auto_ptr<MaRC::MapFactory<unsigned char> > map_factory_byte;
  std::auto_ptr<MaRC::MapFactory<short> >         map_factory_short;
  std::auto_ptr<MaRC::MapFactory<MaRC::Long> >    map_factory_long;
  std::auto_ptr<MaRC::MapFactory<float> >         map_factory_float;
  std::auto_ptr<MaRC::MapFactory<double> >        map_factory_double;

  unsigned int map_samples = 0;
  unsigned int map_lines = 0;

  bool transform_data = false;
  float fits_bzero  = 0;
  float fits_bscale = 1;

  bool blank_set = false;
  int   fits_blank  = 0;

  // To create a grid or not to create a grid?  That is the question.
  bool create_grid = false;
  float lat_interval;
  float lon_interval;

  MaRC::ValuePtr<MaRC::ImageFactory> image_factory;

  MaRC::ValuePtr<MaRC::PhotoImageFactory> photo_factory;
  MaRC::MosaicImageFactory::list_type photo_factories;
  MaRC::MosaicImage::average_type averaging_type =
    MaRC::MosaicImage::AVG_WEIGHTED;

  MaRC::MapCommand::image_factories_type image_factories;

  bool north_pole = true;

  // Current line number in stream.
  //int lineno = 0;

  // Temporary storage for plane number
  unsigned int plane_num = 0;

  // Number of planes to be expected
  unsigned int num_planes = 0;

  // Actual number of planes read
  unsigned int planes_queued = 0;

  // Used to ensure num_planes are defined in sequence
  unsigned int expected_plane = 1;

  // Number of samples in a given photo image.  Only used to determine
  // whether Galileo images were written summation mode.
  unsigned int photo_samples = 0;

  double minimum = MaRC::NaN;
  double maximum = MaRC::NaN;

  unsigned int nibble_left_val;
  unsigned int nibble_right_val;
  unsigned int nibble_top_val;
  unsigned int nibble_bottom_val;

  double sample_center = MaRC::NaN;
  double line_center   = MaRC::NaN;
  double lat_at_center = MaRC::NaN;
  double lon_at_center = MaRC::NaN;

  double km_per_pixel_val   = -1;
  double arcsec_per_pix_val = -1;
  double focal_length_val   = -1;
  double pixel_scale_val    = -1;

//   double _range;  // Observer to target center distance

  bool graphic_lat = false;

  // Conformal projection options
  double max_lat = MaRC::NaN;

  // Simple Cylindrical projection options
  double lo_lat = -90;
  double hi_lat = 90;
  double lo_lon = 0;
  double hi_lon = 360;

  // Orthographic and Perspective projection options

  // Sub-observer latitude and longitude.
  MaRC::SubObserv sub_observation_data = { MaRC::NaN, MaRC::NaN };

  double position_angle_val = -1;   // Position (north) angle
  MaRC::OrthographicCenter ortho_center;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 182 "parse.yy"
typedef union YYSTYPE {
  char * sval;
  double val;        // For returning numbers
  bool bval;         // For returning boolean values
  MaRC::sym_entry * tptr;  // For returning symbol-table pointers
  MaRC::SubObserv sub_observ_data; // Sub-observation point.
  MaRC::SubSolar sub_solar_data;   // Sub-solar point.
  MaRC::Radii radii_data;          // Oblate spheroid radii.
} YYSTYPE;
/* Line 190 of yacc.c.  */
#line 479 "parse.cc"
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


/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 503 "parse.cc"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYLTYPE_IS_TRIVIAL) && YYLTYPE_IS_TRIVIAL \
             && defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   467

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  100
/* YYNRULES -- Number of rules. */
#define YYNRULES  221
/* YYNRULES -- Number of states. */
#define YYNSTATES  449

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   372

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     131,   132,    10,     9,     2,     8,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   124,     2,
       2,     7,     2,     2,     2,     2,     2,   127,     2,   129,
       2,   128,     2,     2,     2,     2,     2,     2,   125,     2,
       2,     2,     2,   126,     2,     2,     2,   130,     2,     2,
       2,     2,     2,     2,    13,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,    12,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     7,    11,    13,    16,    29,    33,
      34,    38,    39,    43,    46,    47,    50,    54,    55,    58,
      62,    67,    68,    72,    73,    77,    81,    85,    89,    93,
      97,    98,   102,   105,   109,   113,   114,   116,   118,   120,
     123,   126,   130,   134,   138,   140,   142,   144,   146,   147,
     151,   155,   159,   161,   167,   171,   175,   178,   181,   184,
     187,   190,   193,   197,   201,   205,   207,   210,   214,   218,
     221,   222,   226,   230,   237,   244,   246,   248,   250,   252,
     254,   256,   258,   261,   279,   283,   284,   286,   288,   290,
     293,   296,   300,   302,   304,   307,   310,   312,   314,   317,
     320,   324,   328,   332,   336,   337,   341,   345,   349,   350,
     354,   358,   359,   363,   367,   373,   379,   385,   389,   391,
     392,   396,   397,   401,   405,   409,   410,   414,   418,   419,
     423,   424,   428,   432,   436,   441,   446,   448,   450,   452,
     456,   459,   462,   466,   470,   474,   479,   480,   483,   485,
     487,   489,   491,   494,   498,   503,   510,   517,   524,   531,
     535,   540,   545,   549,   551,   553,   560,   567,   574,   581,
     582,   589,   596,   601,   602,   606,   611,   612,   616,   620,
     624,   626,   628,   629,   631,   633,   636,   639,   646,   653,
     660,   667,   670,   673,   675,   676,   680,   684,   688,   689,
     693,   697,   701,   703,   706,   709,   711,   714,   717,   719,
     722,   725,   727,   729,   733,   738,   742,   746,   750,   754,
     757,   761
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     134,     0,    -1,   136,    -1,   135,    -1,   153,   171,   176,
      -1,   137,    -1,   137,   136,    -1,   138,   139,   140,   141,
     162,   146,   151,   157,   158,   159,   160,   168,    -1,    27,
     124,     6,    -1,    -1,    28,   124,     6,    -1,    -1,    29,
     124,     6,    -1,   142,   144,    -1,    -1,   143,   142,    -1,
      30,   124,     6,    -1,    -1,   145,   144,    -1,    31,   124,
       6,    -1,   147,   148,   149,   150,    -1,    -1,    33,   124,
     161,    -1,    -1,    34,   124,   161,    -1,    32,   124,   112,
      -1,    32,   124,   113,    -1,    32,   124,   114,    -1,    32,
     124,   115,    -1,    32,   124,   116,    -1,    -1,    35,   124,
     232,    -1,   152,   153,    -1,    36,   124,   119,    -1,    36,
     124,   120,    -1,    -1,   154,    -1,   155,    -1,   156,    -1,
     155,   156,    -1,   156,   155,    -1,    37,   124,   161,    -1,
      38,   124,   161,    -1,    39,   124,   161,    -1,   203,    -1,
     204,    -1,   215,    -1,   217,    -1,    -1,    63,   124,   161,
      -1,    41,   124,   161,    -1,    42,   124,   161,    -1,   232,
      -1,    43,   124,     6,   164,   163,    -1,    57,   124,    47,
      -1,    57,   124,    48,    -1,   165,   166,    -1,   166,   165,
      -1,   165,   167,    -1,   167,   165,    -1,   166,   167,    -1,
     167,   166,    -1,    55,   124,   232,    -1,    56,   124,   232,
      -1,    49,   124,   232,    -1,   169,    -1,   169,   168,    -1,
     170,   171,   172,    -1,    44,   124,   161,    -1,    44,   124,
      -1,    -1,    45,   124,   161,    -1,    46,   124,   161,    -1,
      45,   124,   161,    46,   124,   161,    -1,    46,   124,   161,
      45,   124,   161,    -1,   173,    -1,   187,    -1,   188,    -1,
     189,    -1,   190,    -1,   191,    -1,   174,    -1,   174,   173,
      -1,   175,   176,   184,   185,   186,   211,   214,   192,   193,
     194,   195,   196,   207,   209,   208,   197,   198,    -1,    58,
     124,     6,    -1,    -1,   177,    -1,   178,    -1,   179,    -1,
     178,   179,    -1,   179,   178,    -1,    82,   124,   161,    -1,
     180,    -1,   181,    -1,   180,   181,    -1,   181,   180,    -1,
     182,    -1,   183,    -1,   182,   183,    -1,   183,   182,    -1,
      83,   124,   161,    -1,    84,   124,   161,    -1,    85,   124,
     161,    -1,    86,   124,   161,    -1,    -1,    87,   124,    89,
      -1,    87,   124,    88,    -1,    87,   124,    90,    -1,    -1,
      91,   124,   119,    -1,    91,   124,   120,    -1,    -1,   107,
     124,   119,    -1,   107,   124,   120,    -1,    60,   124,   207,
     197,   208,    -1,    61,   124,   207,   197,   208,    -1,    62,
     124,   207,   197,   208,    -1,    68,   124,   219,    -1,    69,
      -1,    -1,    94,   124,     6,    -1,    -1,    95,   124,   232,
      -1,    95,   124,    96,    -1,    95,   124,    97,    -1,    -1,
      98,   124,   119,    -1,    98,   124,   120,    -1,    -1,    99,
     124,   232,    -1,    -1,   100,   124,   119,    -1,   100,   124,
     120,    -1,   106,   124,   232,    -1,   106,   124,   232,   123,
      -1,   106,   124,   232,   122,    -1,   210,    -1,   199,    -1,
     200,    -1,   110,   124,   232,    -1,   201,   202,    -1,   202,
     201,    -1,   108,   124,   232,    -1,   109,   124,   232,    -1,
      40,   124,    20,    -1,    40,   124,    21,   205,    -1,    -1,
     223,   206,    -1,   207,    -1,   209,    -1,   210,    -1,   211,
      -1,   207,   209,    -1,   207,   209,   210,    -1,   207,   209,
     210,   211,    -1,   101,   124,   229,   102,   124,   231,    -1,
     102,   124,   231,   101,   124,   229,    -1,   104,   124,   229,
     105,   124,   231,    -1,   105,   124,   231,   104,   124,   229,
      -1,   103,   124,   232,    -1,   103,   124,   232,   117,    -1,
     103,   124,   232,   118,    -1,   111,   124,   232,    -1,   212,
      -1,   213,    -1,    92,   124,   232,    93,   124,   232,    -1,
      93,   124,   232,    92,   124,   232,    -1,    73,   124,   229,
      74,   124,   231,    -1,    74,   124,   231,    73,   124,   229,
      -1,    -1,    75,   124,   232,    76,   124,   232,    -1,    76,
     124,   232,    75,   124,   232,    -1,    40,   124,    22,   216,
      -1,    -1,   223,   228,   227,    -1,    40,   124,    24,   218,
      -1,    -1,   223,   219,   220,    -1,    70,   124,    71,    -1,
      70,   124,    72,    -1,    71,    -1,    72,    -1,    -1,   221,
      -1,   222,    -1,   221,   222,    -1,   222,   221,    -1,    64,
     124,   229,    65,   124,   229,    -1,    65,   124,   229,    64,
     124,   229,    -1,    66,   124,   231,    67,   124,   231,    -1,
      67,   124,   231,    66,   124,   231,    -1,   224,   225,    -1,
      54,   124,    -1,   226,    -1,    -1,    50,   124,    53,    -1,
      50,   124,    52,    -1,    50,   124,    51,    -1,    -1,    80,
     124,   229,    -1,    81,   124,   125,    -1,    81,   124,   126,
      -1,   230,    -1,   230,   127,    -1,   230,   128,    -1,   232,
      -1,   232,   125,    -1,   232,   126,    -1,   232,    -1,   232,
     129,    -1,   232,   130,    -1,     3,    -1,     4,    -1,     4,
       7,   232,    -1,     5,   131,   232,   132,    -1,   232,     9,
     232,    -1,   232,     8,   232,    -1,   232,    10,   232,    -1,
     232,    11,   232,    -1,     8,   232,    -1,   232,    13,   232,
      -1,   131,   232,   132,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   245,   245,   246,   250,   271,   272,   276,   394,   417,
     418,   421,   422,   426,   430,   431,   435,   438,   439,   443,
     448,   454,   455,   461,   462,   469,   470,   471,   472,   473,
     476,   477,   491,   496,   501,   504,   505,   506,   507,   508,
     509,   513,   531,   546,   572,   573,   574,   575,   578,   578,
     597,   608,   619,   622,   638,   641,   646,   652,   658,   664,
     670,   676,   685,   689,   693,   696,   697,   701,   714,   761,
     764,   765,   766,   767,   782,   801,   802,   803,   804,   805,
     806,   811,   814,   823,   899,   908,   909,   910,   911,   912,
     913,   917,   934,   935,   936,   937,   942,   943,   944,   945,
     949,   963,   977,   991,  1004,  1005,  1006,  1007,  1010,  1012,
    1013,  1016,  1017,  1018,  1021,  1035,  1048,  1064,  1076,  1084,
    1085,  1088,  1089,  1090,  1092,  1096,  1099,  1103,  1108,  1109,
    1112,  1113,  1114,  1117,  1118,  1119,  1123,  1124,  1125,  1129,
    1133,  1135,  1140,  1144,  1270,  1306,  1384,  1386,  1391,  1395,
    1396,  1397,  1415,  1420,  1425,  1451,  1456,  1464,  1466,  1471,
    1472,  1482,  1495,  1499,  1500,  1504,  1509,  1517,  1529,  1543,
    1544,  1546,  1583,  1633,  1635,  1642,  1712,  1714,  1720,  1721,
    1723,  1724,  1727,  1728,  1729,  1730,  1731,  1735,  1745,  1758,
    1763,  1788,  1793,  1797,  1800,  1801,  1803,  1805,  1860,  1861,
    1864,  1865,  1869,  1870,  1871,  1878,  1885,  1893,  1904,  1917,
    1933,  1953,  1954,  1955,  1956,  1957,  1958,  1959,  1960,  1973,
    1974,  1975
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "VAR", "FNCT", "_STRING", "'='",
  "'-'", "'+'", "'*'", "'/'", "NEG", "'^'", "_ALBEQV1", "_ALBEQV2",
  "_LAMCNF1", "_LAMCNF2", "_LAMCYLEQ", "_LAMPOLEQ", "_MERCATOR", "_ORTHO",
  "_P_STEREO", "_PERSPECTIVE", "_SIMPLE_C", "_SINUSOID", "_IDENTITY",
  "_MAP", "AUTHOR", "ORIGIN", "_COMMENT", "XCOMMENT", "_DATA_TYPE",
  "DATA_OFFSET", "DATA_SCALE", "DATA_BLANK", "GRID", "GRID_INTERVAL",
  "LAT_GRID_INTERVAL", "LON_GRID_INTERVAL", "MAP_TYPE", "SAMPLES", "LINES",
  "BODY", "PLANE", "DATA_MIN", "DATA_MAX", "PROGRADE", "RETROGRADE",
  "FLATTENING", "AVERAGING", "NONE", "WEIGHTED", "UNWEIGHTED", "OPTIONS",
  "EQ_RAD", "POL_RAD", "ROTATION", "_IMAGE", "_PHOTO", "_MU", "_MU0",
  "_PHASE", "PLANES", "LO_LAT", "HI_LAT", "LO_LON", "HI_LON", "LATITUDE",
  "LONGITUDE", "LATITUDE_TYPE", "CENTRIC", "GRAPHIC", "LAT_AT_CENTER",
  "LON_AT_CENTER", "SAMPLE_OA", "LINE_OA", "STD_LAT", "STD_LAT_1",
  "STD_LAT_2", "MAX_LAT", "POLE", "NIBBLE", "NIBBLE_LEFT", "NIBBLE_RIGHT",
  "NIBBLE_TOP", "NIBBLE_BOTTOM", "INVERT", "HORIZONTAL", "VERTICAL",
  "BOTH", "_INTERPOLATE", "SAMPLE_CENTER", "LINE_CENTER", "FLAT_FIELD",
  "MINNAERT", "AUTO", "TABLE", "GEOM_CORRECT", "_EMI_ANG_LIMIT",
  "TERMINATOR", "SUB_OBSERV_LAT", "SUB_OBSERV_LON", "POSITION_ANGLE",
  "SUB_SOLAR_LAT", "SUB_SOLAR_LON", "RANGE", "_REMOVE_SKY", "FOCAL_LENGTH",
  "PIXEL_SCALE", "ARCSEC_PER_PIX", "KM_PER_PIXEL", "BYTE_DATA",
  "SHORT_DATA", "LONG_DATA", "FLOAT_DATA", "DOUBLE_DATA", "CW", "CCW",
  "YES", "NO", "UNMATCHED", "AU", "KM", "':'", "'N'", "'S'", "'C'", "'G'",
  "'E'", "'W'", "'('", "')'", "$accept", "map", "user_file_parse",
  "map_parse", "map_setup", "map_entry", "author", "origin", "comments",
  "comment", "comment_setup", "xcomment", "xcomment_setup", "data_info",
  "data_offset", "data_scale", "data_type", "data_blank", "grid",
  "grid_yes_or_no", "grid_intervals", "grid_interval", "lat_grid_interval",
  "lon_grid_interval", "projection_type", "planes", "samples", "lines",
  "size", "body", "rotation", "radii", "eq_rad", "pol_rad", "flattening",
  "plane", "plane_setup", "plane_size", "plane_data_range", "plane_type",
  "image", "image_setup", "image_initialize", "nibbling", "nibble",
  "nibble_samples", "nibble_lines", "nibble_left", "nibble_right",
  "nibble_top", "nibble_bottom", "inversion", "image_interpolate",
  "remove_sky", "mu", "mu0", "phase", "lat_plane", "lon_plane",
  "flat_field", "photo_correct", "geom_correct", "emi_ang_limit",
  "terminator", "range", "image_geometry", "arcsec_per_pix",
  "lens_geometry", "focal_length", "pixel_scale", "mercator", "ortho",
  "ortho_options", "ortho_optsub", "sub_observ", "sub_solar",
  "position_angle", "km_per_pixel", "centers", "center_given",
  "lat_lon_given", "optical_axis", "p_stereo", "p_stereo_options",
  "simple_c", "simple_c_options", "lat_type", "simple_c_latlonrange",
  "lat_range", "lon_range", "options", "options_keyword", "options_common",
  "averaging", "max_latitude", "pole", "latitude", "latitude_sub",
  "longitude", "expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,    61,    45,    43,
      42,    47,   262,    94,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,    58,    78,    83,    67,    71,    69,
      87,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   133,   134,   134,   135,   136,   136,   137,   138,   139,
     139,   140,   140,   141,   142,   142,   143,   144,   144,   145,
     146,   147,   147,   148,   148,   149,   149,   149,   149,   149,
     150,   150,   151,   152,   152,   153,   153,   153,   153,   153,
     153,   154,   155,   156,   157,   157,   157,   157,   158,   158,
     159,   160,   161,   162,   163,   163,   164,   164,   164,   164,
     164,   164,   165,   166,   167,   168,   168,   169,   170,   170,
     171,   171,   171,   171,   171,   172,   172,   172,   172,   172,
     172,   173,   173,   174,   175,   176,   176,   176,   176,   176,
     176,   177,   178,   178,   178,   178,   179,   179,   179,   179,
     180,   181,   182,   183,   184,   184,   184,   184,   185,   185,
     185,   186,   186,   186,   187,   188,   189,   190,   191,   192,
     192,   193,   193,   193,   193,   194,   194,   194,   195,   195,
     196,   196,   196,   197,   197,   197,   198,   198,   198,   199,
     200,   200,   201,   202,   203,   204,   205,   205,   206,   206,
     206,   206,   206,   206,   206,   207,   207,   208,   208,   209,
     209,   209,   210,   211,   211,   212,   212,   213,   213,   214,
     214,   214,   215,   216,   216,   217,   218,   218,   219,   219,
     219,   219,   220,   220,   220,   220,   220,   221,   221,   222,
     222,   223,   224,   225,   226,   226,   226,   226,   227,   227,
     228,   228,   229,   229,   229,   230,   230,   230,   231,   231,
     231,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     3,     1,     2,    12,     3,     0,
       3,     0,     3,     2,     0,     2,     3,     0,     2,     3,
       4,     0,     3,     0,     3,     3,     3,     3,     3,     3,
       0,     3,     2,     3,     3,     0,     1,     1,     1,     2,
       2,     3,     3,     3,     1,     1,     1,     1,     0,     3,
       3,     3,     1,     5,     3,     3,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     1,     2,     3,     3,     2,
       0,     3,     3,     6,     6,     1,     1,     1,     1,     1,
       1,     1,     2,    17,     3,     0,     1,     1,     1,     2,
       2,     3,     1,     1,     2,     2,     1,     1,     2,     2,
       3,     3,     3,     3,     0,     3,     3,     3,     0,     3,
       3,     0,     3,     3,     5,     5,     5,     3,     1,     0,
       3,     0,     3,     3,     3,     0,     3,     3,     0,     3,
       0,     3,     3,     3,     4,     4,     1,     1,     1,     3,
       2,     2,     3,     3,     3,     4,     0,     2,     1,     1,
       1,     1,     2,     3,     4,     6,     6,     6,     6,     3,
       4,     4,     3,     1,     1,     6,     6,     6,     6,     0,
       6,     6,     4,     0,     3,     4,     0,     3,     3,     3,
       1,     1,     0,     1,     1,     2,     2,     6,     6,     6,
       6,     2,     2,     1,     0,     3,     3,     3,     0,     3,
       3,     3,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     1,     3,     4,     3,     3,     3,     3,     2,
       3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      35,     0,     0,     0,     0,     0,     3,     2,     5,     9,
      70,    36,    37,    38,     0,     0,     0,     0,     1,     6,
       0,    11,     0,     0,    85,    39,    40,     8,   211,   212,
       0,     0,     0,    41,    52,    42,    43,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     4,    86,    87,
      88,    92,    93,    96,    97,     0,     0,   219,     0,     0,
       0,     0,     0,     0,    10,     0,     0,     0,    17,    14,
      71,    72,     0,     0,     0,     0,     0,    89,    90,    94,
      95,    98,    99,   213,     0,   221,   216,   215,   217,   218,
     220,    12,     0,     0,    21,     0,    13,    17,    15,     0,
       0,    91,   100,   101,   102,   103,   214,    16,     0,     0,
       0,    23,     0,    18,     0,     0,     0,     0,     0,     0,
      35,     0,     0,    19,    73,    74,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,    48,    44,    45,    46,
      47,    32,     0,     0,    30,     0,     0,     0,     0,    53,
      56,    58,    57,    60,    59,    61,    33,    34,     0,     0,
       0,    24,     0,     0,    20,    64,    62,    63,     0,   144,
     146,   173,   176,     0,     0,     0,    25,    26,    27,    28,
      29,     0,    54,    55,     0,   145,     0,   194,   172,     0,
     175,     0,    49,     0,     0,     0,    31,   192,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   148,   149,   150,
     151,   163,   164,     0,   191,   193,     0,   198,     0,   180,
     181,   182,    50,     0,     0,     7,    65,    70,     0,     0,
       0,     0,     0,     0,     0,     0,   152,     0,     0,     0,
     174,     0,     0,     0,     0,     0,   177,   183,   184,    51,
      69,    66,     0,     0,   202,   205,     0,   208,     0,     0,
       0,     0,   159,   162,   153,   197,   196,   195,   200,   201,
       0,   178,   179,     0,     0,     0,     0,   185,   186,    68,
       0,     0,     0,     0,     0,   118,    67,    75,    81,    85,
      76,    77,    78,    79,    80,     0,   203,   204,   206,   207,
       0,   209,   210,     0,     0,     0,     0,   160,   161,   154,
     199,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,   104,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,     0,   117,     0,   108,   167,
     168,   165,   166,   155,   156,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   187,   188,   189,   190,
       0,     0,     0,   114,   115,   116,   106,   105,   107,     0,
       0,     0,   133,     0,     0,   109,   110,     0,   169,   135,
     134,     0,     0,   112,   113,     0,     0,   119,     0,     0,
       0,     0,     0,   121,     0,     0,     0,     0,     0,     0,
     125,   157,   158,     0,     0,   120,     0,     0,   128,     0,
       0,   123,   124,   122,     0,     0,   130,   170,   171,   126,
     127,     0,     0,     0,   129,     0,     0,   131,   132,     0,
       0,     0,     0,     0,     0,    83,   137,   138,     0,     0,
     136,     0,     0,     0,   140,   141,   142,   143,   139
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     5,     6,     7,     8,     9,    21,    39,    67,    68,
      69,    96,    97,   110,   111,   122,   144,   164,   119,   120,
      10,    11,    12,    13,   136,   160,   175,   195,    33,    94,
     149,   129,   130,   131,   132,   225,   226,   227,    24,   286,
     287,   288,   289,    47,    48,    49,    50,    51,    52,    53,
      54,   338,   355,   371,   290,   291,   292,   293,   294,   393,
     400,   408,   416,   423,   350,   435,   436,   437,   438,   439,
     137,   138,   185,   206,   207,   363,   208,   209,   210,   211,
     212,   387,   139,   188,   140,   190,   221,   246,   247,   248,
     186,   187,   214,   215,   240,   217,   253,   254,   256,    34
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -331
static const short int yypact[] =
{
      48,  -118,  -113,   -57,   -51,   107,  -331,  -331,    53,    69,
     -30,  -331,    87,    95,   136,    15,    15,    15,  -331,  -331,
      14,   133,    34,    49,   142,  -331,  -331,  -331,  -331,   175,
      18,    15,    15,  -331,   206,  -331,  -331,   182,    67,   176,
      15,    15,    71,    84,    94,   166,   173,  -331,  -331,   -29,
     127,   154,   198,   163,   217,    15,    15,   290,    25,    15,
      15,    15,    15,    15,  -331,   298,   181,   263,   276,   176,
     262,   264,    15,    15,    15,    15,    15,  -331,  -331,  -331,
    -331,  -331,  -331,   206,    31,  -331,   224,   224,   290,   290,
     290,  -331,   304,   188,   278,   189,  -331,   276,  -331,   195,
     196,  -331,  -331,  -331,  -331,  -331,  -331,  -331,   312,   197,
     286,   289,   318,  -331,    15,    15,    98,    15,   202,   285,
      80,   203,   296,  -331,  -331,  -331,   208,   209,   210,   273,
      -2,   -12,   185,  -331,   140,   211,   268,  -331,  -331,  -331,
    -331,  -331,    15,   212,   302,    15,    15,    15,   215,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,   226,   216,
     297,  -331,   117,   220,  -331,   206,   206,   206,   214,  -331,
     287,   287,   287,    15,   221,   305,  -331,  -331,  -331,  -331,
    -331,    15,  -331,  -331,   222,  -331,    63,   299,  -331,   267,
    -331,    97,  -331,    15,   227,   306,   206,  -331,   228,   229,
     230,   231,   232,   233,   234,   237,  -331,   259,  -331,  -331,
    -331,  -331,  -331,   241,  -331,  -331,   242,   288,   243,  -331,
    -331,   187,  -331,    15,   245,  -331,   306,   -30,    15,    15,
      15,    15,    15,    15,    15,    15,   260,   125,   138,   246,
    -331,   194,   248,   249,   250,   252,  -331,   201,   205,  -331,
      15,  -331,    83,   303,   144,    55,   307,    42,    82,   100,
     279,   281,    68,   206,   120,  -331,  -331,  -331,  -331,  -331,
      15,  -331,  -331,    15,    15,    15,    15,  -331,  -331,  -331,
     261,   265,   269,   270,   271,  -331,  -331,  -331,   325,   142,
    -331,  -331,  -331,  -331,  -331,   272,  -331,  -331,  -331,  -331,
     274,  -331,  -331,   275,   277,   280,   282,  -331,  -331,  -331,
    -331,   319,   322,   320,   326,   382,   172,   172,   172,    97,
    -331,   310,    15,    15,    15,    15,    15,    15,   283,   284,
     291,   292,  -331,   294,   294,   294,  -331,   293,   300,  -331,
    -331,   206,   206,  -331,  -331,    15,    15,    15,    15,   295,
     171,   171,   171,   132,   301,   311,  -331,  -331,  -331,  -331,
      15,   308,   309,  -331,  -331,  -331,  -331,  -331,  -331,   158,
     313,   120,    61,    15,    15,  -331,  -331,   160,   207,  -331,
    -331,   315,   317,  -331,  -331,   314,   316,   328,   321,   323,
      15,    15,   324,   329,    15,    15,   114,   121,   396,   327,
     330,  -331,  -331,   331,   332,  -331,     9,   333,   335,    15,
      15,  -331,  -331,   206,   165,   334,   336,   206,   206,  -331,
    -331,    15,   337,   172,   206,   167,   259,  -331,  -331,   171,
     294,   147,   338,   339,   340,  -331,  -331,  -331,   341,   344,
    -331,    15,    15,    15,  -331,  -331,   206,   206,   206
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -331,  -331,  -331,   395,  -331,  -331,  -331,  -331,  -331,   345,
    -331,   342,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
     346,  -331,   392,   397,  -331,  -331,  -331,  -331,   -14,  -331,
    -331,  -331,   157,   -36,   168,   200,  -331,  -331,   186,  -331,
     135,  -331,  -331,   141,  -331,   377,   380,   379,   384,   387,
     389,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
    -331,  -331,  -331,  -331,  -326,  -331,  -331,  -331,     4,     6,
    -331,  -331,  -331,  -331,  -234,  -330,  -203,  -226,  -259,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,   130,  -331,   219,   199,
     129,  -331,  -331,  -331,  -331,  -331,  -225,  -331,  -187,   -31
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short int yytable[] =
{
      57,    58,    35,    36,   236,   309,    14,   260,   351,   352,
     264,    15,    28,    29,    30,    22,    23,    31,    28,    29,
      30,   364,   365,    31,    83,    84,    70,    71,    86,    87,
      88,    89,    90,    59,    60,    61,    62,   126,    63,    59,
      60,    61,    62,   127,    63,   310,   261,   126,   311,   312,
      59,    60,    61,    62,   128,    63,    45,    46,   101,   102,
     103,   104,   105,    59,    60,    61,    62,    16,    63,    59,
      60,    61,    62,    17,    63,     1,    59,    60,    61,    62,
       1,    63,   333,   334,   335,     2,     3,     4,   313,   314,
      59,    60,    61,    62,   150,    63,   155,    20,   340,   430,
     124,   125,   344,   133,   431,   411,   412,    18,    59,    60,
      61,    62,   378,    63,   165,   166,   167,     2,     3,     4,
     356,   357,    59,    60,    61,    62,     4,    63,   161,    59,
      60,    61,    62,     3,    63,   339,   198,   199,    37,   343,
      32,   280,    27,   281,   282,   283,    32,   126,   381,    56,
     196,   284,   285,   127,   128,   200,   201,    85,    40,   192,
     358,   359,    38,   106,   202,   203,   204,   218,   219,   220,
     402,   301,   302,    41,   205,   303,   265,   266,   267,   222,
     298,   299,    55,   379,   380,   307,   308,   382,    64,   426,
     403,    65,   304,   198,   199,    72,   404,   255,   257,   258,
     259,   255,   257,   262,   263,   440,    66,   401,    73,   249,
      43,    44,   200,   201,    59,    60,    61,    62,    74,    63,
     366,   367,   368,   429,    42,    43,    44,    45,    46,   176,
     177,   178,   179,   180,    61,    62,   279,    63,    44,   255,
     127,   128,   255,   255,   257,   257,   169,   170,   171,    46,
     172,   242,   243,   244,   245,   432,   433,   434,   205,   156,
     157,   182,   183,   268,   269,   271,   272,   244,   245,   242,
     243,   296,   297,   202,   203,   361,   362,   375,   376,   383,
     384,    43,   385,   386,   419,   420,   427,   428,   152,   154,
      75,   257,   255,   341,   342,   257,   255,    76,   151,   153,
     189,   191,    45,    63,    91,    92,    93,    95,    99,   100,
     107,   109,   108,   112,   255,   255,   257,   257,   116,   114,
     115,   117,   118,   121,   123,   135,   134,   142,   143,   372,
     148,   159,   145,   146,   147,   158,   162,   163,   174,   168,
     173,   184,   255,   257,   181,   193,   197,   194,   216,   213,
     224,   223,   228,   229,   230,   231,   232,   233,   234,   396,
     397,   235,   204,   257,   255,   237,   238,   241,   239,   250,
     270,   205,   273,   274,   275,   413,   276,   295,   417,   418,
     300,   305,   306,   280,   328,   315,   329,   330,   332,   316,
     424,   354,   331,   317,   318,   319,   322,   337,   323,   324,
     349,   325,   405,    19,   326,    26,   327,   345,   346,    25,
     446,   447,   448,   252,    98,   347,   348,   353,   370,   360,
     388,   389,   392,   320,   399,   369,   251,    78,   407,    77,
     321,    80,   373,   374,   415,    79,   422,   377,   390,   113,
     391,    82,    81,   445,   444,   394,   277,   395,   398,   336,
     433,   406,   432,     0,     0,   409,   410,   414,   421,     0,
       0,   425,   441,   442,   443,     0,   141,   278
};

static const short int yycheck[] =
{
      31,    32,    16,    17,   207,   264,   124,   232,   334,   335,
     236,   124,     3,     4,     5,    45,    46,     8,     3,     4,
       5,   351,   352,     8,    55,    56,    40,    41,    59,    60,
      61,    62,    63,     8,     9,    10,    11,    49,    13,     8,
       9,    10,    11,    55,    13,   270,   233,    49,   273,   274,
       8,     9,    10,    11,    56,    13,    85,    86,    72,    73,
      74,    75,    76,     8,     9,    10,    11,   124,    13,     8,
       9,    10,    11,   124,    13,    27,     8,     9,    10,    11,
      27,    13,   316,   317,   318,    37,    38,    39,   275,   276,
       8,     9,    10,    11,   130,    13,   132,    28,   323,   429,
     114,   115,   327,   117,   430,    96,    97,     0,     8,     9,
      10,    11,   371,    13,   145,   146,   147,    37,    38,    39,
     345,   346,     8,     9,    10,    11,    39,    13,   142,     8,
       9,    10,    11,    38,    13,   322,    73,    74,   124,   326,
     131,    58,     6,    60,    61,    62,   131,    49,   373,   131,
     181,    68,    69,    55,    56,    92,    93,   132,   124,   173,
     347,   348,    29,   132,   101,   102,   103,    70,    71,    72,
     395,   129,   130,   124,   111,    93,    51,    52,    53,   193,
     125,   126,     7,   122,   123,   117,   118,   374,     6,   423,
      76,   124,    92,    73,    74,   124,    75,   228,   229,   230,
     231,   232,   233,   234,   235,   431,    30,   394,   124,   223,
      83,    84,    92,    93,     8,     9,    10,    11,   124,    13,
      88,    89,    90,   426,    82,    83,    84,    85,    86,   112,
     113,   114,   115,   116,    10,    11,   250,    13,    84,   270,
      55,    56,   273,   274,   275,   276,    20,    21,    22,    86,
      24,    64,    65,    66,    67,   108,   109,   110,   111,   119,
     120,    47,    48,   125,   126,    71,    72,    66,    67,    64,
      65,   127,   128,   101,   102,   104,   105,   119,   120,   119,
     120,    83,    75,    76,   119,   120,   119,   120,   131,   132,
     124,   322,   323,   324,   325,   326,   327,   124,   130,   131,
     171,   172,    85,    13,     6,   124,    43,    31,    46,    45,
       6,    33,   124,   124,   345,   346,   347,   348,     6,   124,
     124,   124,    36,    34,     6,    40,   124,   124,    32,   360,
      57,    63,   124,   124,   124,   124,   124,    35,    41,   124,
     124,    54,   373,   374,   124,   124,   124,    42,    81,    50,
      44,   124,   124,   124,   124,   124,   124,   124,   124,   390,
     391,   124,   103,   394,   395,   124,   124,   124,    80,   124,
     124,   111,   124,   124,   124,   406,   124,    74,   409,   410,
      73,   102,   101,    58,    65,   124,    64,    67,     6,   124,
     421,    91,    66,   124,   124,   124,   124,    87,   124,   124,
     106,   124,     6,     8,   124,    13,   124,   124,   124,    12,
     441,   442,   443,   227,    69,   124,   124,   124,   107,   124,
     105,   104,    94,   288,    95,   124,   226,    50,    98,    49,
     289,    52,   124,   124,    99,    51,   100,   124,   124,    97,
     124,    54,    53,   439,   438,   124,   247,   124,   124,   319,
     109,   124,   108,    -1,    -1,   124,   124,   124,   124,    -1,
      -1,   124,   124,   124,   124,    -1,   120,   248
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    27,    37,    38,    39,   134,   135,   136,   137,   138,
     153,   154,   155,   156,   124,   124,   124,   124,     0,   136,
      28,   139,    45,    46,   171,   156,   155,     6,     3,     4,
       5,     8,   131,   161,   232,   161,   161,   124,    29,   140,
     124,   124,    82,    83,    84,    85,    86,   176,   177,   178,
     179,   180,   181,   182,   183,     7,   131,   232,   232,     8,
       9,    10,    11,    13,     6,   124,    30,   141,   142,   143,
     161,   161,   124,   124,   124,   124,   124,   179,   178,   181,
     180,   183,   182,   232,   232,   132,   232,   232,   232,   232,
     232,     6,   124,    43,   162,    31,   144,   145,   142,    46,
      45,   161,   161,   161,   161,   161,   132,     6,   124,    33,
     146,   147,   124,   144,   124,   124,     6,   124,    36,   151,
     152,    34,   148,     6,   161,   161,    49,    55,    56,   164,
     165,   166,   167,   161,   124,    40,   157,   203,   204,   215,
     217,   153,   124,    32,   149,   124,   124,   124,    57,   163,
     166,   167,   165,   167,   165,   166,   119,   120,   124,    63,
     158,   161,   124,    35,   150,   232,   232,   232,   124,    20,
      21,    22,    24,   124,    41,   159,   112,   113,   114,   115,
     116,   124,    47,    48,    54,   205,   223,   224,   216,   223,
     218,   223,   161,   124,    42,   160,   232,   124,    73,    74,
      92,    93,   101,   102,   103,   111,   206,   207,   209,   210,
     211,   212,   213,    50,   225,   226,    81,   228,    70,    71,
      72,   219,   161,   124,    44,   168,   169,   170,   124,   124,
     124,   124,   124,   124,   124,   124,   209,   124,   124,    80,
     227,   124,    64,    65,    66,    67,   220,   221,   222,   161,
     124,   168,   171,   229,   230,   232,   231,   232,   232,   232,
     229,   231,   232,   232,   210,    51,    52,    53,   125,   126,
     124,    71,    72,   124,   124,   124,   124,   222,   221,   161,
      58,    60,    61,    62,    68,    69,   172,   173,   174,   175,
     187,   188,   189,   190,   191,    74,   127,   128,   125,   126,
      73,   129,   130,    93,    92,   102,   101,   117,   118,   211,
     229,   229,   229,   231,   231,   124,   124,   124,   124,   124,
     173,   176,   124,   124,   124,   124,   124,   124,    65,    64,
      67,    66,     6,   207,   207,   207,   219,    87,   184,   231,
     229,   232,   232,   231,   229,   124,   124,   124,   124,   106,
     197,   197,   197,   124,    91,   185,   229,   229,   231,   231,
     124,   104,   105,   208,   208,   208,    88,    89,    90,   124,
     107,   186,   232,   124,   124,   119,   120,   124,   211,   122,
     123,   229,   231,   119,   120,    75,    76,   214,   105,   104,
     124,   124,    94,   192,   124,   124,   232,   232,   124,    95,
     193,   231,   229,    76,    75,     6,   124,    98,   194,   124,
     124,    96,    97,   232,   124,    99,   195,   232,   232,   119,
     120,   124,   100,   196,   232,   124,   207,   119,   120,   209,
     208,   197,   108,   109,   110,   198,   199,   200,   201,   202,
     210,   124,   124,   124,   202,   201,   232,   232,   232
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror (&yylloc, pp, "syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, pp)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Type, Value, Location);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  fprintf (yyoutput, ": ");

# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (MaRC::ParseParameter & pp);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (MaRC::ParseParameter & pp)
#else
int
yyparse (pp)
    MaRC::ParseParameter & pp;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended. */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif


  yyvsp[0] = yylval;
    yylsp[0] = yylloc;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
  *++yylsp = yylloc;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, yylsp - yylen, yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 250 "parse.yy"
    {
          // Reset defaults to those chosen by the user.  If none were
          // chosen by the user, the values will remain unchanged.

          pp.lat_interval  = lat_interval;
          pp.lon_interval  = lon_interval;

          if (!isnan (minimum))
              pp.minimum = minimum;

          if (!isnan (maximum))
              pp.maximum = maximum;

          pp.nibble_left   = nibble_left_val;
          pp.nibble_right  = nibble_right_val;
          pp.nibble_top    = nibble_top_val;
          pp.nibble_bottom = nibble_bottom_val;
        }
    break;

  case 7:
#line 287 "parse.yy"
    {
          // We only perform this check if the number of planes was
          // actually set since it is no longer necessary to set the
          // number of planes.  This also means that we no longer need
          // this check.  It is only kept around to maintain
          // consistent error checking with earlier versions of MaRC.
          // Once support for the "PLANES" keyword is removed, this
          // check can be removed.
          if (num_planes > 0 && planes_queued != num_planes)
            {
              std::cerr << "ERROR: Number of planes in map entry does not"
                        << std::endl
                        << "       match the number of planes stated by"
                        << std::endl
                        << "       the \"PLANES\" keyword." << std::endl
                        << "           Expected planes: " << num_planes
                        << std::endl
                        << "           Actual planes:   " << planes_queued
                        << std::endl;
            }
          else
            {
              MaRC::ValuePtr<MaRC::MapCommand> map_command;

              switch (map_data_type)
                {
                case BYTE:
                  map_command =
                    MaRC::ValuePtr<MaRC::MapCommand_T<unsigned char> > (
                      new MaRC::MapCommand_T<unsigned char> (map_filename,
                                                             body_name,
                                                             map_factory_byte,
                                                             map_samples,
                                                             map_lines));
                  break;

                case SHORT:
                  map_command =
                    MaRC::ValuePtr<MaRC::MapCommand_T<short> > (
                      new MaRC::MapCommand_T<short> (map_filename,
                                                     body_name,
                                                     map_factory_short,
                                                     map_samples,
                                                     map_lines));
                  break;

                case LONG:
                  map_command =
                    MaRC::ValuePtr<MaRC::MapCommand_T<MaRC::Long> > (
                      new MaRC::MapCommand_T<MaRC::Long> (map_filename,
                                                          body_name,
                                                          map_factory_long,
                                                          map_samples,
                                                          map_lines));
                  break;

                case FLOAT:
                  map_command =
                    MaRC::ValuePtr<MaRC::MapCommand_T<float> > (
                      new MaRC::MapCommand_T<float> (map_filename,
                                                     body_name,
                                                     map_factory_float,
                                                     map_samples,
                                                     map_lines));
                  break;

                case DOUBLE:
                  map_command =
                    MaRC::ValuePtr<MaRC::MapCommand_T<double> > (
                      new MaRC::MapCommand_T<double> (map_filename,
                                                      body_name,
                                                      map_factory_double,
                                                      map_samples,
                                                      map_lines));
                  break;

                default:
                  throw std::invalid_argument ("Unrecognized map data type");
                  break;
                }

              map_command->author (map_author);
              map_command->origin (map_origin);

              map_command->comment_list (comment_list);
              map_command->xcomment_list (xcomment_list);

              if (create_grid)
                map_command->grid_intervals (lat_interval, lon_interval);

              if (transform_data)
                {
                  map_command->data_zero (fits_bzero);
                  map_command->data_scale (fits_bscale);
                }

              if (blank_set)
                map_command->data_blank (fits_blank);

              map_command->image_factories (image_factories);

              pp.push_command (map_command);
            }
        }
    break;

  case 8:
#line 394 "parse.yy"
    {
          map_filename = (yyvsp[0].sval);
          free ((yyvsp[0].sval));

          // Reset items that may have been set for the previous map.
          map_author.clear ();
          map_origin.clear ();

          comment_list.clear ();
          xcomment_list.clear ();

          create_grid = false;
          transform_data = false;
          blank_set = false;

          image_factories.clear ();

          // @todo Remove once deprecate plane number support is
          //       removed.
          num_planes = 0; expected_plane = 1; planes_queued = 0;
        }
    break;

  case 10:
#line 418 "parse.yy"
    { map_author = (yyvsp[0].sval); free ((yyvsp[0].sval)); }
    break;

  case 12:
#line 422 "parse.yy"
    { map_origin = (yyvsp[0].sval); free ((yyvsp[0].sval)); }
    break;

  case 16:
#line 435 "parse.yy"
    { comment_list.push_back ((yyvsp[0].sval)); free ((yyvsp[0].sval)); }
    break;

  case 19:
#line 443 "parse.yy"
    { xcomment_list.push_back ((yyvsp[0].sval)); free ((yyvsp[0].sval)); }
    break;

  case 22:
#line 455 "parse.yy"
    {
            fits_bzero = static_cast<float> ((yyvsp[0].val));
            transform_data = true;
        }
    break;

  case 24:
#line 462 "parse.yy"
    {
            fits_bscale = static_cast<float> ((yyvsp[0].val));
            transform_data = true;
        }
    break;

  case 25:
#line 469 "parse.yy"
    { map_data_type = BYTE;   }
    break;

  case 26:
#line 470 "parse.yy"
    { map_data_type = SHORT;  }
    break;

  case 27:
#line 471 "parse.yy"
    { map_data_type = LONG;   }
    break;

  case 28:
#line 472 "parse.yy"
    { map_data_type = FLOAT;  }
    break;

  case 29:
#line 473 "parse.yy"
    { map_data_type = DOUBLE; }
    break;

  case 31:
#line 477 "parse.yy"
    {
          if (map_data_type == FLOAT || map_data_type == DOUBLE)
            throw std::invalid_argument ("\"BLANK\" keyword not valid with "
                                         "floating point types.");
          else
            {
              fits_blank = static_cast<int> ((yyvsp[0].val));
              blank_set = true;
            }
          ;
        }
    break;

  case 33:
#line 496 "parse.yy"
    {
          create_grid = true;
          lat_interval = pp.lat_interval;
          lon_interval = pp.lon_interval;
        }
    break;

  case 34:
#line 501 "parse.yy"
    { create_grid = false; }
    break;

  case 41:
#line 513 "parse.yy"
    {
          if ((yyvsp[0].val) <= 0)
            {
              std::ostringstream s;
              s << "Grid interval value (" << (yyvsp[0].val) << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            {
              lat_interval = (yyvsp[0].val);
              lon_interval = (yyvsp[0].val);
            }
        }
    break;

  case 42:
#line 531 "parse.yy"
    {
          if ((yyvsp[0].val) <= 0)
            {
              std::ostringstream s;
              s << "Latitude grid interval value (" << (yyvsp[0].val) << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            lat_interval = (yyvsp[0].val);
        }
    break;

  case 43:
#line 546 "parse.yy"
    {
          if ((yyvsp[0].val) <= 0)
            {
              std::ostringstream s;
              s << "Longitude grid interval value (" << (yyvsp[0].val) << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            lon_interval = (yyvsp[0].val);
        }
    break;

  case 49:
#line 578 "parse.yy"
    {
          /**
           * @deprecated The "PLANES" keyword is no longer necessary.
           *
           * @todo Remove support for the "PLANES" keyword.
           */

          // We really don't need to ask the user to set the number of
          // planes or the plane number any more.  The number of
          // planes is set dynamically as source images are queued.
          if ((yyvsp[0].val) > 0)
            num_planes = static_cast<unsigned int> ((yyvsp[0].val));
          else
            std::cerr << "Incorrect number of planes entered: "
                      << (yyvsp[0].val) << std::endl;
        }
    break;

  case 50:
#line 597 "parse.yy"
    {
          if ((yyvsp[0].val) > 0)
            map_samples = static_cast<unsigned int> ((yyvsp[0].val));
          else
            {
              std::cerr << "Incorrect value for SAMPLES entered: "
                        << (yyvsp[0].val) << std::endl;
            }
        }
    break;

  case 51:
#line 608 "parse.yy"
    {
          if ((yyvsp[0].val) > 0)
            map_lines = static_cast<unsigned int> ((yyvsp[0].val));
          else
            {
              std::cerr << "Incorrect value for LINES entered: " << (yyvsp[0].val)
                        << std::endl;
            }
        }
    break;

  case 52:
#line 619 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); }
    break;

  case 53:
#line 624 "parse.yy"
    {
          body_name = (yyvsp[-2].sval);
          free ((yyvsp[-2].sval));

          oblate_spheroid =
            MaRC::ValuePtr<MaRC::OblateSpheroid> (
              new MaRC::OblateSpheroid ((yyvsp[0].bval),
                                        ((yyvsp[-1].radii_data)).eq_rad,
                                        ((yyvsp[-1].radii_data)).pol_rad,
                                        ((yyvsp[-1].radii_data)).flattening));
        }
    break;

  case 54:
#line 638 "parse.yy"
    {
          (yyval.bval) = true;
        }
    break;

  case 55:
#line 641 "parse.yy"
    {
          (yyval.bval) = false;
        }
    break;

  case 56:
#line 647 "parse.yy"
    {
          ((yyval.radii_data)).eq_rad = (yyvsp[-1].val);
          ((yyval.radii_data)).pol_rad = (yyvsp[0].val);
          ((yyval.radii_data)).flattening = -1;
        }
    break;

  case 57:
#line 653 "parse.yy"
    {
          ((yyval.radii_data)).eq_rad = (yyvsp[0].val);
          ((yyval.radii_data)).pol_rad = (yyvsp[-1].val);
          ((yyval.radii_data)).flattening = -1;
        }
    break;

  case 58:
#line 659 "parse.yy"
    {
          ((yyval.radii_data)).eq_rad = (yyvsp[-1].val);
          ((yyval.radii_data)).pol_rad = -1;
          ((yyval.radii_data)).flattening = (yyvsp[0].val);
        }
    break;

  case 59:
#line 665 "parse.yy"
    {
          ((yyval.radii_data)).eq_rad = (yyvsp[0].val);
          ((yyval.radii_data)).pol_rad = -1;
          ((yyval.radii_data)).flattening = (yyvsp[-1].val);
        }
    break;

  case 60:
#line 671 "parse.yy"
    {
          ((yyval.radii_data)).eq_rad = -1;
          ((yyval.radii_data)).pol_rad = (yyvsp[-1].val);
          ((yyval.radii_data)).flattening = (yyvsp[0].val);
        }
    break;

  case 61:
#line 677 "parse.yy"
    {
          ((yyval.radii_data)).eq_rad = -1;
          ((yyval.radii_data)).pol_rad = (yyvsp[0].val);
          ((yyval.radii_data)).flattening = (yyvsp[-1].val);
        }
    break;

  case 62:
#line 685 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); }
    break;

  case 63:
#line 689 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); }
    break;

  case 64:
#line 693 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); }
    break;

  case 67:
#line 703 "parse.yy"
    {
          image_factory->minimum (minimum);
          image_factory->maximum (maximum);

          image_factories.push_back (image_factory);

          photo_factories.clear ();
        }
    break;

  case 68:
#line 714 "parse.yy"
    {
          /**
           * @deprecated It is no longer necessary to specify
           *             the plane number.
           *
           * @todo Remove support for specifying the plane
           *       number.
           */
          if (num_planes == 0)
            {
              std::cerr << "Number of planes not entered prior to plane "
                        << "definition." << std::endl;
            }

          const unsigned int map_plane = static_cast<unsigned int> ((yyvsp[0].val));

          if (map_plane == expected_plane)
            {
              if (map_plane <= num_planes)
                {
                  plane_num = map_plane;
                  ++expected_plane;
                  ++planes_queued;
                }
              else
                {
                  std::cerr
                    << std::endl
                    << "ERROR:" << std::endl
                    << "Incorrect plane number entered.  Plane" << std::endl
                    << "number should be greater than zero" << std::endl
                    << "and less than or equal to the number" << std::endl
                    << "of planes (" << num_planes << ")."
                    << std::endl
                    << "You entered:  " << (yyvsp[0].val) << std::endl;
                }
            }
          else
            {
              std::cerr << "Incorrect plane number entered." << std::endl
                        << "Expected plane number is: " << expected_plane
                        << std::endl
                        << "You entered:  " << (yyvsp[0].val) << std::endl;
            }

          minimum = pp.minimum; maximum = pp.maximum;
        }
    break;

  case 69:
#line 761 "parse.yy"
    { minimum = pp.minimum; maximum = pp.maximum; }
    break;

  case 71:
#line 765 "parse.yy"
    { minimum = (yyvsp[0].val); }
    break;

  case 72:
#line 766 "parse.yy"
    { maximum = (yyvsp[0].val); }
    break;

  case 73:
#line 768 "parse.yy"
    {
            if ((yyvsp[-3].val) < (yyvsp[0].val))
              {
                minimum = (yyvsp[-3].val);
                maximum = (yyvsp[0].val);
              }
            else
              {
                std::cerr << "Minimum data value: " << (yyvsp[-3].val)
                          << " is greater than"
                          << std::endl
                          << "maximum data value: " << (yyvsp[0].val) << std::endl;
              }
        }
    break;

  case 74:
#line 783 "parse.yy"
    {
            if ((yyvsp[0].val) < (yyvsp[-3].val))
              {
                minimum = (yyvsp[0].val);
                maximum = (yyvsp[-3].val);
              }
            else
              {
                std::cerr << "Minimum data value: " << (yyvsp[0].val)
                          << " is greater than"
                          << std::endl
                          << "maximum data value: " << (yyvsp[-3].val)
                          << std::endl;
              }
        }
    break;

  case 81:
#line 811 "parse.yy"
    {
          image_factory = photo_factory;
        }
    break;

  case 82:
#line 814 "parse.yy"
    {
            image_factory =
              MaRC::ValuePtr<MaRC::MosaicImageFactory> (
                new MaRC::MosaicImageFactory (photo_factories,
                                              averaging_type));
        }
    break;

  case 83:
#line 839 "parse.yy"
    {
          photo_factory->nibbling (nibble_left_val,
                                   nibble_right_val,
                                   nibble_top_val,
                                   nibble_bottom_val);

          nibble_left_val   = pp.nibble_left;
          nibble_right_val  = pp.nibble_right;
          nibble_top_val    = pp.nibble_top;
          nibble_bottom_val = pp.nibble_bottom;

          if (km_per_pixel_val > 0)
            {
              photo_factory->km_per_pixel (km_per_pixel_val);
              km_per_pixel_val = -1;  // Reset to "bad" value.
            }

          if (arcsec_per_pix_val > 0)
            {
              photo_factory->arcsec_per_pixel (arcsec_per_pix_val);
              arcsec_per_pix_val = -1;  // Reset to "bad" value.
            }

          if (focal_length_val > 0)
            {
              photo_factory->focal_length (focal_length_val);
              focal_length_val = -1;  // Reset to "bad" value.
            }

          if (pixel_scale_val > 0)
            {
              photo_factory->scale (pixel_scale_val);
              pixel_scale_val = -1;  // Reset to "bad" value.
            }

          if (!isnan (sample_center) && !isnan (line_center))
            {
              photo_factory->body_center (sample_center, line_center);
              sample_center = MaRC::NaN;  // Reset to "bad" value.
              line_center = MaRC::NaN;  // Reset to "bad" value.
            }

          if (!isnan (lat_at_center) && !isnan (lon_at_center))
            {
              photo_factory->lat_lon_center (lat_at_center, lon_at_center);
              lat_at_center = MaRC::NaN;  // Reset to "bad" value.
              lon_at_center = MaRC::NaN;  // Reset to "bad" value.
            }

          photo_factory->sub_observ (((yyvsp[-4].sub_observ_data)).lat, ((yyvsp[-4].sub_observ_data)).lon);
          photo_factory->position_angle ((yyvsp[-3].val));
          photo_factory->sub_solar (((yyvsp[-2].sub_solar_data)).lat, ((yyvsp[-2].sub_solar_data)).lon);
          photo_factory->range ((yyvsp[-1].val));

          photo_factories.push_back (*photo_factory);
        }
    break;

  case 84:
#line 899 "parse.yy"
    {
          photo_factory =
            MaRC::ValuePtr<MaRC::PhotoImageFactory> (
              new MaRC::PhotoImageFactory ((yyvsp[0].sval),
                                           *oblate_spheroid));
            free ((yyvsp[0].sval));
        }
    break;

  case 91:
#line 917 "parse.yy"
    {
          if ((yyvsp[0].val) >= 0)
            {
              nibble_left_val   = static_cast<unsigned int> ((yyvsp[0].val));
              nibble_right_val  = static_cast<unsigned int> ((yyvsp[0].val));
              nibble_top_val    = static_cast<unsigned int> ((yyvsp[0].val));
              nibble_bottom_val = static_cast<unsigned int> ((yyvsp[0].val));
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE entered: "
                        << (yyvsp[0].val) << std::endl;
            }
        }
    break;

  case 100:
#line 949 "parse.yy"
    {
          if ((yyvsp[0].val) >= 0)
            {
              nibble_left_val = static_cast<unsigned int> ((yyvsp[0].val));
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_LEFT entered: "
                        << (yyvsp[0].val) << std::endl;
            }
        }
    break;

  case 101:
#line 963 "parse.yy"
    {
          if ((yyvsp[0].val) >= 0)
            {
              nibble_right_val = static_cast<unsigned int> ((yyvsp[0].val));
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_RIGHT entered: "
                        << (yyvsp[0].val) << std::endl;
            }
        }
    break;

  case 102:
#line 977 "parse.yy"
    {
          if ((yyvsp[0].val) >= 0)
            {
              nibble_top_val = static_cast<unsigned int> ((yyvsp[0].val));
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_TOP entered: "
                        << (yyvsp[0].val) << std::endl;
            }
        }
    break;

  case 103:
#line 991 "parse.yy"
    {
          if ((yyvsp[0].val) >= 0)
            {
              nibble_bottom_val = static_cast<unsigned int> ((yyvsp[0].val));
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_BOTTOM entered: "
                        << (yyvsp[0].val) << std::endl;
            }
        }
    break;

  case 105:
#line 1005 "parse.yy"
    { photo_factory->invert (true,  false); }
    break;

  case 106:
#line 1006 "parse.yy"
    { photo_factory->invert (false, true);  }
    break;

  case 107:
#line 1007 "parse.yy"
    { photo_factory->invert (true,  true);  }
    break;

  case 109:
#line 1012 "parse.yy"
    { photo_factory->interpolate (true);  }
    break;

  case 110:
#line 1013 "parse.yy"
    { photo_factory->interpolate (false); }
    break;

  case 112:
#line 1017 "parse.yy"
    { photo_factory->remove_sky (true);  }
    break;

  case 113:
#line 1018 "parse.yy"
    { photo_factory->remove_sky (false); }
    break;

  case 114:
#line 1024 "parse.yy"
    {
            // Mu * 10000
            image_factory =
              MaRC::ValuePtr<MaRC::MuImageFactory> (
                new MaRC::MuImageFactory (*oblate_spheroid,
                                          ((yyvsp[-2].sub_observ_data)).lat,
                                          ((yyvsp[-2].sub_observ_data)).lon,
                                          (yyvsp[-1].val)));
        }
    break;

  case 115:
#line 1038 "parse.yy"
    {
          // Mu0 * 10000
          image_factory =
            MaRC::ValuePtr<MaRC::Mu0ImageFactory> (
              new MaRC::Mu0ImageFactory (*oblate_spheroid,
                                         ((yyvsp[0].sub_solar_data)).lat,
                                         ((yyvsp[0].sub_solar_data)).lon));
        }
    break;

  case 116:
#line 1051 "parse.yy"
    {
          // cos (phase angle) * 10000
          image_factory =
            MaRC::ValuePtr<MaRC::CosPhaseImageFactory> (
              new MaRC::CosPhaseImageFactory (*oblate_spheroid,
                                              ((yyvsp[-2].sub_observ_data)).lat,
                                              ((yyvsp[-2].sub_observ_data)).lon,
                                              ((yyvsp[0].sub_solar_data)).lat,
                                              ((yyvsp[0].sub_solar_data)).lon,
                                              (yyvsp[-1].val)));
        }
    break;

  case 117:
#line 1064 "parse.yy"
    {
            // Latitudes in radians
            MaRC::ValuePtr<MaRC::BodyData> bd (
              new MaRC::OblateSpheroid (*oblate_spheroid));

            image_factory =
              MaRC::ValuePtr<MaRC::LatitudeImageFactory> (
                new MaRC::LatitudeImageFactory (bd, graphic_lat));
           }
    break;

  case 118:
#line 1076 "parse.yy"
    {
            // Longitudes in radians
            image_factory =
              MaRC::ValuePtr<MaRC::LongitudeImageFactory> (
                new MaRC::LongitudeImageFactory);
           }
    break;

  case 120:
#line 1085 "parse.yy"
    { photo_factory->flat_field ((yyvsp[0].sval)); free ((yyvsp[0].sval));}
    break;

  case 122:
#line 1089 "parse.yy"
    { /* Image->setMinnaertExponent ($3); */ }
    break;

  case 123:
#line 1090 "parse.yy"
    {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_AUTO); */ }
    break;

  case 124:
#line 1092 "parse.yy"
    {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_TABLE); */ }
    break;

  case 126:
#line 1099 "parse.yy"
    {
            // Galileo spacecraft (GLL) geometric correction.
            photo_factory->geometric_correction (true);
          }
    break;

  case 127:
#line 1103 "parse.yy"
    {
            photo_factory->geometric_correction (false);
          }
    break;

  case 129:
#line 1109 "parse.yy"
    { photo_factory->emi_ang_limit ((yyvsp[0].val)); }
    break;

  case 131:
#line 1113 "parse.yy"
    { photo_factory->use_terminator (true);  }
    break;

  case 132:
#line 1114 "parse.yy"
    { photo_factory->use_terminator (false); }
    break;

  case 133:
#line 1117 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); }
    break;

  case 134:
#line 1118 "parse.yy"
    { (yyval.val) = (yyvsp[-1].val); }
    break;

  case 135:
#line 1119 "parse.yy"
    { (yyval.val) = (yyvsp[-1].val) * C::astronomical_unit / 1000; }
    break;

  case 139:
#line 1129 "parse.yy"
    { arcsec_per_pix_val = (yyvsp[0].val); }
    break;

  case 142:
#line 1140 "parse.yy"
    { focal_length_val = (yyvsp[0].val); }
    break;

  case 143:
#line 1144 "parse.yy"
    { pixel_scale_val = (yyvsp[0].val); }
    break;

  case 144:
#line 1270 "parse.yy"
    {
          switch (map_data_type)
            {
              case BYTE:
                map_factory_byte.reset (
                  new MaRC::Mercator<unsigned char> (oblate_spheroid));
                break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::Mercator<short> (oblate_spheroid));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::Mercator<MaRC::Long> (oblate_spheroid));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::Mercator<float> (oblate_spheroid));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::Mercator<double> (oblate_spheroid));
                break;

              default:
                throw std::invalid_argument ("Unrecognized map data type");
                break;
            }
        }
    break;

  case 145:
#line 1307 "parse.yy"
    {
          switch (map_data_type)
            {
              case BYTE:
                map_factory_byte.reset (
                  new MaRC::Orthographic<unsigned char> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::Orthographic<short> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::Orthographic<MaRC::Long> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::Orthographic<float> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::Orthographic<double> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              default:
                throw std::invalid_argument ("Unrecognized map data type");
                break;
            }

          // Reset options
          sub_observation_data.lat = 0;
          sub_observation_data.lon = 0;
          km_per_pixel_val = -1;
          position_angle_val = MaRC::NaN;
          ortho_center.geometry = MaRC::DEFAULT;
        }
    break;

  case 148:
#line 1391 "parse.yy"
    {
          sub_observation_data.lat = ((yyvsp[0].sub_observ_data)).lat;
          sub_observation_data.lon = ((yyvsp[0].sub_observ_data)).lon;
        }
    break;

  case 149:
#line 1395 "parse.yy"
    { position_angle_val = (yyvsp[0].val); }
    break;

  case 151:
#line 1397 "parse.yy"
    {
            if (!isnan (sample_center) && !isnan (line_center))
              {
                ortho_center.geometry = MaRC::CENTER_GIVEN;
                ortho_center.sample_lat_center = sample_center;
                ortho_center.line_lon_center   = line_center;
                sample_center = MaRC::NaN;  // Reset to "bad" value.
                line_center   = MaRC::NaN;  // Reset to "bad" value.
              }
            else if (!isnan (lat_at_center) && !isnan (lon_at_center))
              {
                ortho_center.geometry = MaRC::LAT_LON_GIVEN;
                ortho_center.sample_lat_center = lat_at_center;
                ortho_center.line_lon_center   = lon_at_center;
                lat_at_center = MaRC::NaN;  // Reset to "bad" value.
                lon_at_center = MaRC::NaN;  // Reset to "bad" value.
              }
        }
    break;

  case 152:
#line 1415 "parse.yy"
    {
          sub_observation_data.lat = ((yyvsp[-1].sub_observ_data)).lat;
          sub_observation_data.lon = ((yyvsp[-1].sub_observ_data)).lon;
            position_angle_val = (yyvsp[0].val);
        }
    break;

  case 153:
#line 1420 "parse.yy"
    {
          sub_observation_data.lat = ((yyvsp[-2].sub_observ_data)).lat;
          sub_observation_data.lon = ((yyvsp[-2].sub_observ_data)).lon;
            position_angle_val = (yyvsp[-1].val);
        }
    break;

  case 154:
#line 1425 "parse.yy"
    {
          sub_observation_data.lat = ((yyvsp[-3].sub_observ_data)).lat;
          sub_observation_data.lon = ((yyvsp[-3].sub_observ_data)).lon;
            position_angle_val = (yyvsp[-2].val);
            if (!isnan (sample_center) && !isnan (line_center))
              {
                ortho_center.geometry = MaRC::CENTER_GIVEN;
                ortho_center.sample_lat_center = sample_center;
                ortho_center.line_lon_center   = line_center;
                sample_center = MaRC::NaN;  // Reset to "bad" value.
                line_center   = MaRC::NaN;  // Reset to "bad" value.
              }
            else if (!isnan (lat_at_center) && !isnan (lon_at_center))
              {
                ortho_center.geometry = MaRC::LAT_LON_GIVEN;
                ortho_center.sample_lat_center = lat_at_center;
                ortho_center.line_lon_center   = lon_at_center;
                lat_at_center = MaRC::NaN;  // Reset to "bad" value.
                lon_at_center = MaRC::NaN;  // Reset to "bad" value.
              }
        }
    break;

  case 155:
#line 1452 "parse.yy"
    {
          (yyval.sub_observ_data).lat = (yyvsp[-3].val);
          (yyval.sub_observ_data).lon = (yyvsp[0].val);
        }
    break;

  case 156:
#line 1457 "parse.yy"
    {
            (yyval.sub_observ_data).lat = (yyvsp[0].val);
            (yyval.sub_observ_data).lon = (yyvsp[-3].val);
        }
    break;

  case 157:
#line 1465 "parse.yy"
    { ((yyval.sub_solar_data)).lat = (yyvsp[-3].val); ((yyval.sub_solar_data)).lon = (yyvsp[0].val); }
    break;

  case 158:
#line 1467 "parse.yy"
    { ((yyval.sub_solar_data)).lat = (yyvsp[0].val); ((yyval.sub_solar_data)).lon = (yyvsp[-3].val); }
    break;

  case 159:
#line 1471 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); }
    break;

  case 160:
#line 1472 "parse.yy"
    {
            if ((yyvsp[-1].val) >= 0)
              (yyval.val) = (yyvsp[-1].val);
            else
              {
                std::cerr << "Incorrect position (North) angle entered: "
                          << (yyvsp[-1].val) << " CW" << std::endl
                          << "Numeric value should be positive." << std::endl;
              }
        }
    break;

  case 161:
#line 1482 "parse.yy"
    {
            if ((yyvsp[-1].val) >= 0)
              (yyval.val) = (yyvsp[-1].val);
            else
              {
                std::cerr << "Incorrect position (North) angle entered: "
                          << (yyvsp[-1].val) << " CCW" << std::endl
                          << "Numeric value should be positive." << std::endl;
              }
        }
    break;

  case 162:
#line 1495 "parse.yy"
    { km_per_pixel_val = (yyvsp[0].val); }
    break;

  case 165:
#line 1505 "parse.yy"
    {
          sample_center = (yyvsp[-3].val);
          line_center   = (yyvsp[0].val);
        }
    break;

  case 166:
#line 1510 "parse.yy"
    {
            sample_center = (yyvsp[0].val);
            line_center   = (yyvsp[-3].val);
        }
    break;

  case 167:
#line 1518 "parse.yy"
    {
//           if (in_ortho_options)
//             MapEntry->setOrthoCenter ($3, $6, LAT_LON_GIVEN);
//           else if (in_perspective_options)
//             MapEntry->setPerspectiveCenter ($3, $6, LAT_LON_GIVEN);
//           else
//             Image->setLatLonCenter ($3, $6);

             lat_at_center = (yyvsp[-3].val);
             lon_at_center = (yyvsp[0].val);
        }
    break;

  case 168:
#line 1530 "parse.yy"
    {
//             if (in_ortho_options)
//               MapEntry->setOrthoCenter ($6, $3, LAT_LON_GIVEN);
//             else if (in_perspective_options)
//               MapEntry->setPerspectiveCenter ($6, $3, LAT_LON_GIVEN);
//             else
//               Image->setLatLonCenter ($6, $3);

             lat_at_center = (yyvsp[0].val);
             lon_at_center = (yyvsp[-3].val);
        }
    break;

  case 170:
#line 1545 "parse.yy"
    { photo_factory->optical_axis ((yyvsp[-3].val), (yyvsp[0].val)); }
    break;

  case 171:
#line 1547 "parse.yy"
    { photo_factory->optical_axis ((yyvsp[0].val), (yyvsp[-3].val)); }
    break;

  case 172:
#line 1584 "parse.yy"
    {
          switch (map_data_type)
            {
              case BYTE:
                map_factory_byte.reset (
                  new MaRC::PolarStereographic<unsigned char> (oblate_spheroid,
                                                               max_lat,
                                                               north_pole));
                break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::PolarStereographic<short> (oblate_spheroid,
                                                       max_lat,
                                                       north_pole));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::PolarStereographic<MaRC::Long> (oblate_spheroid,
                                                            max_lat,
                                                            north_pole));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::PolarStereographic<float> (oblate_spheroid,
                                                       max_lat,
                                                       north_pole));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::PolarStereographic<double> (oblate_spheroid,
                                                        max_lat,
                                                        north_pole));
                break;

              default:
                throw std::invalid_argument ("Unrecognized map data type");
                break;
            }

          // Reset options
          max_lat = MaRC::NaN;
          north_pole = true;
        }
    break;

  case 175:
#line 1643 "parse.yy"
    {
          switch (map_data_type)
            {
              case BYTE:
                map_factory_byte.reset (
                  new MaRC::SimpleCylindrical<unsigned char> (
                        oblate_spheroid,
                        lo_lat,
                        hi_lat,
                        lo_lon,
                        hi_lon,
                        graphic_lat));
                break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::SimpleCylindrical<short> (oblate_spheroid,
                                                      lo_lat,
                                                      hi_lat,
                                                      lo_lon,
                                                      hi_lon,
                                                      graphic_lat));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::SimpleCylindrical<MaRC::Long> (
                    oblate_spheroid,
                    lo_lat,
                    hi_lat,
                    lo_lon,
                    hi_lon,
                    graphic_lat));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::SimpleCylindrical<float> (oblate_spheroid,
                                                      lo_lat,
                                                      hi_lat,
                                                      lo_lon,
                                                      hi_lon,
                                                      graphic_lat));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::SimpleCylindrical<double> (oblate_spheroid,
                                                       lo_lat,
                                                       hi_lat,
                                                       lo_lon,
                                                       hi_lon,
                                                       graphic_lat));
                break;

              default:
                throw std::invalid_argument ("Unrecognized map data type");
                break;
            }

            // Reset options
            lo_lat = -90;
            hi_lat = 90;
            lo_lon = 0;
            hi_lon = 360;
            graphic_lat = false;
        }
    break;

  case 178:
#line 1720 "parse.yy"
    { graphic_lat = false; }
    break;

  case 179:
#line 1721 "parse.yy"
    { graphic_lat = true;  }
    break;

  case 180:
#line 1723 "parse.yy"
    { graphic_lat = false; }
    break;

  case 181:
#line 1724 "parse.yy"
    { graphic_lat = true;  }
    break;

  case 187:
#line 1736 "parse.yy"
    {
          if ((yyvsp[-3].val) < (yyvsp[0].val))
            {
              lo_lat = (yyvsp[-3].val);
              hi_lat = (yyvsp[0].val);
            }
          else
            std::cerr << "Error: LO_LAT is greater than HI_LAT" << std::endl;
        }
    break;

  case 188:
#line 1746 "parse.yy"
    {
          if ((yyvsp[-3].val) < (yyvsp[0].val))
            {
              lo_lat = (yyvsp[0].val);
              hi_lat = (yyvsp[-3].val);
            }
          else
            std::cerr << "Error: LO_LAT is greater than HI_LAT" << std::endl;
        }
    break;

  case 189:
#line 1759 "parse.yy"
    {
          lo_lon = (yyvsp[-3].val);
          hi_lon = (yyvsp[0].val);
        }
    break;

  case 190:
#line 1764 "parse.yy"
    {
          lo_lon = (yyvsp[0].val);
          hi_lon = (yyvsp[-3].val);
        }
    break;

  case 195:
#line 1801 "parse.yy"
    {
              averaging_type = MaRC::MosaicImage::AVG_UNWEIGHTED; }
    break;

  case 196:
#line 1803 "parse.yy"
    {
              averaging_type = MaRC::MosaicImage::AVG_WEIGHTED; }
    break;

  case 197:
#line 1805 "parse.yy"
    {
              averaging_type = MaRC::MosaicImage::AVG_NONE; }
    break;

  case 199:
#line 1861 "parse.yy"
    { max_lat = (yyvsp[0].val); }
    break;

  case 200:
#line 1864 "parse.yy"
    { north_pole = true; }
    break;

  case 201:
#line 1865 "parse.yy"
    { north_pole = false; }
    break;

  case 202:
#line 1869 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); }
    break;

  case 203:
#line 1870 "parse.yy"
    { (yyval.val) = (yyvsp[-1].val); }
    break;

  case 204:
#line 1871 "parse.yy"
    {
            /* Convert to CENTRIC latitude. */
            (yyval.val) = oblate_spheroid->centric_latitude ((yyvsp[-1].val) * C::degree)
                   / C::degree; }
    break;

  case 205:
#line 1878 "parse.yy"
    {
          if (::fabs ((yyvsp[0].val)) <= 90)
            (yyval.val) = (yyvsp[0].val);
          else {
            std::cerr << "Incorrect latitude entered: " << (yyvsp[0].val) << std::endl;
          }
        }
    break;

  case 206:
#line 1885 "parse.yy"
    {
          if ((yyvsp[-1].val) >= 0 && (yyvsp[-1].val) <= 90)
            (yyval.val) = (yyvsp[-1].val);
          else {
            std::cerr << "Incorrect latitude entered: " << (yyvsp[-1].val) << " N"
                      << std::endl;
          }
        }
    break;

  case 207:
#line 1893 "parse.yy"
    {
            if ((yyvsp[-1].val) >= 0 && (yyvsp[-1].val) <= 90)
              (yyval.val) = -(yyvsp[-1].val);
            else {
              std::cerr << "Incorrect latitude entered: " << (yyvsp[-1].val) << " S"
                        << std::endl;
            }
        }
    break;

  case 208:
#line 1904 "parse.yy"
    {
                if (::fabs ((yyvsp[0].val)) <= 360)
                  {
                    if ((yyvsp[0].val) < 0)
                      (yyvsp[0].val)+= 360;
                    (yyval.val) = (yyvsp[0].val);
                  }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << (yyvsp[0].val)
                              << std::endl;
                  }
          }
    break;

  case 209:
#line 1917 "parse.yy"
    {
                if (::fabs ((yyvsp[-1].val)) <= 360)
                  {
                    if ((yyvsp[-1].val) < 0)
                      (yyvsp[-1].val)+= 360;
                    if (oblate_spheroid->prograde ())
                      (yyval.val) = 360 - (yyvsp[-1].val);
                    else
                      (yyval.val) = (yyvsp[-1].val);
                }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << (yyvsp[-1].val)
                              << std::endl;
                  }
        }
    break;

  case 210:
#line 1933 "parse.yy"
    {
                if (::fabs ((yyvsp[-1].val)) <= 360)
                  {
                    if ((yyvsp[-1].val) < 0)
                      (yyvsp[-1].val)+= 360;
                    if (oblate_spheroid->prograde ())
                      (yyval.val) = (yyvsp[-1].val);
                    else
                      (yyval.val) = 360 - (yyvsp[-1].val);
                  }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << (yyvsp[-1].val)
                              << std::endl;
                  }
        }
    break;

  case 211:
#line 1953 "parse.yy"
    { (yyval.val) = (yyvsp[0].val);                         }
    break;

  case 212:
#line 1954 "parse.yy"
    { (yyval.val) = (yyvsp[0].tptr)->value.var;              }
    break;

  case 213:
#line 1955 "parse.yy"
    { (yyval.val) = (yyvsp[0].val); (yyvsp[-2].tptr)->value.var = (yyvsp[0].val);     }
    break;

  case 214:
#line 1956 "parse.yy"
    { (yyval.val) = (* ((yyvsp[-3].tptr)->value.fnctptr)) ((yyvsp[-1].val)); }
    break;

  case 215:
#line 1957 "parse.yy"
    { (yyval.val) = (yyvsp[-2].val) + (yyvsp[0].val);                    }
    break;

  case 216:
#line 1958 "parse.yy"
    { (yyval.val) = (yyvsp[-2].val) - (yyvsp[0].val);                    }
    break;

  case 217:
#line 1959 "parse.yy"
    { (yyval.val) = (yyvsp[-2].val) * (yyvsp[0].val);                    }
    break;

  case 218:
#line 1960 "parse.yy"
    { if ((yyvsp[0].val) != 0)
                                    (yyval.val) = (yyvsp[-2].val) / (yyvsp[0].val);
                                  else
                                    {
                                      (yyval.val) = (yyvsp[-2].val);
                                      fprintf (stderr,
                                               "%d.%d-%d.%d: division by zero",
                                               (yylsp[0]).first_line,
                                               (yylsp[0]).first_column,
                                               (yylsp[0]).last_line,
                                               (yylsp[0]).last_column);
                                    }
                                }
    break;

  case 219:
#line 1973 "parse.yy"
    { (yyval.val) = -(yyvsp[0].val);                        }
    break;

  case 220:
#line 1974 "parse.yy"
    { (yyval.val) = ::pow ((yyvsp[-2].val), (yyvsp[0].val));             }
    break;

  case 221:
#line 1975 "parse.yy"
    { (yyval.val) = (yyvsp[-1].val);                         }
    break;


    }

/* Line 1037 of yacc.c.  */
#line 3507 "parse.cc"

  yyvsp -= yylen;
  yyssp -= yylen;
  yylsp -= yylen;

  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (&yylloc, pp, yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror (&yylloc, pp, "syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (&yylloc, pp, "syntax error");
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
                 yyerror_range[0] = *yylsp;
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 yydestruct ("Error: popping",
                             yystos[*yyssp], yyvsp, yylsp);
	       }
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyerror_range[0] = yylsp[1-yylen];
  yylsp -= yylen;
  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping", yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though. */
  YYLLOC_DEFAULT (yyloc, yyerror_range - 1, 2);
  *++yylsp = yyloc;

  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yydestruct ("Error: discarding lookahead",
              yytoken, &yylval, &yylloc);
  yychar = YYEMPTY;
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror (&yylloc, pp, "parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1979 "parse.yy"


