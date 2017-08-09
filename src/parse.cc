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

  // Verify supersampling, or not?
  bool supersample_verify = false;

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
/* Line 191 of yacc.c.  */
#line 484 "parse.cc"
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


/* Line 214 of yacc.c.  */
#line 508 "parse.cc"

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
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
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
  short yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
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
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   494

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  101
/* YYNRULES -- Number of rules. */
#define YYNRULES  224
/* YYNRULES -- Number of states. */
#define YYNSTATES  455

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   373

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     132,   133,    10,     9,     2,     8,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   125,     2,
       2,     7,     2,     2,     2,     2,     2,   128,     2,   130,
       2,   129,     2,     2,     2,     2,     2,     2,   126,     2,
       2,     2,     2,   127,     2,     2,     2,   131,     2,     2,
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
     121,   122,   123,   124
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     7,    12,    14,    17,    30,    34,
      35,    39,    40,    44,    47,    48,    51,    55,    56,    59,
      63,    68,    69,    73,    74,    78,    82,    86,    90,    94,
      98,    99,   103,   106,   110,   114,   115,   117,   119,   121,
     124,   127,   131,   135,   139,   141,   143,   145,   147,   148,
     152,   156,   160,   162,   168,   172,   176,   179,   182,   185,
     188,   191,   194,   198,   202,   206,   208,   211,   215,   219,
     222,   223,   227,   231,   238,   245,   247,   249,   251,   253,
     255,   257,   259,   262,   281,   285,   286,   290,   294,   295,
     297,   299,   301,   304,   307,   311,   313,   315,   318,   321,
     323,   325,   328,   331,   335,   339,   343,   347,   348,   352,
     356,   360,   361,   365,   369,   370,   374,   378,   384,   390,
     396,   400,   402,   403,   407,   408,   412,   416,   420,   421,
     425,   429,   430,   434,   435,   439,   443,   447,   452,   457,
     459,   461,   463,   467,   470,   473,   477,   481,   485,   490,
     491,   494,   496,   498,   500,   502,   505,   509,   514,   521,
     528,   535,   542,   546,   551,   556,   560,   562,   564,   571,
     578,   585,   592,   593,   600,   607,   612,   613,   617,   622,
     623,   627,   631,   635,   637,   639,   640,   642,   644,   647,
     650,   657,   664,   671,   678,   681,   684,   686,   687,   691,
     695,   699,   700,   704,   708,   712,   714,   717,   720,   722,
     725,   728,   730,   733,   736,   738,   740,   744,   749,   753,
     757,   761,   765,   768,   772
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     135,     0,    -1,   137,    -1,   136,    -1,   154,   172,   177,
     178,    -1,   138,    -1,   138,   137,    -1,   139,   140,   141,
     142,   163,   147,   152,   158,   159,   160,   161,   169,    -1,
      27,   125,     6,    -1,    -1,    28,   125,     6,    -1,    -1,
      29,   125,     6,    -1,   143,   145,    -1,    -1,   144,   143,
      -1,    30,   125,     6,    -1,    -1,   146,   145,    -1,    31,
     125,     6,    -1,   148,   149,   150,   151,    -1,    -1,    33,
     125,   162,    -1,    -1,    34,   125,   162,    -1,    32,   125,
     113,    -1,    32,   125,   114,    -1,    32,   125,   115,    -1,
      32,   125,   116,    -1,    32,   125,   117,    -1,    -1,    35,
     125,   234,    -1,   153,   154,    -1,    36,   125,   120,    -1,
      36,   125,   121,    -1,    -1,   155,    -1,   156,    -1,   157,
      -1,   156,   157,    -1,   157,   156,    -1,    37,   125,   162,
      -1,    38,   125,   162,    -1,    39,   125,   162,    -1,   205,
      -1,   206,    -1,   217,    -1,   219,    -1,    -1,    63,   125,
     162,    -1,    41,   125,   162,    -1,    42,   125,   162,    -1,
     234,    -1,    43,   125,     6,   165,   164,    -1,    57,   125,
      47,    -1,    57,   125,    48,    -1,   166,   167,    -1,   167,
     166,    -1,   166,   168,    -1,   168,   166,    -1,   167,   168,
      -1,   168,   167,    -1,    55,   125,   234,    -1,    56,   125,
     234,    -1,    49,   125,   234,    -1,   170,    -1,   170,   169,
      -1,   171,   172,   173,    -1,    44,   125,   162,    -1,    44,
     125,    -1,    -1,    45,   125,   162,    -1,    46,   125,   162,
      -1,    45,   125,   162,    46,   125,   162,    -1,    46,   125,
     162,    45,   125,   162,    -1,   174,    -1,   189,    -1,   190,
      -1,   191,    -1,   192,    -1,   193,    -1,   175,    -1,   175,
     174,    -1,   176,   177,   178,   186,   187,   188,   213,   216,
     194,   195,   196,   197,   198,   209,   211,   210,   199,   200,
      -1,    58,   125,     6,    -1,    -1,   101,   125,   120,    -1,
     101,   125,   121,    -1,    -1,   179,    -1,   180,    -1,   181,
      -1,   180,   181,    -1,   181,   180,    -1,    82,   125,   162,
      -1,   182,    -1,   183,    -1,   182,   183,    -1,   183,   182,
      -1,   184,    -1,   185,    -1,   184,   185,    -1,   185,   184,
      -1,    83,   125,   162,    -1,    84,   125,   162,    -1,    85,
     125,   162,    -1,    86,   125,   162,    -1,    -1,    87,   125,
      89,    -1,    87,   125,    88,    -1,    87,   125,    90,    -1,
      -1,    91,   125,   120,    -1,    91,   125,   121,    -1,    -1,
     108,   125,   120,    -1,   108,   125,   121,    -1,    60,   125,
     209,   199,   210,    -1,    61,   125,   209,   199,   210,    -1,
      62,   125,   209,   199,   210,    -1,    68,   125,   221,    -1,
      69,    -1,    -1,    94,   125,     6,    -1,    -1,    95,   125,
     234,    -1,    95,   125,    96,    -1,    95,   125,    97,    -1,
      -1,    98,   125,   120,    -1,    98,   125,   121,    -1,    -1,
      99,   125,   234,    -1,    -1,   100,   125,   120,    -1,   100,
     125,   121,    -1,   107,   125,   234,    -1,   107,   125,   234,
     124,    -1,   107,   125,   234,   123,    -1,   212,    -1,   201,
      -1,   202,    -1,   111,   125,   234,    -1,   203,   204,    -1,
     204,   203,    -1,   109,   125,   234,    -1,   110,   125,   234,
      -1,    40,   125,    20,    -1,    40,   125,    21,   207,    -1,
      -1,   225,   208,    -1,   209,    -1,   211,    -1,   212,    -1,
     213,    -1,   209,   211,    -1,   209,   211,   212,    -1,   209,
     211,   212,   213,    -1,   102,   125,   231,   103,   125,   233,
      -1,   103,   125,   233,   102,   125,   231,    -1,   105,   125,
     231,   106,   125,   233,    -1,   106,   125,   233,   105,   125,
     231,    -1,   104,   125,   234,    -1,   104,   125,   234,   118,
      -1,   104,   125,   234,   119,    -1,   112,   125,   234,    -1,
     214,    -1,   215,    -1,    92,   125,   234,    93,   125,   234,
      -1,    93,   125,   234,    92,   125,   234,    -1,    73,   125,
     231,    74,   125,   233,    -1,    74,   125,   233,    73,   125,
     231,    -1,    -1,    75,   125,   234,    76,   125,   234,    -1,
      76,   125,   234,    75,   125,   234,    -1,    40,   125,    22,
     218,    -1,    -1,   225,   230,   229,    -1,    40,   125,    24,
     220,    -1,    -1,   225,   221,   222,    -1,    70,   125,    71,
      -1,    70,   125,    72,    -1,    71,    -1,    72,    -1,    -1,
     223,    -1,   224,    -1,   223,   224,    -1,   224,   223,    -1,
      64,   125,   231,    65,   125,   231,    -1,    65,   125,   231,
      64,   125,   231,    -1,    66,   125,   233,    67,   125,   233,
      -1,    67,   125,   233,    66,   125,   233,    -1,   226,   227,
      -1,    54,   125,    -1,   228,    -1,    -1,    50,   125,    53,
      -1,    50,   125,    52,    -1,    50,   125,    51,    -1,    -1,
      80,   125,   231,    -1,    81,   125,   126,    -1,    81,   125,
     127,    -1,   232,    -1,   232,   128,    -1,   232,   129,    -1,
     234,    -1,   234,   126,    -1,   234,   127,    -1,   234,    -1,
     234,   130,    -1,   234,   131,    -1,     3,    -1,     4,    -1,
       4,     7,   234,    -1,     5,   132,   234,   133,    -1,   234,
       9,   234,    -1,   234,     8,   234,    -1,   234,    10,   234,
      -1,   234,    11,   234,    -1,     8,   234,    -1,   234,    13,
     234,    -1,   132,   234,   133,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   249,   249,   250,   254,   277,   278,   282,   400,   423,
     424,   427,   428,   432,   436,   437,   441,   444,   445,   449,
     454,   460,   461,   467,   468,   475,   476,   477,   478,   479,
     482,   483,   497,   502,   507,   510,   511,   512,   513,   514,
     515,   519,   537,   552,   578,   579,   580,   581,   584,   584,
     603,   614,   625,   628,   644,   647,   652,   658,   664,   670,
     676,   682,   691,   695,   699,   702,   703,   707,   720,   767,
     770,   771,   772,   773,   788,   807,   808,   809,   810,   811,
     812,   817,   820,   829,   909,   918,   919,   921,   925,   926,
     927,   928,   929,   930,   934,   951,   952,   953,   954,   959,
     960,   961,   962,   966,   980,   994,  1008,  1021,  1022,  1023,
    1024,  1027,  1029,  1030,  1033,  1034,  1035,  1038,  1052,  1065,
    1081,  1093,  1101,  1102,  1105,  1106,  1107,  1109,  1113,  1116,
    1120,  1125,  1126,  1129,  1130,  1131,  1134,  1135,  1136,  1140,
    1141,  1142,  1146,  1150,  1152,  1157,  1161,  1287,  1323,  1401,
    1403,  1408,  1412,  1413,  1414,  1432,  1437,  1442,  1468,  1473,
    1481,  1483,  1488,  1489,  1499,  1512,  1516,  1517,  1521,  1526,
    1534,  1546,  1560,  1561,  1563,  1600,  1650,  1652,  1659,  1729,
    1731,  1737,  1738,  1740,  1741,  1744,  1745,  1746,  1747,  1748,
    1752,  1762,  1775,  1780,  1805,  1810,  1814,  1817,  1818,  1820,
    1822,  1877,  1878,  1881,  1882,  1886,  1887,  1888,  1895,  1902,
    1910,  1921,  1934,  1950,  1970,  1971,  1972,  1973,  1974,  1975,
    1976,  1977,  1990,  1991,  1992
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
  "TERMINATOR", "SUPERSAMPLE_VERIFY", "SUB_OBSERV_LAT", "SUB_OBSERV_LON",
  "POSITION_ANGLE", "SUB_SOLAR_LAT", "SUB_SOLAR_LON", "RANGE",
  "_REMOVE_SKY", "FOCAL_LENGTH", "PIXEL_SCALE", "ARCSEC_PER_PIX",
  "KM_PER_PIXEL", "BYTE_DATA", "SHORT_DATA", "LONG_DATA", "FLOAT_DATA",
  "DOUBLE_DATA", "CW", "CCW", "YES", "NO", "UNMATCHED", "AU", "KM", "':'",
  "'N'", "'S'", "'C'", "'G'", "'E'", "'W'", "'('", "')'", "$accept", "map",
  "user_file_parse", "map_parse", "map_setup", "map_entry", "author",
  "origin", "comments", "comment", "comment_setup", "xcomment",
  "xcomment_setup", "data_info", "data_offset", "data_scale", "data_type",
  "data_blank", "grid", "grid_yes_or_no", "grid_intervals",
  "grid_interval", "lat_grid_interval", "lon_grid_interval",
  "projection_type", "planes", "samples", "lines", "size", "body",
  "rotation", "radii", "eq_rad", "pol_rad", "flattening", "plane",
  "plane_setup", "plane_size", "plane_data_range", "plane_type", "image",
  "image_setup", "image_initialize", "supersample_verify", "nibbling",
  "nibble", "nibble_samples", "nibble_lines", "nibble_left",
  "nibble_right", "nibble_top", "nibble_bottom", "inversion",
  "image_interpolate", "remove_sky", "mu", "mu0", "phase", "lat_plane",
  "lon_plane", "flat_field", "photo_correct", "geom_correct",
  "emi_ang_limit", "terminator", "range", "image_geometry",
  "arcsec_per_pix", "lens_geometry", "focal_length", "pixel_scale",
  "mercator", "ortho", "ortho_options", "ortho_optsub", "sub_observ",
  "sub_solar", "position_angle", "km_per_pixel", "centers", "center_given",
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
static const unsigned short yytoknum[] =
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
     369,   370,   371,   372,   373,    58,    78,    83,    67,    71,
      69,    87,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   134,   135,   135,   136,   137,   137,   138,   139,   140,
     140,   141,   141,   142,   143,   143,   144,   145,   145,   146,
     147,   148,   148,   149,   149,   150,   150,   150,   150,   150,
     151,   151,   152,   153,   153,   154,   154,   154,   154,   154,
     154,   155,   156,   157,   158,   158,   158,   158,   159,   159,
     160,   161,   162,   163,   164,   164,   165,   165,   165,   165,
     165,   165,   166,   167,   168,   169,   169,   170,   171,   171,
     172,   172,   172,   172,   172,   173,   173,   173,   173,   173,
     173,   174,   174,   175,   176,   177,   177,   177,   178,   178,
     178,   178,   178,   178,   179,   180,   180,   180,   180,   181,
     181,   181,   181,   182,   183,   184,   185,   186,   186,   186,
     186,   187,   187,   187,   188,   188,   188,   189,   190,   191,
     192,   193,   194,   194,   195,   195,   195,   195,   196,   196,
     196,   197,   197,   198,   198,   198,   199,   199,   199,   200,
     200,   200,   201,   202,   202,   203,   204,   205,   206,   207,
     207,   208,   208,   208,   208,   208,   208,   208,   209,   209,
     210,   210,   211,   211,   211,   212,   213,   213,   214,   214,
     215,   215,   216,   216,   216,   217,   218,   218,   219,   220,
     220,   221,   221,   221,   221,   222,   222,   222,   222,   222,
     223,   223,   224,   224,   225,   226,   227,   228,   228,   228,
     228,   229,   229,   230,   230,   231,   231,   231,   232,   232,
     232,   233,   233,   233,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     4,     1,     2,    12,     3,     0,
       3,     0,     3,     2,     0,     2,     3,     0,     2,     3,
       4,     0,     3,     0,     3,     3,     3,     3,     3,     3,
       0,     3,     2,     3,     3,     0,     1,     1,     1,     2,
       2,     3,     3,     3,     1,     1,     1,     1,     0,     3,
       3,     3,     1,     5,     3,     3,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     1,     2,     3,     3,     2,
       0,     3,     3,     6,     6,     1,     1,     1,     1,     1,
       1,     1,     2,    18,     3,     0,     3,     3,     0,     1,
       1,     1,     2,     2,     3,     1,     1,     2,     2,     1,
       1,     2,     2,     3,     3,     3,     3,     0,     3,     3,
       3,     0,     3,     3,     0,     3,     3,     5,     5,     5,
       3,     1,     0,     3,     0,     3,     3,     3,     0,     3,
       3,     0,     3,     0,     3,     3,     3,     4,     4,     1,
       1,     1,     3,     2,     2,     3,     3,     3,     4,     0,
       2,     1,     1,     1,     1,     2,     3,     4,     6,     6,
       6,     6,     3,     4,     4,     3,     1,     1,     6,     6,
       6,     6,     0,     6,     6,     4,     0,     3,     4,     0,
       3,     3,     3,     1,     1,     0,     1,     1,     2,     2,
       6,     6,     6,     6,     2,     2,     1,     0,     3,     3,
       3,     0,     3,     3,     3,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     1,     3,     4,     3,     3,
       3,     3,     2,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      35,     0,     0,     0,     0,     0,     3,     2,     5,     9,
      70,    36,    37,    38,     0,     0,     0,     0,     1,     6,
       0,    11,     0,     0,    85,    39,    40,     8,   214,   215,
       0,     0,     0,    41,    52,    42,    43,     0,     0,    14,
       0,     0,     0,    88,     0,     0,   222,     0,     0,     0,
       0,     0,     0,    10,     0,     0,     0,    17,    14,    71,
      72,     0,     0,     0,     0,     0,     0,     4,    89,    90,
      91,    95,    96,    99,   100,   216,     0,   224,   219,   218,
     220,   221,   223,    12,     0,     0,    21,     0,    13,    17,
      15,     0,     0,    86,    87,     0,     0,     0,     0,     0,
      92,    93,    97,    98,   101,   102,   217,    16,     0,     0,
       0,    23,     0,    18,     0,     0,    94,   103,   104,   105,
     106,     0,     0,     0,     0,    35,     0,     0,    19,    73,
      74,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,    48,    44,    45,    46,    47,    32,     0,     0,    30,
       0,     0,     0,     0,    53,    56,    58,    57,    60,    59,
      61,    33,    34,     0,     0,     0,    24,     0,     0,    20,
      64,    62,    63,     0,   147,   149,   176,   179,     0,     0,
       0,    25,    26,    27,    28,    29,     0,    54,    55,     0,
     148,     0,   197,   175,     0,   178,     0,    49,     0,     0,
       0,    31,   195,     0,     0,     0,     0,     0,     0,     0,
       0,   150,   151,   152,   153,   154,   166,   167,     0,   194,
     196,     0,   201,     0,   183,   184,   185,    50,     0,     0,
       7,    65,    70,     0,     0,     0,     0,     0,     0,     0,
       0,   155,     0,     0,     0,   177,     0,     0,     0,     0,
       0,   180,   186,   187,    51,    69,    66,     0,     0,   205,
     208,     0,   211,     0,     0,     0,     0,   162,   165,   156,
     200,   199,   198,   203,   204,     0,   181,   182,     0,     0,
       0,     0,   188,   189,    68,     0,     0,     0,     0,     0,
     121,    67,    75,    81,    85,    76,    77,    78,    79,    80,
       0,   206,   207,   209,   210,     0,   212,   213,     0,     0,
       0,     0,   163,   164,   157,   202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,   120,   107,   170,   171,   168,   169,   158,   159,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,   190,
     191,   192,   193,     0,     0,     0,   117,   118,   119,     0,
       0,   114,   136,     0,     0,   109,   108,   110,     0,     0,
       0,   138,   137,     0,     0,   112,   113,     0,   172,     0,
       0,   115,   116,     0,     0,   122,     0,     0,     0,     0,
       0,   124,   160,   161,     0,     0,     0,     0,   128,     0,
       0,   123,     0,     0,   131,     0,     0,   126,   127,   125,
       0,     0,   133,   173,   174,   129,   130,     0,     0,     0,
     132,     0,     0,   134,   135,     0,     0,     0,     0,     0,
       0,    83,   140,   141,     0,     0,   139,     0,     0,     0,
     143,   144,   145,   146,   142
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     5,     6,     7,     8,     9,    21,    39,    56,    57,
      58,    88,    89,   110,   111,   127,   149,   169,   124,   125,
      10,    11,    12,    13,   141,   165,   180,   200,    33,    86,
     154,   134,   135,   136,   137,   230,   231,   232,    24,   291,
     292,   293,   294,    43,    67,    68,    69,    70,    71,    72,
      73,    74,   358,   371,   380,   295,   296,   297,   298,   299,
     401,   408,   414,   422,   429,   354,   441,   442,   443,   444,
     445,   142,   143,   190,   211,   212,   366,   213,   214,   215,
     216,   217,   395,   144,   193,   145,   195,   226,   251,   252,
     253,   191,   192,   219,   220,   245,   222,   258,   259,   261,
      34
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -310
static const short yypact[] =
{
     112,  -115,  -110,   -90,   -54,    77,  -310,  -310,    63,    79,
      11,  -310,    57,    76,   110,     4,     4,     4,  -310,  -310,
      -7,    93,     3,     7,    34,  -310,  -310,  -310,  -310,   180,
      26,     4,     4,  -310,   219,  -310,  -310,   163,    40,   145,
       4,     4,    85,   151,     4,     4,   203,    30,     4,     4,
       4,     4,     4,  -310,   225,   118,   213,   271,   145,   257,
     259,   -17,   181,   183,   184,   185,   186,  -310,  -310,    68,
      89,   221,   224,   226,   228,   219,    37,  -310,   149,   149,
     203,   203,   203,  -310,   308,   191,   282,   192,  -310,   271,
    -310,   198,   199,  -310,  -310,     4,     4,     4,     4,     4,
    -310,  -310,  -310,  -310,  -310,  -310,  -310,  -310,   316,   200,
     290,   293,   322,  -310,     4,     4,  -310,  -310,  -310,  -310,
    -310,   -27,     4,   205,   289,   103,   206,   301,  -310,  -310,
    -310,   210,   211,   212,   277,    -5,   -13,   190,  -310,    92,
     214,   275,  -310,  -310,  -310,  -310,  -310,     4,   215,   306,
       4,     4,     4,   220,  -310,  -310,  -310,  -310,  -310,  -310,
    -310,  -310,  -310,   218,   222,   303,  -310,   138,   223,  -310,
     219,   219,   219,   217,  -310,   292,   292,   292,     4,   227,
     307,  -310,  -310,  -310,  -310,  -310,     4,  -310,  -310,   229,
    -310,    64,   300,  -310,   270,  -310,   128,  -310,     4,   230,
     309,   219,  -310,   231,   232,   233,   234,   235,   236,   237,
     238,  -310,   260,  -310,  -310,  -310,  -310,  -310,   244,  -310,
    -310,   245,   291,   247,  -310,  -310,   114,  -310,     4,   248,
    -310,   309,    11,     4,     4,     4,     4,     4,     4,     4,
       4,   262,   207,   140,   250,  -310,   197,   251,   252,   253,
     254,  -310,   204,   208,  -310,     4,  -310,   157,   312,   146,
      59,   310,    52,    84,   102,   279,   278,    78,   219,     6,
    -310,  -310,  -310,  -310,  -310,     4,  -310,  -310,     4,     4,
       4,     4,  -310,  -310,  -310,   263,   264,   265,   266,   267,
    -310,  -310,  -310,   329,    34,  -310,  -310,  -310,  -310,  -310,
     268,  -310,  -310,  -310,  -310,   269,  -310,  -310,   272,   273,
     274,   276,  -310,  -310,  -310,  -310,   330,   336,   335,   337,
     398,   174,   174,   174,   128,  -310,   151,     4,     4,     4,
       4,     4,     4,   280,   281,   283,   284,  -310,   304,   304,
     304,  -310,   320,  -310,  -310,   219,   219,  -310,  -310,     4,
       4,     4,     4,   285,   175,   175,   175,   287,   323,  -310,
    -310,  -310,  -310,     4,   288,   294,  -310,  -310,  -310,   173,
     295,   313,    65,     4,     4,  -310,  -310,  -310,   158,   297,
       6,  -310,  -310,   317,   319,  -310,  -310,   162,   209,   302,
     305,  -310,  -310,   311,   314,   321,     4,     4,     4,     4,
     315,   331,  -310,  -310,   135,   182,   419,   318,   333,   324,
     325,  -310,    29,   326,   334,     4,     4,  -310,  -310,   219,
     166,   327,   328,   219,   219,  -310,  -310,     4,   332,   174,
     219,   168,   260,  -310,  -310,   175,   304,   111,   338,   339,
     340,  -310,  -310,  -310,   343,   345,  -310,     4,     4,     4,
    -310,  -310,   219,   219,   219
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -310,  -310,  -310,   421,  -310,  -310,  -310,  -310,  -310,   374,
    -310,   346,  -310,  -310,  -310,  -310,  -310,  -310,  -310,  -310,
     341,  -310,   424,   422,  -310,  -310,  -310,  -310,   -14,  -310,
    -310,  -310,   154,   -71,   159,   216,  -310,  -310,   239,  -310,
     148,  -310,  -310,   144,   119,  -310,   372,   375,   376,   384,
     382,   373,  -310,  -310,  -310,  -310,  -310,  -310,  -310,  -310,
    -310,  -310,  -310,  -310,  -310,  -309,  -310,  -310,  -310,    13,
      15,  -310,  -310,  -310,  -310,  -298,  -301,  -208,  -236,  -263,
    -310,  -310,  -310,  -310,  -310,  -310,  -310,   136,  -310,   240,
     242,   116,  -310,  -310,  -310,  -310,  -310,  -226,  -310,  -222,
     -31
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short yytable[] =
{
      46,    47,    35,    36,   241,   269,   314,    28,    29,    30,
      14,   265,    31,    75,    76,    15,   266,    78,    79,    80,
      81,    82,   131,   338,   339,   340,    59,    60,   132,   133,
     355,   356,    28,    29,    30,    16,   131,    31,    48,    49,
      50,    51,   132,    52,   131,    48,    49,    50,    51,   315,
      52,   133,   316,   317,   367,   368,    22,    23,   318,   319,
      48,    49,    50,    51,   155,    52,   160,    48,    49,    50,
      51,    17,    52,    48,    49,    50,    51,    18,    52,   203,
     204,   116,   117,   118,   119,   120,    48,    49,    50,    51,
       1,    52,    48,    49,    50,    51,     4,    52,   205,   206,
     129,   130,   344,    93,    94,   343,   348,    20,   138,   347,
      48,    49,    50,    51,     3,    52,    27,   388,    37,   170,
     171,   172,    38,   359,   360,   417,   418,   437,    40,   361,
     362,   432,    41,   166,   436,    42,    32,   203,   204,     1,
       2,     3,     4,    48,    49,    50,    51,   383,    52,     2,
       3,     4,   384,    65,    66,   201,   205,   206,    45,    50,
      51,    32,    52,    77,   197,    54,   207,   208,   209,    53,
     106,   403,    63,    64,   402,    55,   210,   308,   247,   248,
     249,   250,   306,   307,   227,   303,   304,    44,   381,   382,
      48,    49,    50,    51,   309,    52,   312,   313,   223,   224,
     225,   446,   260,   262,   263,   264,   260,   262,   267,   268,
      61,   409,   161,   162,   254,   285,    52,   286,   287,   288,
     438,   439,   440,   210,   435,   289,   290,    48,    49,    50,
      51,    83,    52,    62,    63,    64,    65,    66,   174,   175,
     176,   284,   177,    84,   260,   132,   133,   260,   260,   262,
     262,   181,   182,   183,   184,   185,    85,   410,   270,   271,
     272,   375,   376,   377,   187,   188,   273,   274,   276,   277,
     249,   250,   247,   248,   301,   302,   207,   208,   385,   386,
     364,   365,   391,   392,   393,   394,   425,   426,   433,   434,
     157,   159,   194,   196,   156,   158,   262,   260,   345,   346,
     262,   260,    87,    91,    92,    64,    95,    63,    96,    97,
      98,    99,    66,    65,   107,   109,   108,   112,   260,   260,
     262,   262,   121,   114,   115,   122,   123,   126,   128,   140,
     139,   147,   372,   148,   153,   150,   151,   152,   164,   163,
     167,   168,   260,   262,   179,   173,   189,   178,   186,   199,
     218,   221,   198,   229,   202,   228,   233,   234,   235,   236,
     237,   238,   239,   240,   209,   262,   260,   404,   405,   242,
     243,   244,   246,   255,   210,   275,   278,   279,   280,   281,
     311,   419,   310,   305,   423,   424,   300,   285,   320,   321,
     322,   323,   324,   327,   328,   333,   430,   329,   330,   331,
     334,   332,   335,   336,   337,   349,   350,   357,   351,   352,
     363,   353,   369,   373,   370,   400,   452,   453,   454,   374,
     378,   379,   387,   389,   390,   411,   407,   396,   428,    19,
     397,   413,    90,   421,    25,   113,   398,    26,   326,   399,
     406,   325,   101,   412,   100,   342,   104,   256,   103,   415,
     416,   420,   427,   439,   438,   102,   105,   431,   451,   450,
     341,     0,     0,   447,   448,   449,   146,     0,     0,     0,
       0,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   283,   282
};

static const short yycheck[] =
{
      31,    32,    16,    17,   212,   241,   269,     3,     4,     5,
     125,   237,     8,    44,    45,   125,   238,    48,    49,    50,
      51,    52,    49,   321,   322,   323,    40,    41,    55,    56,
     339,   340,     3,     4,     5,   125,    49,     8,     8,     9,
      10,    11,    55,    13,    49,     8,     9,    10,    11,   275,
      13,    56,   278,   279,   355,   356,    45,    46,   280,   281,
       8,     9,    10,    11,   135,    13,   137,     8,     9,    10,
      11,   125,    13,     8,     9,    10,    11,     0,    13,    73,
      74,    95,    96,    97,    98,    99,     8,     9,    10,    11,
      27,    13,     8,     9,    10,    11,    39,    13,    92,    93,
     114,   115,   328,   120,   121,   327,   332,    28,   122,   331,
       8,     9,    10,    11,    38,    13,     6,   380,   125,   150,
     151,   152,    29,   349,   350,    96,    97,   436,   125,   351,
     352,   429,   125,   147,   435,   101,   132,    73,    74,    27,
      37,    38,    39,     8,     9,    10,    11,   373,    13,    37,
      38,    39,   374,    85,    86,   186,    92,    93,   132,    10,
      11,   132,    13,   133,   178,   125,   102,   103,   104,     6,
     133,   397,    83,    84,   396,    30,   112,    93,    64,    65,
      66,    67,   130,   131,   198,   126,   127,     7,   123,   124,
       8,     9,    10,    11,    92,    13,   118,   119,    70,    71,
      72,   437,   233,   234,   235,   236,   237,   238,   239,   240,
     125,    76,   120,   121,   228,    58,    13,    60,    61,    62,
     109,   110,   111,   112,   432,    68,    69,     8,     9,    10,
      11,     6,    13,    82,    83,    84,    85,    86,    20,    21,
      22,   255,    24,   125,   275,    55,    56,   278,   279,   280,
     281,   113,   114,   115,   116,   117,    43,    75,    51,    52,
      53,    88,    89,    90,    47,    48,   126,   127,    71,    72,
      66,    67,    64,    65,   128,   129,   102,   103,   120,   121,
     105,   106,   120,   121,    75,    76,   120,   121,   120,   121,
     136,   137,   176,   177,   135,   136,   327,   328,   329,   330,
     331,   332,    31,    46,    45,    84,   125,    83,   125,   125,
     125,   125,    86,    85,     6,    33,   125,   125,   349,   350,
     351,   352,     6,   125,   125,   125,    36,    34,     6,    40,
     125,   125,   363,    32,    57,   125,   125,   125,    63,   125,
     125,    35,   373,   374,    41,   125,    54,   125,   125,    42,
      50,    81,   125,    44,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   104,   396,   397,   398,   399,   125,
     125,    80,   125,   125,   112,   125,   125,   125,   125,   125,
     102,   412,   103,    73,   415,   416,    74,    58,   125,   125,
     125,   125,   125,   125,   125,    65,   427,   125,   125,   125,
      64,   125,    67,    66,     6,   125,   125,    87,   125,   125,
     125,   107,   125,   125,    91,    94,   447,   448,   449,   125,
     125,   108,   125,   106,   105,     6,    95,   125,   100,     8,
     125,    98,    58,    99,    12,    89,   125,    13,   294,   125,
     125,   293,    70,   125,    69,   326,    73,   231,    72,   125,
     125,   125,   125,   110,   109,    71,    74,   125,   445,   444,
     324,    -1,    -1,   125,   125,   125,   125,    -1,    -1,    -1,
      -1,   232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   253,   252
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    27,    37,    38,    39,   135,   136,   137,   138,   139,
     154,   155,   156,   157,   125,   125,   125,   125,     0,   137,
      28,   140,    45,    46,   172,   157,   156,     6,     3,     4,
       5,     8,   132,   162,   234,   162,   162,   125,    29,   141,
     125,   125,   101,   177,     7,   132,   234,   234,     8,     9,
      10,    11,    13,     6,   125,    30,   142,   143,   144,   162,
     162,   125,    82,    83,    84,    85,    86,   178,   179,   180,
     181,   182,   183,   184,   185,   234,   234,   133,   234,   234,
     234,   234,   234,     6,   125,    43,   163,    31,   145,   146,
     143,    46,    45,   120,   121,   125,   125,   125,   125,   125,
     181,   180,   183,   182,   185,   184,   133,     6,   125,    33,
     147,   148,   125,   145,   125,   125,   162,   162,   162,   162,
     162,     6,   125,    36,   152,   153,    34,   149,     6,   162,
     162,    49,    55,    56,   165,   166,   167,   168,   162,   125,
      40,   158,   205,   206,   217,   219,   154,   125,    32,   150,
     125,   125,   125,    57,   164,   167,   168,   166,   168,   166,
     167,   120,   121,   125,    63,   159,   162,   125,    35,   151,
     234,   234,   234,   125,    20,    21,    22,    24,   125,    41,
     160,   113,   114,   115,   116,   117,   125,    47,    48,    54,
     207,   225,   226,   218,   225,   220,   225,   162,   125,    42,
     161,   234,   125,    73,    74,    92,    93,   102,   103,   104,
     112,   208,   209,   211,   212,   213,   214,   215,    50,   227,
     228,    81,   230,    70,    71,    72,   221,   162,   125,    44,
     169,   170,   171,   125,   125,   125,   125,   125,   125,   125,
     125,   211,   125,   125,    80,   229,   125,    64,    65,    66,
      67,   222,   223,   224,   162,   125,   169,   172,   231,   232,
     234,   233,   234,   234,   234,   231,   233,   234,   234,   212,
      51,    52,    53,   126,   127,   125,    71,    72,   125,   125,
     125,   125,   224,   223,   162,    58,    60,    61,    62,    68,
      69,   173,   174,   175,   176,   189,   190,   191,   192,   193,
      74,   128,   129,   126,   127,    73,   130,   131,    93,    92,
     103,   102,   118,   119,   213,   231,   231,   231,   233,   233,
     125,   125,   125,   125,   125,   174,   177,   125,   125,   125,
     125,   125,   125,    65,    64,    67,    66,     6,   209,   209,
     209,   221,   178,   233,   231,   234,   234,   233,   231,   125,
     125,   125,   125,   107,   199,   199,   199,    87,   186,   231,
     231,   233,   233,   125,   105,   106,   210,   210,   210,   125,
      91,   187,   234,   125,   125,    88,    89,    90,   125,   108,
     188,   123,   124,   231,   233,   120,   121,   125,   213,   106,
     105,   120,   121,    75,    76,   216,   125,   125,   125,   125,
      94,   194,   233,   231,   234,   234,   125,    95,   195,    76,
      75,     6,   125,    98,   196,   125,   125,    96,    97,   234,
     125,    99,   197,   234,   234,   120,   121,   125,   100,   198,
     234,   125,   209,   120,   121,   211,   210,   199,   109,   110,
     111,   200,   201,   202,   203,   204,   212,   125,   125,   125,
     204,   203,   234,   234,   234
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

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
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

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value, Location);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
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
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

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
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

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
yydestruct (int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yytype, yyvaluep, yylocationp)
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

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
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  YYLTYPE *yylerrsp;

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
	short *yyss1 = yyss;
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
	short *yyss1 = yyss;
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
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

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
#line 254 "parse.yy"
    {
          // Reset defaults to those chosen by the user.  If none were
          // chosen by the user, the values will remain unchanged.

          pp.lat_interval  = lat_interval;
          pp.lon_interval  = lon_interval;

          if (!isnan (minimum))
              pp.minimum = minimum;

          if (!isnan (maximum))
              pp.maximum = maximum;

          pp.supersample_verify = supersample_verify;

          pp.nibble_left   = nibble_left_val;
          pp.nibble_right  = nibble_right_val;
          pp.nibble_top    = nibble_top_val;
          pp.nibble_bottom = nibble_bottom_val;
        }
    break;

  case 7:
#line 293 "parse.yy"
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
#line 400 "parse.yy"
    {
          map_filename = yyvsp[0].sval;
          free (yyvsp[0].sval);

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
#line 424 "parse.yy"
    { map_author = yyvsp[0].sval; free (yyvsp[0].sval); }
    break;

  case 12:
#line 428 "parse.yy"
    { map_origin = yyvsp[0].sval; free (yyvsp[0].sval); }
    break;

  case 16:
#line 441 "parse.yy"
    { comment_list.push_back (yyvsp[0].sval); free (yyvsp[0].sval); }
    break;

  case 19:
#line 449 "parse.yy"
    { xcomment_list.push_back (yyvsp[0].sval); free (yyvsp[0].sval); }
    break;

  case 22:
#line 461 "parse.yy"
    {
            fits_bzero = static_cast<float> (yyvsp[0].val);
            transform_data = true;
        }
    break;

  case 24:
#line 468 "parse.yy"
    {
            fits_bscale = static_cast<float> (yyvsp[0].val);
            transform_data = true;
        }
    break;

  case 25:
#line 475 "parse.yy"
    { map_data_type = BYTE;   }
    break;

  case 26:
#line 476 "parse.yy"
    { map_data_type = SHORT;  }
    break;

  case 27:
#line 477 "parse.yy"
    { map_data_type = LONG;   }
    break;

  case 28:
#line 478 "parse.yy"
    { map_data_type = FLOAT;  }
    break;

  case 29:
#line 479 "parse.yy"
    { map_data_type = DOUBLE; }
    break;

  case 31:
#line 483 "parse.yy"
    {
          if (map_data_type == FLOAT || map_data_type == DOUBLE)
            throw std::invalid_argument ("\"BLANK\" keyword not valid with "
                                         "floating point types.");
          else
            {
              fits_blank = static_cast<int> (yyvsp[0].val);
              blank_set = true;
            }
          ;
        }
    break;

  case 33:
#line 502 "parse.yy"
    {
          create_grid = true;
          lat_interval = pp.lat_interval;
          lon_interval = pp.lon_interval;
        }
    break;

  case 34:
#line 507 "parse.yy"
    { create_grid = false; }
    break;

  case 41:
#line 519 "parse.yy"
    {
          if (yyvsp[0].val <= 0)
            {
              std::ostringstream s;
              s << "Grid interval value (" << yyvsp[0].val << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            {
              lat_interval = yyvsp[0].val;
              lon_interval = yyvsp[0].val;
            }
        }
    break;

  case 42:
#line 537 "parse.yy"
    {
          if (yyvsp[0].val <= 0)
            {
              std::ostringstream s;
              s << "Latitude grid interval value (" << yyvsp[0].val << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            lat_interval = yyvsp[0].val;
        }
    break;

  case 43:
#line 552 "parse.yy"
    {
          if (yyvsp[0].val <= 0)
            {
              std::ostringstream s;
              s << "Longitude grid interval value (" << yyvsp[0].val << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            lon_interval = yyvsp[0].val;
        }
    break;

  case 49:
#line 584 "parse.yy"
    {
          /**
           * @deprecated The "PLANES" keyword is no longer necessary.
           *
           * @todo Remove support for the "PLANES" keyword.
           */

          // We really don't need to ask the user to set the number of
          // planes or the plane number any more.  The number of
          // planes is set dynamically as source images are queued.
          if (yyvsp[0].val > 0)
            num_planes = static_cast<unsigned int> (yyvsp[0].val);
          else
            std::cerr << "Incorrect number of planes entered: "
                      << yyvsp[0].val << std::endl;
        }
    break;

  case 50:
#line 603 "parse.yy"
    {
          if (yyvsp[0].val > 0)
            map_samples = static_cast<unsigned int> (yyvsp[0].val);
          else
            {
              std::cerr << "Incorrect value for SAMPLES entered: "
                        << yyvsp[0].val << std::endl;
            }
        }
    break;

  case 51:
#line 614 "parse.yy"
    {
          if (yyvsp[0].val > 0)
            map_lines = static_cast<unsigned int> (yyvsp[0].val);
          else
            {
              std::cerr << "Incorrect value for LINES entered: " << yyvsp[0].val
                        << std::endl;
            }
        }
    break;

  case 52:
#line 625 "parse.yy"
    { yyval.val = yyvsp[0].val; }
    break;

  case 53:
#line 630 "parse.yy"
    {
          body_name = yyvsp[-2].sval;
          free (yyvsp[-2].sval);

          oblate_spheroid =
            MaRC::ValuePtr<MaRC::OblateSpheroid> (
              new MaRC::OblateSpheroid (yyvsp[0].bval,
                                        (yyvsp[-1].radii_data).eq_rad,
                                        (yyvsp[-1].radii_data).pol_rad,
                                        (yyvsp[-1].radii_data).flattening));
        }
    break;

  case 54:
#line 644 "parse.yy"
    {
          yyval.bval = true;
        }
    break;

  case 55:
#line 647 "parse.yy"
    {
          yyval.bval = false;
        }
    break;

  case 56:
#line 653 "parse.yy"
    {
          (yyval.radii_data).eq_rad = yyvsp[-1].val;
          (yyval.radii_data).pol_rad = yyvsp[0].val;
          (yyval.radii_data).flattening = -1;
        }
    break;

  case 57:
#line 659 "parse.yy"
    {
          (yyval.radii_data).eq_rad = yyvsp[0].val;
          (yyval.radii_data).pol_rad = yyvsp[-1].val;
          (yyval.radii_data).flattening = -1;
        }
    break;

  case 58:
#line 665 "parse.yy"
    {
          (yyval.radii_data).eq_rad = yyvsp[-1].val;
          (yyval.radii_data).pol_rad = -1;
          (yyval.radii_data).flattening = yyvsp[0].val;
        }
    break;

  case 59:
#line 671 "parse.yy"
    {
          (yyval.radii_data).eq_rad = yyvsp[0].val;
          (yyval.radii_data).pol_rad = -1;
          (yyval.radii_data).flattening = yyvsp[-1].val;
        }
    break;

  case 60:
#line 677 "parse.yy"
    {
          (yyval.radii_data).eq_rad = -1;
          (yyval.radii_data).pol_rad = yyvsp[-1].val;
          (yyval.radii_data).flattening = yyvsp[0].val;
        }
    break;

  case 61:
#line 683 "parse.yy"
    {
          (yyval.radii_data).eq_rad = -1;
          (yyval.radii_data).pol_rad = yyvsp[0].val;
          (yyval.radii_data).flattening = yyvsp[-1].val;
        }
    break;

  case 62:
#line 691 "parse.yy"
    { yyval.val = yyvsp[0].val; }
    break;

  case 63:
#line 695 "parse.yy"
    { yyval.val = yyvsp[0].val; }
    break;

  case 64:
#line 699 "parse.yy"
    { yyval.val = yyvsp[0].val; }
    break;

  case 67:
#line 709 "parse.yy"
    {
          image_factory->minimum (minimum);
          image_factory->maximum (maximum);

          image_factories.push_back (image_factory);

          photo_factories.clear ();
        }
    break;

  case 68:
#line 720 "parse.yy"
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

          const unsigned int map_plane = static_cast<unsigned int> (yyvsp[0].val);

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
                    << "You entered:  " << yyvsp[0].val << std::endl;
                }
            }
          else
            {
              std::cerr << "Incorrect plane number entered." << std::endl
                        << "Expected plane number is: " << expected_plane
                        << std::endl
                        << "You entered:  " << yyvsp[0].val << std::endl;
            }

          minimum = pp.minimum; maximum = pp.maximum;
        }
    break;

  case 69:
#line 767 "parse.yy"
    { minimum = pp.minimum; maximum = pp.maximum; }
    break;

  case 71:
#line 771 "parse.yy"
    { minimum = yyvsp[0].val; }
    break;

  case 72:
#line 772 "parse.yy"
    { maximum = yyvsp[0].val; }
    break;

  case 73:
#line 774 "parse.yy"
    {
            if (yyvsp[-3].val < yyvsp[0].val)
              {
                minimum = yyvsp[-3].val;
                maximum = yyvsp[0].val;
              }
            else
              {
                std::cerr << "Minimum data value: " << yyvsp[-3].val
                          << " is greater than"
                          << std::endl
                          << "maximum data value: " << yyvsp[0].val << std::endl;
              }
        }
    break;

  case 74:
#line 789 "parse.yy"
    {
            if (yyvsp[0].val < yyvsp[-3].val)
              {
                minimum = yyvsp[0].val;
                maximum = yyvsp[-3].val;
              }
            else
              {
                std::cerr << "Minimum data value: " << yyvsp[0].val
                          << " is greater than"
                          << std::endl
                          << "maximum data value: " << yyvsp[-3].val
                          << std::endl;
              }
        }
    break;

  case 81:
#line 817 "parse.yy"
    {
          image_factory = photo_factory;
        }
    break;

  case 82:
#line 820 "parse.yy"
    {
            image_factory =
              MaRC::ValuePtr<MaRC::MosaicImageFactory> (
                new MaRC::MosaicImageFactory (photo_factories,
                                              averaging_type));
        }
    break;

  case 83:
#line 846 "parse.yy"
    {
          photo_factory->set_supersample_verify (supersample_verify);
          supersample_verify = pp.supersample_verify;

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

          photo_factory->sub_observ ((yyvsp[-4].sub_observ_data).lat, (yyvsp[-4].sub_observ_data).lon);
          photo_factory->position_angle (yyvsp[-3].val);
          photo_factory->sub_solar ((yyvsp[-2].sub_solar_data).lat, (yyvsp[-2].sub_solar_data).lon);
          photo_factory->range (yyvsp[-1].val);

          photo_factories.push_back (*photo_factory);
        }
    break;

  case 84:
#line 909 "parse.yy"
    {
          photo_factory =
            MaRC::ValuePtr<MaRC::PhotoImageFactory> (
              new MaRC::PhotoImageFactory (yyvsp[0].sval,
                                           *oblate_spheroid));
            free (yyvsp[0].sval);
        }
    break;

  case 86:
#line 919 "parse.yy"
    {
              supersample_verify = true; }
    break;

  case 87:
#line 921 "parse.yy"
    {
              supersample_verify = false; }
    break;

  case 94:
#line 934 "parse.yy"
    {
          if (yyvsp[0].val >= 0)
            {
              nibble_left_val   = static_cast<unsigned int> (yyvsp[0].val);
              nibble_right_val  = static_cast<unsigned int> (yyvsp[0].val);
              nibble_top_val    = static_cast<unsigned int> (yyvsp[0].val);
              nibble_bottom_val = static_cast<unsigned int> (yyvsp[0].val);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE entered: "
                        << yyvsp[0].val << std::endl;
            }
        }
    break;

  case 103:
#line 966 "parse.yy"
    {
          if (yyvsp[0].val >= 0)
            {
              nibble_left_val = static_cast<unsigned int> (yyvsp[0].val);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_LEFT entered: "
                        << yyvsp[0].val << std::endl;
            }
        }
    break;

  case 104:
#line 980 "parse.yy"
    {
          if (yyvsp[0].val >= 0)
            {
              nibble_right_val = static_cast<unsigned int> (yyvsp[0].val);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_RIGHT entered: "
                        << yyvsp[0].val << std::endl;
            }
        }
    break;

  case 105:
#line 994 "parse.yy"
    {
          if (yyvsp[0].val >= 0)
            {
              nibble_top_val = static_cast<unsigned int> (yyvsp[0].val);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_TOP entered: "
                        << yyvsp[0].val << std::endl;
            }
        }
    break;

  case 106:
#line 1008 "parse.yy"
    {
          if (yyvsp[0].val >= 0)
            {
              nibble_bottom_val = static_cast<unsigned int> (yyvsp[0].val);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_BOTTOM entered: "
                        << yyvsp[0].val << std::endl;
            }
        }
    break;

  case 108:
#line 1022 "parse.yy"
    { photo_factory->invert (true,  false); }
    break;

  case 109:
#line 1023 "parse.yy"
    { photo_factory->invert (false, true);  }
    break;

  case 110:
#line 1024 "parse.yy"
    { photo_factory->invert (true,  true);  }
    break;

  case 112:
#line 1029 "parse.yy"
    { photo_factory->interpolate (true);  }
    break;

  case 113:
#line 1030 "parse.yy"
    { photo_factory->interpolate (false); }
    break;

  case 115:
#line 1034 "parse.yy"
    { photo_factory->remove_sky (true);  }
    break;

  case 116:
#line 1035 "parse.yy"
    { photo_factory->remove_sky (false); }
    break;

  case 117:
#line 1041 "parse.yy"
    {
            // Mu * 10000
            image_factory =
              MaRC::ValuePtr<MaRC::MuImageFactory> (
                new MaRC::MuImageFactory (*oblate_spheroid,
                                          (yyvsp[-2].sub_observ_data).lat,
                                          (yyvsp[-2].sub_observ_data).lon,
                                          yyvsp[-1].val));
        }
    break;

  case 118:
#line 1055 "parse.yy"
    {
          // Mu0 * 10000
          image_factory =
            MaRC::ValuePtr<MaRC::Mu0ImageFactory> (
              new MaRC::Mu0ImageFactory (*oblate_spheroid,
                                         (yyvsp[0].sub_solar_data).lat,
                                         (yyvsp[0].sub_solar_data).lon));
        }
    break;

  case 119:
#line 1068 "parse.yy"
    {
          // cos (phase angle) * 10000
          image_factory =
            MaRC::ValuePtr<MaRC::CosPhaseImageFactory> (
              new MaRC::CosPhaseImageFactory (*oblate_spheroid,
                                              (yyvsp[-2].sub_observ_data).lat,
                                              (yyvsp[-2].sub_observ_data).lon,
                                              (yyvsp[0].sub_solar_data).lat,
                                              (yyvsp[0].sub_solar_data).lon,
                                              yyvsp[-1].val));
        }
    break;

  case 120:
#line 1081 "parse.yy"
    {
            // Latitudes in radians
            MaRC::ValuePtr<MaRC::BodyData> bd (
              new MaRC::OblateSpheroid (*oblate_spheroid));

            image_factory =
              MaRC::ValuePtr<MaRC::LatitudeImageFactory> (
                new MaRC::LatitudeImageFactory (bd, graphic_lat));
           }
    break;

  case 121:
#line 1093 "parse.yy"
    {
            // Longitudes in radians
            image_factory =
              MaRC::ValuePtr<MaRC::LongitudeImageFactory> (
                new MaRC::LongitudeImageFactory);
           }
    break;

  case 123:
#line 1102 "parse.yy"
    { photo_factory->flat_field (yyvsp[0].sval); free (yyvsp[0].sval);}
    break;

  case 125:
#line 1106 "parse.yy"
    { /* Image->setMinnaertExponent ($3); */ }
    break;

  case 126:
#line 1107 "parse.yy"
    {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_AUTO); */ }
    break;

  case 127:
#line 1109 "parse.yy"
    {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_TABLE); */ }
    break;

  case 129:
#line 1116 "parse.yy"
    {
            // Galileo spacecraft (GLL) geometric correction.
            photo_factory->geometric_correction (true);
          }
    break;

  case 130:
#line 1120 "parse.yy"
    {
            photo_factory->geometric_correction (false);
          }
    break;

  case 132:
#line 1126 "parse.yy"
    { photo_factory->emi_ang_limit (yyvsp[0].val); }
    break;

  case 134:
#line 1130 "parse.yy"
    { photo_factory->use_terminator (true);  }
    break;

  case 135:
#line 1131 "parse.yy"
    { photo_factory->use_terminator (false); }
    break;

  case 136:
#line 1134 "parse.yy"
    { yyval.val = yyvsp[0].val; }
    break;

  case 137:
#line 1135 "parse.yy"
    { yyval.val = yyvsp[-1].val; }
    break;

  case 138:
#line 1136 "parse.yy"
    { yyval.val = yyvsp[-1].val * C::astronomical_unit / 1000; }
    break;

  case 142:
#line 1146 "parse.yy"
    { arcsec_per_pix_val = yyvsp[0].val; }
    break;

  case 145:
#line 1157 "parse.yy"
    { focal_length_val = yyvsp[0].val; }
    break;

  case 146:
#line 1161 "parse.yy"
    { pixel_scale_val = yyvsp[0].val; }
    break;

  case 147:
#line 1287 "parse.yy"
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

  case 148:
#line 1324 "parse.yy"
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

  case 151:
#line 1408 "parse.yy"
    {
          sub_observation_data.lat = (yyvsp[0].sub_observ_data).lat;
          sub_observation_data.lon = (yyvsp[0].sub_observ_data).lon;
        }
    break;

  case 152:
#line 1412 "parse.yy"
    { position_angle_val = yyvsp[0].val; }
    break;

  case 154:
#line 1414 "parse.yy"
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

  case 155:
#line 1432 "parse.yy"
    {
          sub_observation_data.lat = (yyvsp[-1].sub_observ_data).lat;
          sub_observation_data.lon = (yyvsp[-1].sub_observ_data).lon;
            position_angle_val = yyvsp[0].val;
        }
    break;

  case 156:
#line 1437 "parse.yy"
    {
          sub_observation_data.lat = (yyvsp[-2].sub_observ_data).lat;
          sub_observation_data.lon = (yyvsp[-2].sub_observ_data).lon;
            position_angle_val = yyvsp[-1].val;
        }
    break;

  case 157:
#line 1442 "parse.yy"
    {
          sub_observation_data.lat = (yyvsp[-3].sub_observ_data).lat;
          sub_observation_data.lon = (yyvsp[-3].sub_observ_data).lon;
            position_angle_val = yyvsp[-2].val;
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

  case 158:
#line 1469 "parse.yy"
    {
          yyval.sub_observ_data.lat = yyvsp[-3].val;
          yyval.sub_observ_data.lon = yyvsp[0].val;
        }
    break;

  case 159:
#line 1474 "parse.yy"
    {
            yyval.sub_observ_data.lat = yyvsp[0].val;
            yyval.sub_observ_data.lon = yyvsp[-3].val;
        }
    break;

  case 160:
#line 1482 "parse.yy"
    { (yyval.sub_solar_data).lat = yyvsp[-3].val; (yyval.sub_solar_data).lon = yyvsp[0].val; }
    break;

  case 161:
#line 1484 "parse.yy"
    { (yyval.sub_solar_data).lat = yyvsp[0].val; (yyval.sub_solar_data).lon = yyvsp[-3].val; }
    break;

  case 162:
#line 1488 "parse.yy"
    { yyval.val = yyvsp[0].val; }
    break;

  case 163:
#line 1489 "parse.yy"
    {
            if (yyvsp[-1].val >= 0)
              yyval.val = yyvsp[-1].val;
            else
              {
                std::cerr << "Incorrect position (North) angle entered: "
                          << yyvsp[-1].val << " CW" << std::endl
                          << "Numeric value should be positive." << std::endl;
              }
        }
    break;

  case 164:
#line 1499 "parse.yy"
    {
            if (yyvsp[-1].val >= 0)
              yyval.val = yyvsp[-1].val;
            else
              {
                std::cerr << "Incorrect position (North) angle entered: "
                          << yyvsp[-1].val << " CCW" << std::endl
                          << "Numeric value should be positive." << std::endl;
              }
        }
    break;

  case 165:
#line 1512 "parse.yy"
    { km_per_pixel_val = yyvsp[0].val; }
    break;

  case 168:
#line 1522 "parse.yy"
    {
          sample_center = yyvsp[-3].val;
          line_center   = yyvsp[0].val;
        }
    break;

  case 169:
#line 1527 "parse.yy"
    {
            sample_center = yyvsp[0].val;
            line_center   = yyvsp[-3].val;
        }
    break;

  case 170:
#line 1535 "parse.yy"
    {
//           if (in_ortho_options)
//             MapEntry->setOrthoCenter ($3, $6, LAT_LON_GIVEN);
//           else if (in_perspective_options)
//             MapEntry->setPerspectiveCenter ($3, $6, LAT_LON_GIVEN);
//           else
//             Image->setLatLonCenter ($3, $6);

             lat_at_center = yyvsp[-3].val;
             lon_at_center = yyvsp[0].val;
        }
    break;

  case 171:
#line 1547 "parse.yy"
    {
//             if (in_ortho_options)
//               MapEntry->setOrthoCenter ($6, $3, LAT_LON_GIVEN);
//             else if (in_perspective_options)
//               MapEntry->setPerspectiveCenter ($6, $3, LAT_LON_GIVEN);
//             else
//               Image->setLatLonCenter ($6, $3);

             lat_at_center = yyvsp[0].val;
             lon_at_center = yyvsp[-3].val;
        }
    break;

  case 173:
#line 1562 "parse.yy"
    { photo_factory->optical_axis (yyvsp[-3].val, yyvsp[0].val); }
    break;

  case 174:
#line 1564 "parse.yy"
    { photo_factory->optical_axis (yyvsp[0].val, yyvsp[-3].val); }
    break;

  case 175:
#line 1601 "parse.yy"
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

  case 178:
#line 1660 "parse.yy"
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

  case 181:
#line 1737 "parse.yy"
    { graphic_lat = false; }
    break;

  case 182:
#line 1738 "parse.yy"
    { graphic_lat = true;  }
    break;

  case 183:
#line 1740 "parse.yy"
    { graphic_lat = false; }
    break;

  case 184:
#line 1741 "parse.yy"
    { graphic_lat = true;  }
    break;

  case 190:
#line 1753 "parse.yy"
    {
          if (yyvsp[-3].val < yyvsp[0].val)
            {
              lo_lat = yyvsp[-3].val;
              hi_lat = yyvsp[0].val;
            }
          else
            std::cerr << "Error: LO_LAT is greater than HI_LAT" << std::endl;
        }
    break;

  case 191:
#line 1763 "parse.yy"
    {
          if (yyvsp[-3].val < yyvsp[0].val)
            {
              lo_lat = yyvsp[0].val;
              hi_lat = yyvsp[-3].val;
            }
          else
            std::cerr << "Error: LO_LAT is greater than HI_LAT" << std::endl;
        }
    break;

  case 192:
#line 1776 "parse.yy"
    {
          lo_lon = yyvsp[-3].val;
          hi_lon = yyvsp[0].val;
        }
    break;

  case 193:
#line 1781 "parse.yy"
    {
          lo_lon = yyvsp[0].val;
          hi_lon = yyvsp[-3].val;
        }
    break;

  case 198:
#line 1818 "parse.yy"
    {
              averaging_type = MaRC::MosaicImage::AVG_UNWEIGHTED; }
    break;

  case 199:
#line 1820 "parse.yy"
    {
              averaging_type = MaRC::MosaicImage::AVG_WEIGHTED; }
    break;

  case 200:
#line 1822 "parse.yy"
    {
              averaging_type = MaRC::MosaicImage::AVG_NONE; }
    break;

  case 202:
#line 1878 "parse.yy"
    { max_lat = yyvsp[0].val; }
    break;

  case 203:
#line 1881 "parse.yy"
    { north_pole = true; }
    break;

  case 204:
#line 1882 "parse.yy"
    { north_pole = false; }
    break;

  case 205:
#line 1886 "parse.yy"
    { yyval.val = yyvsp[0].val; }
    break;

  case 206:
#line 1887 "parse.yy"
    { yyval.val = yyvsp[-1].val; }
    break;

  case 207:
#line 1888 "parse.yy"
    {
            /* Convert to CENTRIC latitude. */
            yyval.val = oblate_spheroid->centric_latitude (yyvsp[-1].val * C::degree)
                   / C::degree; }
    break;

  case 208:
#line 1895 "parse.yy"
    {
          if (::fabs (yyvsp[0].val) <= 90)
            yyval.val = yyvsp[0].val;
          else {
            std::cerr << "Incorrect latitude entered: " << yyvsp[0].val << std::endl;
          }
        }
    break;

  case 209:
#line 1902 "parse.yy"
    {
          if (yyvsp[-1].val >= 0 && yyvsp[-1].val <= 90)
            yyval.val = yyvsp[-1].val;
          else {
            std::cerr << "Incorrect latitude entered: " << yyvsp[-1].val << " N"
                      << std::endl;
          }
        }
    break;

  case 210:
#line 1910 "parse.yy"
    {
            if (yyvsp[-1].val >= 0 && yyvsp[-1].val <= 90)
              yyval.val = -yyvsp[-1].val;
            else {
              std::cerr << "Incorrect latitude entered: " << yyvsp[-1].val << " S"
                        << std::endl;
            }
        }
    break;

  case 211:
#line 1921 "parse.yy"
    {
                if (::fabs (yyvsp[0].val) <= 360)
                  {
                    if (yyvsp[0].val < 0)
                      yyvsp[0].val+= 360;
                    yyval.val = yyvsp[0].val;
                  }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << yyvsp[0].val
                              << std::endl;
                  }
          }
    break;

  case 212:
#line 1934 "parse.yy"
    {
                if (::fabs (yyvsp[-1].val) <= 360)
                  {
                    if (yyvsp[-1].val < 0)
                      yyvsp[-1].val+= 360;
                    if (oblate_spheroid->prograde ())
                      yyval.val = 360 - yyvsp[-1].val;
                    else
                      yyval.val = yyvsp[-1].val;
                }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << yyvsp[-1].val
                              << std::endl;
                  }
        }
    break;

  case 213:
#line 1950 "parse.yy"
    {
                if (::fabs (yyvsp[-1].val) <= 360)
                  {
                    if (yyvsp[-1].val < 0)
                      yyvsp[-1].val+= 360;
                    if (oblate_spheroid->prograde ())
                      yyval.val = yyvsp[-1].val;
                    else
                      yyval.val = 360 - yyvsp[-1].val;
                  }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << yyvsp[-1].val
                              << std::endl;
                  }
        }
    break;

  case 214:
#line 1970 "parse.yy"
    { yyval.val = yyvsp[0].val;                         }
    break;

  case 215:
#line 1971 "parse.yy"
    { yyval.val = yyvsp[0].tptr->value.var;              }
    break;

  case 216:
#line 1972 "parse.yy"
    { yyval.val = yyvsp[0].val; yyvsp[-2].tptr->value.var = yyvsp[0].val;     }
    break;

  case 217:
#line 1973 "parse.yy"
    { yyval.val = (* (yyvsp[-3].tptr->value.fnctptr)) (yyvsp[-1].val); }
    break;

  case 218:
#line 1974 "parse.yy"
    { yyval.val = yyvsp[-2].val + yyvsp[0].val;                    }
    break;

  case 219:
#line 1975 "parse.yy"
    { yyval.val = yyvsp[-2].val - yyvsp[0].val;                    }
    break;

  case 220:
#line 1976 "parse.yy"
    { yyval.val = yyvsp[-2].val * yyvsp[0].val;                    }
    break;

  case 221:
#line 1977 "parse.yy"
    { if (yyvsp[0].val != 0)
                                    yyval.val = yyvsp[-2].val / yyvsp[0].val;
                                  else
                                    {
                                      yyval.val = yyvsp[-2].val;
                                      fprintf (stderr,
                                               "%d.%d-%d.%d: division by zero",
                                               yylsp[0].first_line,
                                               yylsp[0].first_column,
                                               yylsp[0].last_line,
                                               yylsp[0].last_column);
                                    }
                                }
    break;

  case 222:
#line 1990 "parse.yy"
    { yyval.val = -yyvsp[0].val;                        }
    break;

  case 223:
#line 1991 "parse.yy"
    { yyval.val = ::pow (yyvsp[-2].val, yyvsp[0].val);             }
    break;

  case 224:
#line 1992 "parse.yy"
    { yyval.val = yyvsp[-1].val;                         }
    break;


    }

/* Line 1000 of yacc.c.  */
#line 3506 "parse.cc"

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

  yylerrsp = yylsp;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp, yylsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	  *++yylerrsp = yylloc;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
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

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  yylerrsp = yylsp;
  *++yylerrsp = yyloc;
  yylsp -= yylen;
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
  YYLLOC_DEFAULT (yyloc, yylsp, yylerrsp - yylsp);
  *++yylsp = yyloc;

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


#line 1996 "parse.yy"


