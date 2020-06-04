// -*- C++ -*-
/**
 * @file parse.yy
 *
 * Parser for %MaRC input files.  Requires GNU Bison 1.35 or greater.
 *
 * Copyright (C) 1999, 2004, 2017-2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

%{
// SourceImage factories
#include "PhotoImageFactory.h"
#include "MosaicImageFactory.h"
#include "MuImageFactory.h"
#include "Mu0ImageFactory.h"
#include "CosPhaseImageFactory.h"
#include "LatitudeImageFactory.h"
#include "LongitudeImageFactory.h"

// BodyData strategies
#include <marc/OblateSpheroid.h>

// Image parameters
#include <marc/PhotoImageParameters.h>
#include <marc/ViewingGeometry.h>

// Map Strategies
#include <marc/Mercator.h>
#include <marc/Orthographic.h>
#include <marc/PolarStereographic.h>
#include <marc/SimpleCylindrical.h>

#include <marc/Constants.h>
#include <marc/DefaultConfiguration.h>

#include <marc/Log.h>
#include <marc/utility.h>

#include "parse_scan.h"
#include "parse.hh"
#include "lexer.hh"
#include "calc.h"
#include "strerror.h"
#include "MapCommand.h"
#include "FITS_traits.h"

#include <stdexcept>
#include <limits>
#include <memory>
#include <cstring>
#include <cerrno>
#include <cmath>


YY_DECL;

namespace
{
    void yyerror(YYLTYPE * locp,
                 yyscan_t /* scanner */,
                 MaRC::ParseParameter & pp,
                 char const * msg)
    {
        /**
         * @bug The line number is off when the parser explicitly
         *      calls @c yyerror().  For example, if a negative
         *      KM_PER_PIXEL value is set in an input file the
         *      reported line is actually the first line that isn't
         *      solely whitespace after the line containing the
         *      invalid KM_PER_PIXEL value.  Line numbers in syntax
         *      errors, on the other hand, are correct
         */
        MaRC::error("{}:{}:{}: {}",
                    pp.filename,
                    locp->first_line,
                    locp->first_column,
                    msg);
    }

    static constexpr double not_a_number =
        std::numeric_limits<double>::signaling_NaN();

    /**
     * @brief Get angle within 360 degree (circle) range.
     *
     * Determine the angle in the [0,360) (circle) range equivalent to
     * the provided @a angle.  For example, given an @a angle of -365
     * degrees, the equivalent angle in the [0,360) range would be 355
     * degrees.
     *
     * @param[in] angle Angle in degrees to be placed in 360 degree
     *                  range.
     *
     * @return Angle in [0,360) degree range equivalent to the
     *         provided @a angle.
     *
     * @todo Merge this function with similar generalized code used in
     *       @c MaRC::LongitudeImage::read_data_i().
     */
    inline double
    circle_angle(double angle)
    {
        constexpr int circle = 360;  // degrees

        // Place in the range [-360, 360].
        angle = std::fmod(angle, circle);

        // The sign of the first argument to std::fmod(), angle in
        // this case, is retained.  Shift to the [0, 360] range if
        // negative.
        return angle >= 0 ? angle : angle + circle;
    }

    /**
     * @struct blank_verifier
     *
     * @brief 32-bit, or smaller, blank integer value verifier.
     *
     * @tparam T Map integer data type.
     *
     * @see verify_data_blank()
     */
    template<typename T>
    struct blank_verifier
    {
        /**
         * @brief Verify that the provided %FITS @c BLANK value actually
         *        fits within the chosen map integer data type.
         *
         * @param[in] blank     %FITS @c BLANK value explicitly
         *                      provided by the user.
         * @param[in] data_type %MaRC map data type name (e.g. "BYTE",
         *                      "SHORT", or "LONG").
         *
         * @retval true  @a blank fits within chosen map integer data
         *               type.
         * @retval false @a blank is too large for chosen map integer
         *               data type.
         */
        static bool verify(MaRC::blank_type::value_type blank,
                           char const * data_type)
        {
            constexpr auto minimum = std::numeric_limits<T>::lowest();
            constexpr auto maximum = std::numeric_limits<T>::max();

            bool const verified = (blank >= minimum && blank <= maximum);

            if (!verified)
                MaRC::error("DATA_BLANK {} does not fit within "
                            "map data type {}",
                            blank,
                            data_type);

            return verified;
        }
    };

    /**
     * @struct blank_verifier<MaRC::FITS::longlong_type>
     *
     * @brief 64-bit blank integer value verifier specialization.
     *
     * In this case the 64-bit blank value always verifies since the
     * largest integer value supported by the %FITS standard is 64
     * bits wide.
     *
     * @see verify_data_blank()
     */
    template<>
    struct blank_verifier<MaRC::FITS::longlong_type>
    {
        /**
         * @brief Verify that the provided %FITS @c BLANK value actually
         *        fits within the chosen map integer data type.
         *
         * @param[in] blank     %FITS @c BLANK value explicitly
         *                      provided by the user.
         * @param[in] data_type %MaRC map data type name (e.g. "BYTE",
         *                      "SHORT", or "LONG").
         *
         * @retval true Verification always succeeds for the 64-bit
         *              integer typed maps.
         */

        static bool verify(MaRC::blank_type::value_type /* blank */,
                           char const * /* data_type */)
        {
            return true;
        }
    };

    /**
     * @brief Verify that the provided %FITS @c BLANK value actually
     *        fits within the chosen map data type.
     *
     * @tparam    T         Map data type.
     * @param[in] blank     %FITS @c BLANK value explicitly provided
     *                      by the user.  While it is of type
     *                      @c double here, it is ultimately stored as
     *                      an integer.
     * @param[in] data_type %MaRC map data type name (e.g. "BYTE",
     *                      "SHORT", etc).
     *
     * @retval true  @a blank fits within chosen map data type.
     * @retval false @a blank is too large for chosen map data type.
     */
    template<typename T>
    bool
    verify_data_blank(MaRC::blank_type::value_type blank,
                      char const * data_type)
    {
        return blank_verifier<T>::verify(blank, data_type);
    }

    /**
     * @brief Convert string to integer.
     *
     * @param[in]  str String containing integer to be converted.
     * @param[out] end Pointer to first invalid character.
     * @param[out] num Integer converted from string @a str.
     *
     * @see strtoll()
     *
     * @note Result must be verified by checking @c errno.
     * @note This is an implementation detail.  Use @c from_string().
     */
    void
    from_string(char const * str, char ** end, std::intmax_t & num)
    {
        constexpr int base = 10;

        num = std::strtoll(str, end, base);
    }

    /**
     * @brief Convert string to floating point value (@c double).
     *
     * @param[in]  str String containing floating point value to be
     *                 converted.
     * @param[out] end Pointer to first invalid character.
     * @param[out] num Floating point value converted from string
     *                 @a str.
     *
     * @see strtod()
     *
     * @note Result must be verified by checking @c errno.
     * @note This is an implementation detail.  Use @c from_string().
     */
    void
    from_string(char const * str, char ** end, double & num)
    {
        num = std::strtod(str, end);
    }

    /**
     * @brief Convert string to a number.
     *
     * @note This implementation relies on C++ library functions like
     *       @c std::strtoll() and @c std::strtod() rather than
     *       @c std::stoll() and @c std::stod() to avoid having an
     *       exception thrown on conversion error.
     *
     * @tparam     T   Integer or floating point type the string
     *                 to which the string @a str converted.
     * @param[in]  str String containing number to be converted.
     * @param[out] num Number converted from string @a str.
     *
     * @return Number converted from string.
     *
     * @see strtoll()
     * @see strtod()
     */
    template <typename T>
    bool
    from_string(char const * str, T & num)
    {
        static_assert(std::is_arithmetic<T>(),
                      "Type of number converted from a string must "
                      "be an integer or floating point type.");

        // errno will be non-zero on string-to-number conversion
        // failure.
        errno = 0;

        char * end = nullptr;

        from_string(str, &end, num);

        if (errno == ERANGE || (errno != 0 && num == 0)) {
            constexpr std::size_t BUFLEN = 80;
            char buf[BUFLEN] = {};

            char const * const err =
                MaRC::strerror(errno, buf, MaRC::size(buf));

            errno = 0;

            MaRC::error("invalid number ({}): {}", str, err);
            return false;
        }

        if (end == str) {
            MaRC::error("no digits were found in number ({})", str);
            return false;
        }

        return true;
    }
}

std::string map_filename;

std::shared_ptr<MaRC::OblateSpheroid> oblate_spheroid;

std::unique_ptr<MaRC::map_parameters> map_params;

using namespace MaRC;

std::unique_ptr<MapFactory> map_factory;

// CFITSIO's "naxes" parameter is an array of long values.
long map_samples = 0;
long map_lines = 0;

// To create a grid or not to create a grid?  That is the question.
bool create_grid = false;
double lat_interval;
double lon_interval;

std::unique_ptr<MaRC::PhotoImageFactory> photo_factory;
MaRC::MosaicImageFactory::list_type photo_factories;
MaRC::MosaicImage::average_type averaging_type;

std::unique_ptr<MaRC::PhotoImageParameters> photo_parameters;
std::unique_ptr<MaRC::ViewingGeometry> viewing_geometry;

// Map plane image factory.
std::unique_ptr<MaRC::SourceImageFactory> image_factory;

// Map plane image factories.
MaRC::MapCommand::image_factories_type image_factories;

bool north_pole = true;

// Temporary storage for plane number
std::size_t plane_num = 0;

// Number of planes to be expected
std::size_t num_planes = 0;

// Used to ensure num_planes are defined in sequence
std::size_t expected_plane = 1;

MaRC::optional<double> minimum;
MaRC::optional<double> maximum;

double sample_center = not_a_number;
double line_center   = not_a_number;
double lat_at_center = not_a_number;
double lon_at_center = not_a_number;

double km_per_pixel_val   = -1;
double arcsec_per_pix_val = -1;
double focal_length_val   = -1;
double pixel_scale_val    = -1;

bool graphic_lat = false;

// Conformal projection options
double max_lat = not_a_number;

// Simple Cylindrical projection options
double lo_lat = MaRC::default_configuration::latitude_low;
double hi_lat = MaRC::default_configuration::latitude_high;
double lo_lon = MaRC::default_configuration::longitude_low;
double hi_lon = MaRC::default_configuration::longitude_high;

// Orthographic and Perspective projection options

// Sub-observer latitude and longitude.
MaRC::SubObserv sub_observation_data = { 0, 0 };

double position_angle_val = 0;   // Position (north) angle
MaRC::OrthographicCenter ortho_center;

/**
 * @struct free_functor
 *
 * @brief Functor used for calling @c free() on pointers.
 *
 * This functor is used as the @c Deleter template parameter to the
 * type alias of @c std::unique_ptr below used for automatically
 * freeing string values created by the parser.
 */
struct deallocate_with_free
{
    template <typename T>
    void
    operator()(T * p) const
    {
        std::free(const_cast<std::remove_const_t<T> *>(p));
    }
};

/**
 * @typedef auto_free
 *
 * Automatically manage memory allocated by C based allocators, such
 * as @c malloc(), etc.
 */
template <typename T>
using auto_free = std::unique_ptr<T, deallocate_with_free>;

%}

/* BISON Declarations */

%param { void * scanner } { MaRC::ParseParameter & pp }

%locations

%define parse.lac full
%define parse.error verbose

/* Generate reentrant parser */
%define api.pure full


%union
 {
     bool bval;           // For returning boolean values.
     char * sval;         // For returning string values.
     std::intmax_t ival;  // For returning integer values.
     double val;          // For returning floating point values.
     MaRC::sym_entry * tptr;  // For returning symbol-table pointers.
     MaRC::SubObserv sub_observ_data; // Sub-observation point.
     MaRC::SubSolar sub_solar_data;   // Sub-solar point.
     MaRC::Radii radii_data;          // Oblate spheroid radii.
}

%token <sval> NUM
%token <tptr> VAR FNCT
%token <sval> _STRING
%type <val> latitude latitude_sub longitude
%type <val> position_angle range expr
%type <ival> integer size
%type <bval> rotation
%type <sub_observ_data> sub_observ
%type <sub_solar_data> sub_solar
%type <radii_data> radii
%type <val> eq_rad pol_rad flattening
%right '='
%left '-' '+'
%left '*' '/'
%left NEG     // Negation--unary minus
%right '^'    // Exponentiation

// Map Projections
%token _ALBEQV1 "ALBEQV1"
%token _ALBEQV2 "ALBEQV2"
%token _LAMCNF1 "LAMCNF1"
%token _LAMCNF2 "LAMCNF2"
%token _LAMCYLEQ "LAMCYLEQ"
%token _LAMPOLEQ "LAMPOLEQ"
%token _MERCATOR
%token _ORTHO "ORTHO"
%token _P_STEREO "P_STEREO"
%token _PERSPECTIVE "PERSPECTIVE"
%token _SIMPLE_C "SIMPLE_C"
%token _SINUSOID "SINUSOID"
%token _IDENTITY

// Reserved input file keywords
%token _MAP "MAP"
%token AUTHOR ORIGIN XCOMMENT
%token _COMMENT "COMMENT"
%token _DATA_TYPE "DATA_TYPE"
%token DATA_OFFSET DATA_SCALE DATA_BLANK
%token GRID GRID_INTERVAL LAT_GRID_INTERVAL LON_GRID_INTERVAL
%token MAP_TYPE "TYPE"
%token SAMPLES LINES BODY PLANE DATA_MIN DATA_MAX
%token PROGRADE RETROGRADE FLATTENING
%token AVERAGING  WEIGHTED UNWEIGHTED
%token _NONE "NONE"
%token OPTIONS EQ_RAD POL_RAD ROTATION
%token _IMAGE "IMAGE"
%token _PHOTO "PHOTO"
%token _MU "MU"
%token _MU0 "MU0"
%token _PHASE "PHASE"
%token PLANES LO_LAT HI_LAT LO_LON HI_LON
%token LATITUDE LONGITUDE
%token LATITUDE_TYPE CENTRIC GRAPHIC
%token LAT_AT_CENTER LON_AT_CENTER
%token SAMPLE_OA LINE_OA STD_LAT STD_LAT_1 STD_LAT_2 MAX_LAT POLE
%token NIBBLE NIBBLE_LEFT NIBBLE_RIGHT NIBBLE_TOP NIBBLE_BOTTOM
%token INVERT HORIZONTAL VERTICAL BOTH
%token _INTERPOLATE "INTERPOLATE"
%token SAMPLE_CENTER LINE_CENTER
%token FLAT_FIELD MINNAERT AUTO TABLE GEOM_CORRECT TERMINATOR
%token _EMI_ANG_LIMIT "EMI_ANG_LIMIT"
%token SUB_OBSERV_LAT SUB_OBSERV_LON POSITION_ANGLE
%token SUB_SOLAR_LAT SUB_SOLAR_LON RANGE
%token _REMOVE_SKY "REMOVE_SKY"
%token FOCAL_LENGTH PIXEL_SCALE ARCSEC_PER_PIX KM_PER_PIXEL
%token BYTE_DATA "BYTE"
%token SHORT_DATA "SHORT"
%token LONG_DATA "LONG"
%token LONGLONG_DATA "LONGLONG"
%token FLOAT_DATA "FLOAT"
%token DOUBLE_DATA "DOUBLE"
%token CW CCW
%token YES NO

// Unit tokens
%token AU KM

 /**
  * @todo Is there a way to get Bison's verbose error output to print
  *       the actual unmatched text rather than "UNMATCHED" or "text"
  *       as we define below?
  */
%token UNMATCHED "text"

// End of file
%token END 0 "end of file"

// Grammar follows
%%

/* -------------------------- MAP SETUP ----------------------- */
map:    map_parse
        | user_file_parse
;

user_file_parse:
        grid_intervals plane_data_range nibbling {
          // Reset defaults to those chosen by the user.  If none were
          // chosen by the user, the values will remain unchanged.

          pp.lat_interval = lat_interval;
          pp.lon_interval = lon_interval;

          if (!minimum)
              pp.minimum = minimum;

          if (!maximum)
              pp.maximum = maximum;
        }
;

map_parse:
        map_setup
        | map_setup map_parse
;

map_setup:
        map_entry
        author
        origin
        comments
        body
        data_info
        grid
        projection_type
        planes
        samples
        lines
        plane {
            /*
              We only perform this check if the number of planes was
              actually set since it is no longer necessary to set the
              number of planes.  This also means that we no longer need
              this check.  It is only kept around to maintain
              consistent error checking with earlier versions of MaRC.
              Once support for the "PLANES" keyword is removed, this
              check can be removed.
            */
            if (num_planes > 0 && image_factories.size() != num_planes) {
                /**
                 * @todo Call yyerror() here instead, e.g.:
                 *       yyerror(&yylloc, scanner, pp,
                 *               "some error message");
                 */
                MaRC::error("number of planes ({}) does not "
                            "match \"PLANES\" value ({})",
                            image_factories.size(),
                            num_planes);
                YYERROR;
            } else {
                auto command =
                    std::make_unique<MaRC::MapCommand>(
                        std::move(map_filename),
                        map_samples,
                        map_lines,
                        std::move(map_factory),
                        std::move(map_params));

                if (create_grid)
                    command->grid_intervals(lat_interval, lon_interval);

                command->image_factories(std::move(image_factories));

                pp.push_command(std::move(command));
            }
        }
;

map_entry:
        _MAP ':' _STRING  {
            auto_free<char> str($3);
            map_filename = $3;

            map_params = std::make_unique<MaRC::map_parameters>();

            create_grid = false;

            image_factories.clear();

            averaging_type = MaRC::MosaicImage::AVG_WEIGHTED;

            /**
             * @deprecated Remove once deprecated plane number support
             *             is removed.
             */
            num_planes = 0; expected_plane = 1;
        }
;

author:
        %empty
        | AUTHOR ':' _STRING {
            auto_free<char> str($3);
            map_params->author($3);
        }
;

origin:
        %empty
        | ORIGIN ':' _STRING {
            auto_free<char> str($3);
            map_params->origin($3);
        }
;

comments:
        comment
        xcomment
;

comment:
        %empty
        | comment comment_setup
;

comment_setup:
        _COMMENT ':' _STRING
        {
            auto_free<char> str($3); map_params->push_comment($3);
        }
;

xcomment:
        %empty
        | xcomment xcomment_setup
;

xcomment_setup:
        XCOMMENT ':' _STRING
        {
            auto_free<char> str($3); map_params->push_xcomment($3);
        }
;

data_info:
        data_offset
        data_scale
        data_type
        data_blank
;

data_offset:
        %empty
        | DATA_OFFSET ':' expr  { map_params->bzero($3); }
;

data_scale:
        %empty
        | DATA_SCALE ':' expr   { map_params->bscale($3); }
;

data_type:
        %empty
        | _DATA_TYPE ':' BYTE_DATA     { map_params->bitpix(BYTE_IMG);     }
        | _DATA_TYPE ':' SHORT_DATA    { map_params->bitpix(SHORT_IMG);    }
        | _DATA_TYPE ':' LONG_DATA     { map_params->bitpix(LONG_IMG);     }
        | _DATA_TYPE ':' LONGLONG_DATA { map_params->bitpix(LONGLONG_IMG); }
        | _DATA_TYPE ':' FLOAT_DATA    { map_params->bitpix(FLOAT_IMG);    }
        | _DATA_TYPE ':' DOUBLE_DATA   { map_params->bitpix(DOUBLE_IMG);   }
;

data_blank:
        %empty
        | DATA_BLANK ':' integer {
            auto const bitpix = map_params->bitpix();
            if (bitpix < 0) {  // Negative bitpix for floating point.
                throw std::invalid_argument(
                    "\"DATA_BLANK\" keyword not valid with "
                    "floating point types.");
            } else {
                bool verified = false;

                if (bitpix == BYTE_IMG)
                    verified =
                        verify_data_blank<FITS::byte_type>($3, "BYTE");
                else if (bitpix == SHORT_IMG)
                    verified =
                        verify_data_blank<FITS::short_type>($3, "SHORT");
                else if (bitpix == LONG_IMG)
                    verified =
                        verify_data_blank<FITS::long_type>($3, "LONG");
                else if (bitpix == LONGLONG_IMG)
                    verified =
                        verify_data_blank<FITS::longlong_type>($3,
                                                               "LONGLONG");
                else
                    MaRC::warn("DATA_BLANK value specified without "
                               "map data type.");

                if (!verified)
                    YYERROR;

                map_params->blank(
                    static_cast<blank_type::value_type>($3));
            }
        }
;

grid:
        %empty
        | grid_yes_or_no
        | grid_yes_or_no grid_intervals
;

grid_yes_or_no:
        GRID ':' YES   {
          create_grid = true;
          lat_interval = pp.lat_interval;
          lon_interval = pp.lon_interval;
        }
        | GRID ':' NO  { create_grid = false; }
;

grid_intervals:
        grid_interval
        | lat_grid_interval
        | lon_grid_interval
        | lat_grid_interval lon_grid_interval
        | lon_grid_interval lat_grid_interval
;

grid_interval:
        GRID_INTERVAL ':' expr {
            if ($3 <= 0) {
                auto s = fmt::format("Grid interval value ({}) "
                                     "must be greater than zero.",
                                     $3);

                throw std::invalid_argument(s);
            } else {
              lat_interval = $3;
              lon_interval = $3;
            }
        }
;

lat_grid_interval:
        LAT_GRID_INTERVAL ':' expr {
            if ($3 <= 0) {
                auto s = fmt::format("Latitude grid interval value ({}) "
                                     "must be greater than zero.",
                                     $3);

                throw std::invalid_argument(s);
            } else {
                lat_interval = $3;
            }
        }
;

lon_grid_interval:
        LON_GRID_INTERVAL ':' expr {
            if ($3 <= 0) {
                auto s = fmt::format("Longitude grid interval value ({}) "
                                     "must be greater than zero.",
                                     $3);

                throw std::invalid_argument(s);
            } else {
                lon_interval = $3;
            }
        }
;

projection_type:
        /*
     albeqv1
     | albeqv2
     | identity
     | lamcnf1
     | lamcnf2
     | lamcyleq
     | lampoleq
     | perspective
     | sinusoid
        */
        mercator
        | ortho
        | p_stereo
        | simple_c
;

planes: %empty | PLANES ':' size {
          /**
           * @deprecated The "PLANES" keyword is no longer necessary.
           *             The number of planes is set dynamically as
           *             source images are queued.
           *
           * @todo Remove support for the "PLANES" keyword.
           */
          if ($3 > 0) {
              num_planes = static_cast<std::size_t>($3);
              // MaRC::info("specifying the number of map "
              //            "planes is no longer necessary.");
          } else {
              MaRC::error("incorrect number of planes entered: {}", $3);
              YYERROR;
          }
        }
;

samples:
        SAMPLES ':' size        {
          if ($3 > 0)
              map_samples = static_cast<long>($3);
          else {
              MaRC::error("incorrect value for SAMPLES entered: {}", $3);
              YYERROR;
          }
        }
;

lines:  LINES ':' size  {
          if ($3 > 0)
              map_lines = static_cast<long>($3);
          else {
              MaRC::error("incorrect value for LINES entered: {}", $3);
              YYERROR;
          }
        }
;

integer: NUM {
            auto_free<char> str($1);
            if (!from_string($1, $$))
                YYERROR;
        }

size:   integer { $$ = $1; }
;

body:   BODY ':' _STRING
        radii
        rotation {
          auto_free<char> str($3);
          map_params->object($3);

          ($4).validate();

          oblate_spheroid =
              std::make_shared<MaRC::OblateSpheroid>($5,
                                                     ($4).eq_rad,
                                                     ($4).pol_rad);
        }
;

rotation:
        ROTATION ':' PROGRADE     {
          $$ = true;
        }
        | ROTATION ':' RETROGRADE {
          $$ = false;
        }
;

radii:  eq_rad
        pol_rad {
          ($$).eq_rad = $1;
          ($$).pol_rad = $2;
          ($$).flattening = -1;
        }
        | pol_rad
          eq_rad {
          ($$).eq_rad = $2;
          ($$).pol_rad = $1;
          ($$).flattening = -1;
        }
        | eq_rad
          flattening {
          ($$).eq_rad = $1;
          ($$).pol_rad = -1;
          ($$).flattening = $2;
        }
        | flattening
          eq_rad {
          ($$).eq_rad = $2;
          ($$).pol_rad = -1;
          ($$).flattening = $1;
        }
        | pol_rad
          flattening {
          ($$).eq_rad = -1;
          ($$).pol_rad = $1;
          ($$).flattening = $2;
        }
        | flattening
          pol_rad {
          ($$).eq_rad = -1;
          ($$).pol_rad = $2;
          ($$).flattening = $1;
        }
;

eq_rad:
        EQ_RAD ':' expr  { $$ = $3; }
;

pol_rad:
        POL_RAD ':' expr  { $$ = $3; }
;

flattening:
        FLATTENING ':'  expr  { $$ = $3; }
;

plane:  plane_setup
        | plane plane_setup
;

plane_setup:
        plane_size
        plane_data_range
        plane_type      {
            if (minimum)
                image_factory->minimum(*minimum);

            if (maximum)
                image_factory->maximum(*maximum);

            image_factories.push_back(std::move(image_factory));

            photo_factories.clear();
        }
;

plane_size:
        PLANE ':' size  {
            /**
             * @deprecated It is no longer necessary to specify
             *             the plane number.
             *
             * @todo Remove support for specifying the plane
             *       number.
             */
            if (num_planes == 0) {
                yyerror(&yylloc,
                        scanner,
                        pp,
                        "number of planes not entered prior to "
                        "plane definition");
                YYERROR;
            } else {
                MaRC::debug("specifying the map plane number is no "
                            "longer necessary.");
            }

            std::size_t const map_plane = static_cast<std::size_t>($3);

            if (map_plane == expected_plane) {
                if (map_plane <= num_planes) {
                    plane_num = map_plane;
                    ++expected_plane;
                } else {
                    /**
                     * @todo Call yyerror() here instead, e.g.:
                     *       yyerror(&yylloc, pp, "some error message");
                     */
                    MaRC::error("plane number ({}) greater than "
                                "the number of planes ({})",
                                $3, num_planes);
                    YYERROR;
                }
            } else {
                /**
                 * @todo Call yyerror() here instead, e.g.:
                 *       yyerror(&yylloc, pp, "some error message");
                 */
                MaRC::error("expected plane number is {}, "
                            "not {} as entered",
                            expected_plane, $3);
                YYERROR;
            }

            minimum = pp.minimum; maximum = pp.maximum;
        }
        | PLANE ':' { minimum = pp.minimum; maximum = pp.maximum; }
;

plane_data_range:
        %empty
        | DATA_MIN ':' expr { minimum = $3; }
        | DATA_MAX ':' expr { maximum = $3; }
        | DATA_MIN ':' expr
          DATA_MAX ':' expr {
            if ($3 < $6) {
                minimum = $3;
                maximum = $6;
            } else {
                /**
                 * @todo Call yyerror() here instead, e.g.:
                 *       yyerror(&yylloc, pp, "some error message");
                 */
                MaRC::error("minimum data value {} is greater than "
                            "maximum {}",
                            $3, $6);
                YYERROR;
            }
        }
        | DATA_MAX ':' expr
          DATA_MIN ':' expr {
            if ($6 < $3) {
                minimum = $6;
                maximum = $3;
            } else {
                /**
                 * @todo Call yyerror() here instead, e.g.:
                 *       yyerror(&yylloc, pp, "some error message");
                 */
                MaRC::error("minimum data value {} is greater than "
                            "maximum {}",
                            $6, $3);
                YYERROR;
            }
        }
;

plane_type:
        image {
            if (photo_factories.size() == 1) {
                image_factory = std::move(photo_factories.back());
                photo_factories.pop_back();
            } else {
                image_factory =
                    std::make_unique<MaRC::MosaicImageFactory>(
                        std::move(photo_factories),
                        averaging_type);
            }
        }
        | mu
        | mu0
        | phase
        | lat_plane
        | lon_plane
;

/* -------------------- INPUT IMAGE SETUP -------------------- */
image:
        image_setup
        | image image_setup
;

image_setup:
        image_initialize
        nibbling
        inversion
        image_interpolate
        remove_sky
        centers
        optical_axis
        flat_field
        photo_correct
        geom_correct
        emi_ang_limit
        terminator
        sub_observ
        position_angle
        sub_solar
        range
        image_geometry {
          viewing_geometry->sub_observ(($13).lat, ($13).lon);
          viewing_geometry->position_angle($14);
          viewing_geometry->sub_solar(($15).lat, ($15).lon);
          viewing_geometry->range($16);

          if (km_per_pixel_val > 0) {
              viewing_geometry->km_per_pixel(km_per_pixel_val);
              km_per_pixel_val = -1;  // Reset to "bad" value.
	  }

          if (arcsec_per_pix_val > 0) {
              viewing_geometry->arcsec_per_pixel(arcsec_per_pix_val);
              arcsec_per_pix_val = -1;  // Reset to "bad" value.
	  }

          if (focal_length_val > 0) {
              viewing_geometry->focal_length(focal_length_val);
              focal_length_val = -1;  // Reset to "bad" value.
	  }

          if (pixel_scale_val > 0) {
              viewing_geometry->scale(pixel_scale_val);
              pixel_scale_val = -1;  // Reset to "bad" value.
	  }

          if (!std::isnan(sample_center) && !std::isnan(line_center)) {
              viewing_geometry->body_center(sample_center, line_center);

              // Reset to "bad" value.
              sample_center = not_a_number;
              line_center   = not_a_number;
	  }

          if (!std::isnan(lat_at_center) && !std::isnan(lon_at_center)) {
              viewing_geometry->lat_lon_center(lat_at_center,
                                               lon_at_center);

              // Reset to "bad" value.
              lat_at_center = not_a_number;
              lon_at_center = not_a_number;
	  }

          photo_factory->photo_config(std::move(photo_parameters));
          photo_factory->viewing_geometry(std::move(viewing_geometry));

          photo_factories.push_back(std::move(photo_factory));
        }
;

image_initialize:
        // Parse input image filename and initialize PhotoImageFactory
        // object.
        _IMAGE ':' _STRING {
            auto_free<char> str($3);

            photo_parameters =
                std::make_unique<MaRC::PhotoImageParameters>();

            // Set default user configured nibbling values.
            photo_parameters->nibble_left  (pp.nibble_left);
            photo_parameters->nibble_right (pp.nibble_right);
            photo_parameters->nibble_top   (pp.nibble_top);
            photo_parameters->nibble_bottom(pp.nibble_bottom);

            viewing_geometry =
                std::make_unique<MaRC::ViewingGeometry>(oblate_spheroid);

            /**
             * @todo Move this to the end of the image parsing.
             */
            photo_factory =
                std::make_unique<MaRC::PhotoImageFactory>($3);
        }
;

nibbling:
        %empty
        | nibble
        | nibble_samples
        | nibble_lines
        | nibble_samples nibble_lines
        | nibble_lines nibble_samples
;

nibble:
        NIBBLE ':' size {
          if ($3 >= 0) {
              if (photo_parameters) {
                  photo_parameters->nibble($3);
              } else {
                  // Nibble value from user config file.
                  pp.nibble_left   = $3;
                  pp.nibble_right  = $3;
                  pp.nibble_top    = $3;
                  pp.nibble_bottom = $3;
              }
          } else {
              /**
               * @todo Call yyerror() here instead, e.g.:
               *       yyerror(&yylloc, pp, "some error message");
               */
              MaRC::error("incorrect value for NIBBLE entered: {}", $3);
              YYERROR;
          }
        }
;

nibble_samples:
         nibble_left
         | nibble_right
         | nibble_left nibble_right
         | nibble_right nibble_left

;

nibble_lines:
         nibble_top
         | nibble_bottom
         | nibble_top nibble_bottom
         | nibble_bottom nibble_top
;

nibble_left:
        NIBBLE_LEFT ':' size {
          if ($3 >= 0) {
              if (photo_parameters) {
                  photo_parameters->nibble_left($3);
              } else {
                  // Nibble value from user config file.
                  pp.nibble_left = $3;
              }
          } else {
              MaRC::error("incorrect value for NIBBLE_LEFT entered: {}",
                          $3);
              YYERROR;
          }
        }
;

nibble_right:
        NIBBLE_RIGHT ':' size {
          if ($3 >= 0) {
              if (photo_parameters) {
                  photo_parameters->nibble_right($3);
              } else {
                  // Nibble value from user config file.
                  pp.nibble_right = $3;
              }
          } else {
              MaRC::error("incorrect value for NIBBLE_RIGHT entered: {}",
                          $3);
              YYERROR;
          }
        }
;

nibble_top:
        NIBBLE_TOP ':' size {
          if ($3 >= 0) {
              if (photo_parameters) {
                  photo_parameters->nibble_top($3);
              } else {
                  // Nibble value from user config file.
                  pp.nibble_top = $3;
              }
          } else {
              MaRC::error("incorrect value for NIBBLE_TOP entered: {}",
                          $3);
              YYERROR;
          }
        }
;

nibble_bottom:
        NIBBLE_BOTTOM ':' size {
          if ($3 >= 0) {
              if (photo_parameters) {
                  photo_parameters->nibble_bottom($3);
              } else {
                  // Nibble value from user config file.
                  pp.nibble_bottom = $3;
              }
          } else {
              MaRC::error("incorrect value for NIBBLE_BOTTOM entered: {}",
                          $3);
              YYERROR;
          }
        }
;

inversion:
        %empty
        | INVERT ':' VERTICAL   { photo_factory->invert(true,  false); }
        | INVERT ':' HORIZONTAL { photo_factory->invert(false, true);  }
        | INVERT ':' BOTH       { photo_factory->invert(true,  true);  }
;

image_interpolate:
        %empty
        /* If INTERPOLATE is not found, use the program default */
        | _INTERPOLATE ':' YES { photo_factory->interpolate(true);  }
        | _INTERPOLATE ':' NO  { photo_factory->interpolate(false); }
;

remove_sky:
        %empty
        | _REMOVE_SKY ':' YES { photo_parameters->remove_sky(true);  }
        | _REMOVE_SKY ':' NO  { photo_parameters->remove_sky(false); }
;

mu:    _MU ':' sub_observ range {
            // Mu (potentially scaled to increase significant digits)
            image_factory =
                std::make_unique<MaRC::MuImageFactory>(oblate_spheroid,
                                                       ($3).lat,
                                                       ($3).lon,
                                                       $4);
        }
        | _MU ':'
          sub_observ
          range
          sub_solar /* Unused */ {
            // Mu (potentially scaled to increase significant digits)
            image_factory =
                std::make_unique<MaRC::MuImageFactory>(oblate_spheroid,
                                                       ($3).lat,
                                                       ($3).lon,
                                                       $4);

            MaRC::info("sub-solar point is no longer needed for MU planes");
        }
;

mu0:    _MU0 ':' sub_solar {
          // Mu0 (potentially scaled to increase significant digits)
          image_factory =
              std::make_unique<MaRC::Mu0ImageFactory>(oblate_spheroid,
                                                      ($3).lat,
                                                      ($3).lon);
        }
        | _MU0 ':'
        sub_observ      /* Unused */
        range           /* Unused */
        sub_solar       {
          // Mu0 (potentially scaled to increase significant digits)
          image_factory =
              std::make_unique<MaRC::Mu0ImageFactory>(oblate_spheroid,
                                                      ($5).lat,
                                                      ($5).lon);

          MaRC::info("sub-observer point and range are no longer "
                     "needed for MU0 planes");
        }
;

phase:  _PHASE ':' sub_observ range sub_solar {
          // cos(phase angle) (potentially scaled to increase
          //                   significant digits)
          image_factory =
              std::make_unique<MaRC::CosPhaseImageFactory>(
                  oblate_spheroid,
                  ($3).lat,
                  ($3).lon,
                  ($5).lat,
                  ($5).lon,
                  $4);
        }
;

lat_plane: LATITUDE ':' lat_type {
            // Latitudes in degrees (potentially scaled to increase
            //                       significant digits)
            image_factory =
                std::make_unique<MaRC::LatitudeImageFactory>(
                    oblate_spheroid, graphic_lat);
           }
;


lon_plane: LONGITUDE {
            // Longitudes in degrees (potentially scaled to increase
            //                        significant digits)
            image_factory =
                std::make_unique<MaRC::LongitudeImageFactory>();
           }
;

flat_field:
        %empty
        | FLAT_FIELD ':' _STRING {
            auto_free<char> str($3);
            photo_factory->flat_field($3);
        }
;

photo_correct:
        %empty
        | MINNAERT ':' expr { /* Image->setMinnaertExponent($3); */ }
        | MINNAERT ':' AUTO {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_AUTO); */ }
        | MINNAERT ':' TABLE {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_TABLE); */ }
;

geom_correct:
        %empty
        /* Do not perform geometric abberation correction if
           GEOM_CORRECT keyword not present */
        | GEOM_CORRECT ':' YES  {
            // Galileo spacecraft (GLL) geometric correction.
            photo_factory->geometric_correction(true);
          }
        | GEOM_CORRECT ':' NO   {
            photo_factory->geometric_correction(false);
          }
;

emi_ang_limit:
        %empty
        | _EMI_ANG_LIMIT ':' expr { viewing_geometry->emi_ang_limit($3); }
;

terminator:
        %empty
        | TERMINATOR ':' YES { viewing_geometry->use_terminator(true);  }
        | TERMINATOR ':' NO  { viewing_geometry->use_terminator(false); }
;

range:  RANGE ':' expr  { $$ = $3; }
        | RANGE ':' expr KM  { $$ = $3; }
        | RANGE ':' expr AU  {
            // Convert astronomical units to kilometers.
            $$ = $3 * C::astronomical_unit
                / 1000 /* (C::kilo * C::meter) */;
          }
;

image_geometry:
        km_per_pixel
        | arcsec_per_pix
        | lens_geometry
;

arcsec_per_pix:
        ARCSEC_PER_PIX ':' expr {
            if ($3 > 0) {
                arcsec_per_pix_val = $3;
            } else {
                yyerror(&yylloc,
                        scanner,
                        pp,
                        "ARCSEC_PER_PIX must be greater than zero.");
                YYERROR;
            }
        }
;

lens_geometry:
        focal_length
        pixel_scale
        | pixel_scale
          focal_length
;

focal_length:
        FOCAL_LENGTH ':' expr  { focal_length_val = $3; }
;

pixel_scale:
        PIXEL_SCALE ':' expr  { pixel_scale_val = $3; }
;

/* ------ Alber's Equivalent Projection Setup - One Standard Parallel ------ */
/*
albeqv1:
        albeqv1_type
        albeqv1_options
;

albeqv1_type:
        MAP_TYPE ':' _ALBEQV1  {
          MapEntry->setProjection(MapInfo::ALBEQV1); }
;

albeqv1_options:
        |
        options
        pole
        one_std_lat
;
*/

/* ------ Alber's Equivalent Projection Setup - Two Standard Parallels ----- */
/*
albeqv2:
        albeqv2_type
        albeqv2_options
;

albeqv2_type:
        MAP_TYPE ':' _ALBEQV2  {
          MapEntry->setProjection(MapInfo::ALBEQV2); }
;

albeqv2_options:
        %empty
        | options
          pole
          two_std_lats
;
*/

/* --------------------- Identity Projection Setup ------------------------ */
/*
identity:
        identity_type
;

identity_type:
        MAP_TYPE ':' _IDENTITY  {
          MapEntry->setProjection(MapInfo::IDENTITY); }
;
*/

/* ---- Lambert's Normal Conformal Projection - One Standard Parallel ---- */
/*
lamcnf1:
        lamcnf1_type
        lamcnf1_options
;

lamcnf1_type:
        MAP_TYPE ':' _LAMCNF1  {
            MapEntry->setProjection(MapInfo::LAMCNF1); }
;

lamcnf1_options:
        %empty
        | options
          pole
          one_std_lat
          max_latitude
;
*/

/* ---- Lambert's Normal Conformal Projection - Two Standard Parallels ---- */
/*
lamcnf2:
        lamcnf2_type
        lamcnf2_options
;

lamcnf2_type:
        MAP_TYPE ':' _LAMCNF2  {
            MapEntry->setProjection(MapInfo::LAMCNF2); }
;

lamcnf2_options:
        %empty
        | options
          pole
          two_std_lats
          max_latitude
;
*/

/* ----------- Lambert's Cylindrical Equivalent Projection -------------- */
/*
lamcyleq:
        MAP_TYPE ':' _LAMCYLEQ  {
            MapEntry->setProjection(MapInfo::LAMCYLEQ); }
;
*/

/* --------- Lambert's Polar Azimuthal Equivalent Projection ------------- */
/*
lampoleq:
        lampoleq_type
        lampoleq_options
;

lampoleq_type:
        MAP_TYPE ':' _LAMPOLEQ  {
            MapEntry->setProjection(MapInfo::LAMPOLEQ); }
;

lampoleq_options:
        %empty
        | options
          pole
;
*/

/* ------------------------- Mercator Projection ------------------------ */
mercator:
        MAP_TYPE ':' _MERCATOR
        mercator_options {
            map_factory =
                std::make_unique<MaRC::Mercator>(oblate_spheroid);
        }
;

mercator_options:
        %empty
        | options
;

/* ----------------------- Orthographic Projection ------------------------- */
ortho:  MAP_TYPE ':' _ORTHO
        ortho_options {
            map_factory =
                std::make_unique<MaRC::Orthographic>(
                    oblate_spheroid,
                    sub_observation_data.lat,
                    sub_observation_data.lon,
                    position_angle_val,
                    km_per_pixel_val,
                    ortho_center);

            // Reset options
            sub_observation_data.lat = 0;
            sub_observation_data.lon = 0;
            km_per_pixel_val = -1;
            position_angle_val = 0;
            ortho_center.geometry = MaRC::DEFAULT;
        }
;

ortho_options:
        %empty
        | options ortho_optsub
;

ortho_optsub:
        %empty
        | sub_observ {
          sub_observation_data.lat = ($1).lat;
          sub_observation_data.lon = ($1).lon;
        }
        | position_angle { position_angle_val = $1; }
        | km_per_pixel
        | centers {
            if (!std::isnan(sample_center) && !std::isnan(line_center)) {
                ortho_center.geometry = MaRC::CENTER_GIVEN;
                ortho_center.sample_lat_center = sample_center;
                ortho_center.line_lon_center   = line_center;

                // Reset to "bad" value.
                sample_center = not_a_number;
                line_center   = not_a_number;
            } else if (!std::isnan(lat_at_center)
                       && !std::isnan(lon_at_center)) {
                ortho_center.geometry = MaRC::LAT_LON_GIVEN;
                ortho_center.sample_lat_center = lat_at_center;
                ortho_center.line_lon_center   = lon_at_center;

                // Reset to "bad" value.
                lat_at_center = not_a_number;
                lon_at_center = not_a_number;
            }
        }
        | sub_observ position_angle {
          sub_observation_data.lat = ($1).lat;
          sub_observation_data.lon = ($1).lon;
            position_angle_val = $2;
        }
        | sub_observ position_angle km_per_pixel {
          sub_observation_data.lat = ($1).lat;
          sub_observation_data.lon = ($1).lon;
            position_angle_val = $2;
        }
        | sub_observ position_angle km_per_pixel centers {
          sub_observation_data.lat = ($1).lat;
          sub_observation_data.lon = ($1).lon;
            position_angle_val = $2;
            if (!std::isnan(sample_center) && !std::isnan(line_center)) {
                ortho_center.geometry = MaRC::CENTER_GIVEN;
                ortho_center.sample_lat_center = sample_center;
                ortho_center.line_lon_center   = line_center;

                // Reset to "bad" value.
                sample_center = not_a_number;
                line_center   = not_a_number;
            } else if (!std::isnan(lat_at_center)
                       && !std::isnan(lon_at_center)) {
                ortho_center.geometry = MaRC::LAT_LON_GIVEN;
                ortho_center.sample_lat_center = lat_at_center;
                ortho_center.line_lon_center   = lon_at_center;

                // Reset to "bad" value.
                lat_at_center = not_a_number;
                lon_at_center = not_a_number;
            }
        }
;

/* ------------------------------------------------------------------------ */

sub_observ:
        SUB_OBSERV_LAT ':' latitude
        SUB_OBSERV_LON ':' longitude  {
          $$.lat = $3;
          $$.lon = $6;
        }
        | SUB_OBSERV_LON ':' longitude
          SUB_OBSERV_LAT ':' latitude   {
            $$.lat = $6;
            $$.lon = $3;
        }
;

sub_solar:
        SUB_SOLAR_LAT ':' latitude
        SUB_SOLAR_LON ':' longitude  { ($$).lat = $3; ($$).lon = $6; }
        | SUB_SOLAR_LON ':' longitude
          SUB_SOLAR_LAT ':' latitude   { ($$).lat = $6; ($$).lon = $3; }
;

position_angle:
        POSITION_ANGLE ':' expr    {
            if (std::abs($3) > 360) {
                MaRC::error("incorrect position (North) "
                            "angle entered: {}",
                            $3);
                YYERROR;
            }

            $$ = circle_angle($3);
        }
        | POSITION_ANGLE ':' expr CW   {
            if (std::abs($3) > 360) {
                MaRC::error("incorrect position (North) "
                            "angle entered: {} CW",
                            $3);
                YYERROR;
            }

            $$ = 360 - circle_angle($3);
        }
        | POSITION_ANGLE ':' expr CCW    {
            if (std::abs($3) > 360) {
                MaRC::error("incorrect position (North) "
                            "angle entered: {} CCW",
                            $3);
                YYERROR;
            }

            $$ = circle_angle($3);
        }
;

km_per_pixel:
        KM_PER_PIXEL ':' expr  {
            if ($3 > 0) {
                km_per_pixel_val = $3;
            } else {
                yyerror(&yylloc,
                        scanner,
                        pp,
                        "KM_PER_PIXEL must be greater than zero.");
                YYERROR;
            }
        }
;

centers:
        center_given
        | lat_lon_given
;

center_given:
        SAMPLE_CENTER ':' expr
        LINE_CENTER ':' expr  {
          sample_center = $3;
          line_center   = $6;
        }
        | LINE_CENTER ':' expr
          SAMPLE_CENTER ':' expr  {
            sample_center = $6;
            line_center   = $3;
        }
;

lat_lon_given:
        LAT_AT_CENTER ':' latitude
        LON_AT_CENTER ':' longitude  {
//           if (in_ortho_options)
//             MapEntry->setOrthoCenter ($3, $6, LAT_LON_GIVEN);
//           else if (in_perspective_options)
//             MapEntry->setPerspectiveCenter ($3, $6, LAT_LON_GIVEN);
//           else
//             Image->setLatLonCenter ($3, $6);

             lat_at_center = $3;
             lon_at_center = $6;
        }
        | LON_AT_CENTER ':' longitude
          LAT_AT_CENTER ':' latitude  {
//             if (in_ortho_options)
//               MapEntry->setOrthoCenter ($6, $3, LAT_LON_GIVEN);
//             else if (in_perspective_options)
//               MapEntry->setPerspectiveCenter ($6, $3, LAT_LON_GIVEN);
//             else
//               Image->setLatLonCenter ($6, $3);

             lat_at_center = $6;
             lon_at_center = $3;
        }
;

optical_axis:
        %empty
        | SAMPLE_OA ':' expr
          LINE_OA   ':' expr  { viewing_geometry->optical_axis($3, $6); }
        | LINE_OA   ':' expr
          SAMPLE_OA ':' expr  { viewing_geometry->optical_axis($6, $3); }
;

/* ----------------------- Perspective Projection ------------------------- */
/*
perspective:
        perspective_type
        perspective_options { in_perspective_options = false; }
;

perspective_type:
        MAP_TYPE ':' _PERSPECTIVE  {
          MapEntry->setProjection(MapInfo::PERSPECTIVE); }
;

perspective_options:
        %empty
        | options { in_perspective_options = true; }
          perspective_optsub
;

perspective_optsub:
        %empty
        | centers
        | sub_observ {}
        | position_angle {}
        | range {}
        | image_geometry
        | centers sub_observ
        | centers sub_observ position_angle
        | centers sub_observ position_angle range
        | centers sub_observ position_angle range image_geometry
;
*/

/* ------------------- Polar Stereographic Projection --------------------- */
p_stereo:
        MAP_TYPE ':' _P_STEREO
        p_stereo_options {
            map_factory =
                std::make_unique<MaRC::PolarStereographic>(
                    oblate_spheroid,
                    max_lat,
                    north_pole);

            // Reset options
            max_lat = not_a_number;
            north_pole = true;
        }
;

p_stereo_options:
        %empty
        | options pole max_latitude
;

/* -------------- Simple Cylindrical (Rectangular ?) Projection ------------ */
simple_c:
        MAP_TYPE ':' _SIMPLE_C
        simple_c_options {
            map_factory =
                std::make_unique<MaRC::SimpleCylindrical>(
                    oblate_spheroid,
                    lo_lat,
                    hi_lat,
                    lo_lon,
                    hi_lon,
                    graphic_lat);

            // Reset options
            lo_lat = -90;
            hi_lat = 90;
            lo_lon = 0;
            hi_lon = 360;
            graphic_lat = false;
        }
;

simple_c_options:
        %empty
        | options lat_type simple_c_latlonrange
;

lat_type:
        %empty
        | LATITUDE_TYPE ':' CENTRIC { graphic_lat = false; }
        | LATITUDE_TYPE ':' GRAPHIC { graphic_lat = true;  }
        /**
         * @deprecated The following are only for backward
         *             compatibility.
         */
        | CENTRIC   { graphic_lat = false; }
        | GRAPHIC   { graphic_lat = true;  }
;

simple_c_latlonrange:
        %empty
        | lat_range
        | lon_range
        | lat_range lon_range
        | lon_range lat_range
;

lat_range:
        LO_LAT ':' latitude
        HI_LAT ':' latitude  {
          if ($3 < $6) {
              lo_lat = $3;
              hi_lat = $6;
          } else  {
              yyerror(&yylloc,
                      scanner,
                      pp,
                      "LO_LAT is greater than or equal to HI_LAT.");
              YYERROR;
          }
        }
        | HI_LAT ':' latitude
          LO_LAT ':' latitude  {
          if ($6 < $3) {
              lo_lat = $6;
              hi_lat = $3;
          } else {
              yyerror(&yylloc,
                      scanner,
                      pp,
                      "LO_LAT is greater than or equal to HI_LAT.");
              YYERROR;
          }
        }
;

lon_range:
        LO_LON ':' longitude
        HI_LON ':' longitude  {
          lo_lon = $3;
          hi_lon = $6;
        }
        | HI_LON ':' longitude
          LO_LON ':' longitude  {
          lo_lon = $6;
          hi_lon = $3;
        }
;

/* --------- Sanson-Flamsteed (Sinusoidal Equivalent) Projection ---------- */
/*
sinusoid:
        sinusoid_type
        sinusoid_options
;

sinusoid_type:
        MAP_TYPE ':' _SINUSOID  { MapEntry->setProjection(MapInfo::SINUSOID); }
;

sinusoid_options:
        %empty
        | options
;
*/

/* ----------------------- Options entry setup ---------------------------- */
options:
        options_keyword
        options_common
;

options_keyword:
        OPTIONS ':'
;

options_common:
        averaging
;

averaging:
        %empty
        | AVERAGING ':' UNWEIGHTED {
              averaging_type = MaRC::MosaicImage::AVG_UNWEIGHTED; }
        | AVERAGING ':' WEIGHTED {
              averaging_type = MaRC::MosaicImage::AVG_WEIGHTED; }
        | AVERAGING ':' _NONE {
              averaging_type = MaRC::MosaicImage::AVG_NONE; }
;

/* ----------------------- General Subroutines ---------------------------- */
/*
one_std_lat:
        %empty
        | STD_LAT ':' latitude {
          if ((north_pole && $3 > 0 && $3 < 90) ||
              (!north_pole && $3 < 0 && $3 > -90)) {
            MapEntry->setStdLat($3);
          }
          else {
              MaRC::error("Standard latitude must be greater than zero "
                          "north polar projections and negative for "
                          "south polar projections.  Absolute value of "
                          "standard latitude must also be less than 90 "
                          "degrees.  STD_LAT: {}", $3);
              YYERROR;
          }
        }
;

two_std_lats:
        %empty
        | STD_LAT_1 ':' latitude
          STD_LAT_2 ':' latitude {
            if (std::abs($6) < std::abs($3) &&
                (north_pole && $3 < 90 && $3 > 0) ||
                (!north_pole && $3 > -90 && $3 < 0)) {
              MapEntry->setStdLat($3, $6);
            }
            else {
                MaRC::error("Absolute value of first standard latitude "
                            "must be greater than absolute value of "
                            "second standard latitude.  First standard "
                            "latitude must also be greater than zero for "
                            "north polar projections and negative for "
                            "south polar projections.  The absolute "
                            "value of either standard latitude cannot "
                            "be zero or 90 degrees.  STD_LAT_1: {}"
                            "   STD_LAT_2: {}", $3, $6);
                YYERROR;
            }
        }
;
*/

max_latitude:
        %empty
        | MAX_LAT ':' latitude { max_lat = $3; }
;

pole:   POLE ':' 'N' { north_pole = true; }
        | POLE ':' 'S' { north_pole = false; }
;

latitude:
        latitude_sub            { $$ = $1; }
        | latitude_sub 'C'      { $$ = $1; }
        | latitude_sub 'G'      {
            /* Convert to CENTRIC latitude. */
            $$ = oblate_spheroid->centric_latitude($1 * C::degree)
                   / C::degree; }
;

latitude_sub:
        expr    {
            if (std::abs($1) <= 90)
                $$ = $1;
            else {
                MaRC::error("incorrect latitude entered: {}", $1);
                YYERROR;
            }
        }
        | expr 'N' {
            if ($1 >= 0 && $1 <= 90)
                $$ = $1;
            else {
                MaRC::error("incorrect latitude entered: {} N", $1);
                YYERROR;
            }
        }
        | expr 'S' {
            if ($1 >= 0 && $1 <= 90)
                $$ = -$1;
            else {
                MaRC::error("incorrect latitude entered: {} S", $1);
                YYERROR;
            }
        }
;

longitude:
         expr {
             if (std::abs($1) > 360) {
                  MaRC::error("incorrect longitude entered: {}", $1);
                  YYERROR;
             }

             $$ = circle_angle($1);
         }
         | expr 'E' {
             if (std::abs($1) > 360) {
                  MaRC::error("incorrect longitude entered: {} E", $1);
                  YYERROR;
             }

             $$ = circle_angle($1);

             if (oblate_spheroid->prograde())
                 $$ = 360 - $$;  // Convert to West longitude.
         }
         | expr 'W' {
             if (std::abs($1) > 360) {
                  MaRC::error("incorrect longitude entered: {} W", $1);
                  YYERROR;
             }

             $$ = circle_angle($1);

             if (!oblate_spheroid->prograde())
                 $$ = 360 - $$;  // Convert to East longitude.
         }
;

/* --------------- Multifunction Infix Notation Calculator ----------------- */
/* All numbers will be handled with double precision here. */
expr:   NUM {
            auto_free<char> str($1);
            if (!from_string($1, $$))
                YYERROR;
        }
        | VAR                   { $$ = $1->value.var;              }
        | VAR '=' expr          { $$ = $3; $1->value.var = $3;     }
        | FNCT '(' expr ')'     { $$ = (*($1->value.fnctptr))($3); }
        | expr '+' expr         { $$ = $1 + $3;                    }
        | expr '-' expr         { $$ = $1 - $3;                    }
        | expr '*' expr         { $$ = $1 * $3;                    }
        | expr '/' expr         {
            if ($3 != 0)
                $$ = $1 / $3;
            else {
                yyerror(&yylloc,
                        scanner,
                        pp,
                        "division by zero");
                YYERROR;
            }
        }
        | '-' expr  %prec NEG   { $$ = -$2;                        }
        | expr '^' expr         { $$ = std::pow($1, $3);           }
        | '(' expr ')'          { $$ = $2;                         }
;
/* ------------------------------------------------------------------------- */

%%
