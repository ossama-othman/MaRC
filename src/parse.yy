/*  -*- C++ -*-

*/

/*
Parser for MaRC input files
Requires GNU Bison 1.35 or greater
*/

%{
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

  using namespace MaRC;

  std::unique_ptr<MapFactory<FITS::byte_type>>   map_factory_byte;
  std::unique_ptr<MapFactory<FITS::short_type>>  map_factory_short;
  std::unique_ptr<MapFactory<FITS::long_type>>   map_factory_long;
  std::unique_ptr<MapFactory<FITS::float_type>>  map_factory_float;
  std::unique_ptr<MapFactory<FITS::double_type>> map_factory_double;

  // CFITSIO's "naxes" parameter is an array of long values.
  long map_samples = 0;
  long map_lines = 0;

  bool  transform_data = false;
  float fits_bzero  = 0;
  float fits_bscale = 1;

  bool blank_set = false;
  int  fits_blank  = 0;

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

%}

/* BISON Declarations */

%parse-param {MaRC::ParseParameter & pp}
%lex-param {MaRC::ParseParameter & pp}
%locations
%error-verbose

/* Generate reentrant parser */
%pure-parser


%union
{
  char * sval;
  double val;        // For returning numbers
  bool bval;         // For returning boolean values
  MaRC::sym_entry * tptr;  // For returning symbol-table pointers
  MaRC::SubObserv sub_observ_data; // Sub-observation point.
  MaRC::SubSolar sub_solar_data;   // Sub-solar point.
  MaRC::Radii radii_data;          // Oblate spheroid radii.
}

%token <val> NUM
%token <tptr> VAR FNCT
%token <sval> _STRING
%type <val> latitude latitude_sub longitude
%type <val> position_angle range size expr
%type <bval> rotation
%type <sub_observ_data> sub_observ
%type <sub_solar_data> sub_solar
%type <radii_data> radii
%type <val> eq_rad
%type <val> pol_rad
%type <val> flattening
%right '='
%left '-' '+'
%left '*' '/'
%left NEG     // Negation--unary minus
%right '^'    // Exponentiation

// Map Projections
%token _ALBEQV1 _ALBEQV2 _LAMCNF1 _LAMCNF2 _LAMCYLEQ _LAMPOLEQ
%token _MERCATOR _ORTHO _P_STEREO _PERSPECTIVE _SIMPLE_C _SINUSOID
%token _IDENTITY

// Reserved input file keywords
%token _MAP AUTHOR ORIGIN _COMMENT XCOMMENT
%token _DATA_TYPE DATA_OFFSET DATA_SCALE DATA_BLANK
%token GRID GRID_INTERVAL LAT_GRID_INTERVAL LON_GRID_INTERVAL
%token MAP_TYPE SAMPLES LINES BODY PLANE DATA_MIN DATA_MAX
%token PROGRADE RETROGRADE FLATTENING
%token AVERAGING NONE WEIGHTED UNWEIGHTED
%token OPTIONS EQ_RAD POL_RAD ROTATION _IMAGE _PHOTO _MU _MU0 _PHASE
%token PLANES LO_LAT HI_LAT LO_LON HI_LON
%token LATITUDE LONGITUDE
%token LATITUDE_TYPE CENTRIC GRAPHIC
%token LAT_AT_CENTER LON_AT_CENTER
%token SAMPLE_OA LINE_OA STD_LAT STD_LAT_1 STD_LAT_2 MAX_LAT POLE
%token NIBBLE NIBBLE_LEFT NIBBLE_RIGHT NIBBLE_TOP NIBBLE_BOTTOM
%token INVERT HORIZONTAL VERTICAL BOTH _INTERPOLATE SAMPLE_CENTER LINE_CENTER
%token FLAT_FIELD MINNAERT AUTO TABLE GEOM_CORRECT _EMI_ANG_LIMIT TERMINATOR
%token SUB_OBSERV_LAT SUB_OBSERV_LON POSITION_ANGLE
%token SUB_SOLAR_LAT SUB_SOLAR_LON RANGE _REMOVE_SKY
%token FOCAL_LENGTH PIXEL_SCALE ARCSEC_PER_PIX KM_PER_PIXEL
%token BYTE_DATA SHORT_DATA LONG_DATA FLOAT_DATA DOUBLE_DATA
%token CW CCW
%token YES NO UNMATCHED

// Unit tokens
%token AU KM

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

          pp.lat_interval  = lat_interval;
          pp.lon_interval  = lon_interval;

          if (!std::isnan (minimum))
              pp.minimum = minimum;

          if (!std::isnan (maximum))
              pp.maximum = maximum;

          pp.nibble_left   = nibble_left_val;
          pp.nibble_right  = nibble_right_val;
          pp.nibble_top    = nibble_top_val;
          pp.nibble_bottom = nibble_bottom_val;
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
              std::unique_ptr<MaRC::MapCommand> map_command;

              switch (map_data_type) {
              case BYTE:
                  map_command =
                      std::make_unique<MaRC::MapCommand_T<FITS::byte_type>>(
                          map_filename,
                          body_name,
                          std::move(map_factory_byte),
                          map_samples,
                          map_lines);
                  break;

              case SHORT:
                  map_command =
                      std::make_unique<MaRC::MapCommand_T<FITS::short_type>>(
                          map_filename,
                          body_name,
                          std::move(map_factory_short),
                          map_samples,
                          map_lines);
              break;

              case LONG:
                  map_command =
                      std::make_unique<MaRC::MapCommand_T<FITS::long_type>>(
                          map_filename,
                          body_name,
                          std::move(map_factory_long),
                          map_samples,
                          map_lines);
                  break;

              case FLOAT:
                  map_command =
                      std::make_unique<MaRC::MapCommand_T<FITS::float_type>>(
                          map_filename,
                          body_name,
                          std::move(map_factory_float),
                          map_samples,
                          map_lines);
                  break;

              case DOUBLE:
                  map_command =
                      std::make_unique<MaRC::MapCommand_T<FITS::double_type>>(
                          map_filename,
                          body_name,
                          std::move(map_factory_double),
                          map_samples,
                          map_lines);
                  break;

              default:
                  throw std::invalid_argument("Unrecognized map data type");
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

              pp.push_command(std::move(map_command));
            }
        }
;

map_entry:
        _MAP ':' _STRING  {
          map_filename = $3;
          free ($3);

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
;

author:
        | AUTHOR ':' _STRING { map_author = $3; free ($3); }
;

origin:
        | ORIGIN ':' _STRING { map_origin = $3; free ($3); }
;

comments:
        comment
        xcomment
;

comment:
        | comment_setup comment
;

comment_setup:
        _COMMENT ':' _STRING    { comment_list.push_back ($3); free ($3); }
;

xcomment:
        | xcomment_setup xcomment
;

xcomment_setup:
        XCOMMENT ':' _STRING    { xcomment_list.push_back ($3); free ($3); }
;


data_info:
        data_offset
        data_scale
        data_type
        data_blank
;

data_offset:
        | DATA_OFFSET ':' size  {
            fits_bzero = static_cast<FITS::float_type> ($3);
            transform_data = true;
        }
;

data_scale:
        | DATA_SCALE ':' size   {
            fits_bscale = static_cast<FITS::float_type> ($3);
            transform_data = true;
        }
;

data_type:
        _DATA_TYPE ':' BYTE_DATA         { map_data_type = BYTE;   }
        | _DATA_TYPE ':' SHORT_DATA      { map_data_type = SHORT;  }
        | _DATA_TYPE ':' LONG_DATA       { map_data_type = LONG;   }
        | _DATA_TYPE ':' FLOAT_DATA      { map_data_type = FLOAT;  }
        | _DATA_TYPE ':' DOUBLE_DATA     { map_data_type = DOUBLE; }
;

data_blank:
        | DATA_BLANK ':' expr {
          if (map_data_type == FLOAT || map_data_type == DOUBLE)
            throw std::invalid_argument ("\"BLANK\" keyword not valid with "
                                         "floating point types.");
          else
            {
              fits_blank = static_cast<int> ($3);
              blank_set = true;
            }
          ;
        }
;

grid:
        grid_yes_or_no
        grid_intervals
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
        | grid_interval
        | lat_grid_interval
        | lon_grid_interval
        | lat_grid_interval lon_grid_interval
        | lon_grid_interval lat_grid_interval
;

grid_interval:
        GRID_INTERVAL ':' size {
          if ($3 <= 0)
            {
              std::ostringstream s;
              s << "Grid interval value (" << $3 << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            {
              lat_interval = $3;
              lon_interval = $3;
            }
        }
;

lat_grid_interval:
        LAT_GRID_INTERVAL ':' size {
          if ($3 <= 0)
            {
              std::ostringstream s;
              s << "Latitude grid interval value (" << $3 << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            lat_interval = $3;
        }
;

lon_grid_interval:
        LON_GRID_INTERVAL ':' size {
          if ($3 <= 0)
            {
              std::ostringstream s;
              s << "Longitude grid interval value (" << $3 << ") "
                << "less than or equal to zero";

              throw std::out_of_range (s.str ());
            }
          else
            lon_interval = $3;
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

planes: | PLANES ':' size         {
          /**
           * @deprecated The "PLANES" keyword is no longer necessary.
           *
           * @todo Remove support for the "PLANES" keyword.
           */

          // We really don't need to ask the user to set the number of
          // planes or the plane number any more.  The number of
          // planes is set dynamically as source images are queued.
          if ($3 > 0)
            num_planes = static_cast<unsigned int> ($3);
          else
            std::cerr << "Incorrect number of planes entered: "
                      << $3 << std::endl;
        }
;

samples:
        SAMPLES ':' size        {
          if ($3 > 0)
            map_samples = static_cast<long>($3);
          else
            {
              std::cerr << "Incorrect value for SAMPLES entered: "
                        << $3 << std::endl;
            }
        }
;

lines:  LINES ':' size  {
          if ($3 > 0)
            map_lines = static_cast<long>($3);
          else
            {
              std::cerr << "Incorrect value for LINES entered: " << $3
                        << std::endl;
            }
        }
;

size:   expr { $$ = $1; }
;

body:   BODY ':' _STRING
        radii
        rotation {
          body_name = $3;
          free ($3);

          oblate_spheroid =
            MaRC::ValuePtr<MaRC::OblateSpheroid> (
              new MaRC::OblateSpheroid ($5,
                                        ($4).eq_rad,
                                        ($4).pol_rad,
                                        ($4).flattening));
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
        | plane_setup plane
;

plane_setup:
        plane_size
        plane_data_range
        plane_type      {
          image_factory->minimum (minimum);
          image_factory->maximum (maximum);

          image_factories.push_back (image_factory);

          photo_factories.clear ();
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
          if (num_planes == 0)
            {
              std::cerr << "Number of planes not entered prior to plane "
                        << "definition." << std::endl;
            }

          const unsigned int map_plane = static_cast<unsigned int> ($3);

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
                    << "You entered:  " << $3 << std::endl;
                }
            }
          else
            {
              std::cerr << "Incorrect plane number entered." << std::endl
                        << "Expected plane number is: " << expected_plane
                        << std::endl
                        << "You entered:  " << $3 << std::endl;
            }

          minimum = pp.minimum; maximum = pp.maximum;
        }
        | PLANE ':' { minimum = pp.minimum; maximum = pp.maximum; }
;

plane_data_range:
        | DATA_MIN ':' size { minimum = $3; }
        | DATA_MAX ':' size { maximum = $3; }
        | DATA_MIN ':' size
          DATA_MAX ':' size {
            if ($3 < $6)
              {
                minimum = $3;
                maximum = $6;
              }
            else
              {
                std::cerr << "Minimum data value: " << $3
                          << " is greater than"
                          << std::endl
                          << "maximum data value: " << $6 << std::endl;
              }
        }
        | DATA_MAX ':' size
          DATA_MIN ':' size {
            if ($6 < $3)
              {
                minimum = $6;
                maximum = $3;
              }
            else
              {
                std::cerr << "Minimum data value: " << $6
                          << " is greater than"
                          << std::endl
                          << "maximum data value: " << $3
                          << std::endl;
              }
        }
;

plane_type:
        image
        | mu
        | mu0
        | phase
        | lat_plane
        | lon_plane
;

/* -------------------- INPUT IMAGE SETUP -------------------- */
image:
        image_setup    {
          image_factory = photo_factory;
        }
        | image_setup image {
            image_factory =
              MaRC::ValuePtr<MaRC::MosaicImageFactory> (
                new MaRC::MosaicImageFactory (photo_factories,
                                              averaging_type));
        }
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

          if (!std::isnan (sample_center) && !std::isnan (line_center))
            {
              photo_factory->body_center (sample_center, line_center);
              sample_center = MaRC::NaN;  // Reset to "bad" value.
              line_center = MaRC::NaN;  // Reset to "bad" value.
            }

          if (!std::isnan (lat_at_center) && !std::isnan (lon_at_center))
            {
              photo_factory->lat_lon_center (lat_at_center, lon_at_center);
              lat_at_center = MaRC::NaN;  // Reset to "bad" value.
              lon_at_center = MaRC::NaN;  // Reset to "bad" value.
            }

          photo_factory->sub_observ (($13).lat, ($13).lon);
          photo_factory->position_angle ($14);
          photo_factory->sub_solar (($15).lat, ($15).lon);
          photo_factory->range ($16);

          photo_factories.push_back (*photo_factory);
        }
;

image_initialize:
        /* Parse input image filename and initialize SourceImage object */
        _IMAGE ':' _STRING {
          photo_factory =
            MaRC::ValuePtr<MaRC::PhotoImageFactory> (
              new MaRC::PhotoImageFactory ($3,
                                           *oblate_spheroid));
            free ($3);
        }
;

nibbling:
        | nibble
        | nibble_samples
        | nibble_lines
        | nibble_samples nibble_lines
        | nibble_lines nibble_samples
;

nibble:
        NIBBLE ':' size {
          if ($3 >= 0)
            {
              nibble_left_val   = static_cast<unsigned int> ($3);
              nibble_right_val  = static_cast<unsigned int> ($3);
              nibble_top_val    = static_cast<unsigned int> ($3);
              nibble_bottom_val = static_cast<unsigned int> ($3);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE entered: "
                        << $3 << std::endl;
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
          if ($3 >= 0)
            {
              nibble_left_val = static_cast<unsigned int> ($3);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_LEFT entered: "
                        << $3 << std::endl;
            }
        }
;

nibble_right:
        NIBBLE_RIGHT ':' size {
          if ($3 >= 0)
            {
              nibble_right_val = static_cast<unsigned int> ($3);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_RIGHT entered: "
                        << $3 << std::endl;
            }
        }
;

nibble_top:
        NIBBLE_TOP ':' size {
          if ($3 >= 0)
            {
              nibble_top_val = static_cast<unsigned int> ($3);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_TOP entered: "
                        << $3 << std::endl;
            }
        }
;

nibble_bottom:
        NIBBLE_BOTTOM ':' size {
          if ($3 >= 0)
            {
              nibble_bottom_val = static_cast<unsigned int> ($3);
            }
          else
            {
              std::cerr << "Incorrect value for NIBBLE_BOTTOM entered: "
                        << $3 << std::endl;
            }
        }
;

inversion:
        | INVERT ':' VERTICAL   { photo_factory->invert (true,  false); }
        | INVERT ':' HORIZONTAL { photo_factory->invert (false, true);  }
        | INVERT ':' BOTH       { photo_factory->invert (true,  true);  }
;

image_interpolate:
        /* If INTERPOLATE is not found, use the program default */
        | _INTERPOLATE ':' YES   { photo_factory->interpolate (true);  }
        | _INTERPOLATE ':' NO    { photo_factory->interpolate (false); }
;

remove_sky:
        | _REMOVE_SKY ':' YES { photo_factory->remove_sky (true);  }
        | _REMOVE_SKY ':' NO  { photo_factory->remove_sky (false); }
;

mu:     _MU ':'
        sub_observ
        range
        sub_solar       {
            // Mu * 10000
            image_factory =
              MaRC::ValuePtr<MaRC::MuImageFactory> (
                new MaRC::MuImageFactory (*oblate_spheroid,
                                          ($3).lat,
                                          ($3).lon,
                                          $4));
        }
;

mu0:    _MU0 ':'
        sub_observ      /* Unused */
        range           /* Unused */
        sub_solar       {
          // Mu0 * 10000
          image_factory =
            MaRC::ValuePtr<MaRC::Mu0ImageFactory> (
              new MaRC::Mu0ImageFactory (*oblate_spheroid,
                                         ($5).lat,
                                         ($5).lon));
        }
;

phase:  _PHASE ':'
        sub_observ
        range
        sub_solar       {
          // cos (phase angle) * 10000
          image_factory =
            MaRC::ValuePtr<MaRC::CosPhaseImageFactory> (
              new MaRC::CosPhaseImageFactory (*oblate_spheroid,
                                              ($3).lat,
                                              ($3).lon,
                                              ($5).lat,
                                              ($5).lon,
                                              $4));
        }
;

lat_plane: LATITUDE ':' lat_type {
            // Latitudes in radians
            MaRC::ValuePtr<MaRC::BodyData> bd (
              new MaRC::OblateSpheroid (*oblate_spheroid));

            image_factory =
              MaRC::ValuePtr<MaRC::LatitudeImageFactory> (
                new MaRC::LatitudeImageFactory (bd, graphic_lat));
           }
;


lon_plane: LONGITUDE {
            // Longitudes in radians
            image_factory =
              MaRC::ValuePtr<MaRC::LongitudeImageFactory> (
                new MaRC::LongitudeImageFactory);
           }
;

flat_field:
        | FLAT_FIELD ':' _STRING  { photo_factory->flat_field ($3); free ($3);}
;

photo_correct:
        | MINNAERT ':' expr { /* Image->setMinnaertExponent ($3); */ }
        | MINNAERT ':' AUTO {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_AUTO); */ }
        | MINNAERT ':' TABLE {
            /* Image->setLimbCorrect(SourceImage::MINNAERT_TABLE); */ }
;

geom_correct:
        /* Do not perform geometric abberation correction if
           GEOM_CORRECT keyword not present */
        | GEOM_CORRECT ':' YES  {
            // Galileo spacecraft (GLL) geometric correction.
            photo_factory->geometric_correction (true);
          }
        | GEOM_CORRECT ':' NO   {
            photo_factory->geometric_correction (false);
          }
;

emi_ang_limit:
        | _EMI_ANG_LIMIT ':' expr { photo_factory->emi_ang_limit ($3); }
;

terminator:
        | TERMINATOR ':' YES { photo_factory->use_terminator (true);  }
        | TERMINATOR ':' NO  { photo_factory->use_terminator (false); }
;

range:  RANGE ':' expr  { $$ = $3; }
        | RANGE ':' expr KM  { $$ = $3; }
        | RANGE ':' expr AU  { $$ = $3 * C::astronomical_unit / 1000; }
;

image_geometry:
        km_per_pixel
        | arcsec_per_pix
        | lens_geometry
;

arcsec_per_pix:
        ARCSEC_PER_PIX ':' expr  { arcsec_per_pix_val = $3; }
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
        |
        options
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
        |
        options
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
        |
        options
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
        |
        options
        pole
;
*/

/* ------------------------- Mercator Projection ------------------------ */
mercator:
        MAP_TYPE ':' _MERCATOR  {
          switch (map_data_type) {
          case BYTE:
              map_factory_byte =
                  std::make_unique<MaRC::Mercator<FITS::byte_type>>(oblate_spheroid);
              break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::Mercator<FITS::short_type> (oblate_spheroid));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::Mercator<FITS::long_type> (oblate_spheroid));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::Mercator<FITS::float_type> (oblate_spheroid));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::Mercator<FITS::double_type> (oblate_spheroid));
                break;

              default:
                throw std::invalid_argument ("Unrecognized map data type");
                break;
            }
        }
;

/* ----------------------- Orthographic Projection ------------------------- */
ortho:  MAP_TYPE ':' _ORTHO
        ortho_options {
          switch (map_data_type)
            {
              case BYTE:
                map_factory_byte.reset (
                  new MaRC::Orthographic<FITS::byte_type> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!std::isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::Orthographic<FITS::short_type> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!std::isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::Orthographic<FITS::long_type> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!std::isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::Orthographic<FITS::float_type> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!std::isnan (position_angle_val)
                         ? position_angle_val : 0),
                        (km_per_pixel_val > 0 ? km_per_pixel_val : 0),
                        ortho_center));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::Orthographic<FITS::double_type> (
                        oblate_spheroid,
                        sub_observation_data.lat,
                        sub_observation_data.lon,
                        (!std::isnan (position_angle_val)
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
;

ortho_options:
        |
        options
        ortho_optsub
;

ortho_optsub:
        sub_observ {
          sub_observation_data.lat = ($1).lat;
          sub_observation_data.lon = ($1).lon;
        }
        | position_angle { position_angle_val = $1; }
        | km_per_pixel
        | centers {
            if (!std::isnan (sample_center) && !std::isnan (line_center))
              {
                ortho_center.geometry = MaRC::CENTER_GIVEN;
                ortho_center.sample_lat_center = sample_center;
                ortho_center.line_lon_center   = line_center;
                sample_center = MaRC::NaN;  // Reset to "bad" value.
                line_center   = MaRC::NaN;  // Reset to "bad" value.
              }
            else if (!std::isnan (lat_at_center) && !std::isnan (lon_at_center))
              {
                ortho_center.geometry = MaRC::LAT_LON_GIVEN;
                ortho_center.sample_lat_center = lat_at_center;
                ortho_center.line_lon_center   = lon_at_center;
                lat_at_center = MaRC::NaN;  // Reset to "bad" value.
                lon_at_center = MaRC::NaN;  // Reset to "bad" value.
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
            if (!std::isnan (sample_center) && !std::isnan (line_center))
              {
                ortho_center.geometry = MaRC::CENTER_GIVEN;
                ortho_center.sample_lat_center = sample_center;
                ortho_center.line_lon_center   = line_center;
                sample_center = MaRC::NaN;  // Reset to "bad" value.
                line_center   = MaRC::NaN;  // Reset to "bad" value.
              }
            else if (!std::isnan (lat_at_center) && !std::isnan (lon_at_center))
              {
                ortho_center.geometry = MaRC::LAT_LON_GIVEN;
                ortho_center.sample_lat_center = lat_at_center;
                ortho_center.line_lon_center   = lon_at_center;
                lat_at_center = MaRC::NaN;  // Reset to "bad" value.
                lon_at_center = MaRC::NaN;  // Reset to "bad" value.
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
        POSITION_ANGLE ':' expr    { $$ = $3; }
        | POSITION_ANGLE ':' expr CW   {
            if ($3 >= 0)
              $$ = $3;
            else
              {
                std::cerr << "Incorrect position (North) angle entered: "
                          << $3 << " CW" << std::endl
                          << "Numeric value should be positive." << std::endl;
              }
        }
        | POSITION_ANGLE ':' expr CCW {
            if ($3 >= 0)
              $$ = $3;
            else
              {
                std::cerr << "Incorrect position (North) angle entered: "
                          << $3 << " CCW" << std::endl
                          << "Numeric value should be positive." << std::endl;
              }
        }
;

km_per_pixel:
        KM_PER_PIXEL ':' expr  { km_per_pixel_val = $3; }
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
        | SAMPLE_OA ':' expr
          LINE_OA   ':' expr  { photo_factory->optical_axis ($3, $6); }
        | LINE_OA   ':' expr
          SAMPLE_OA ':' expr  { photo_factory->optical_axis ($6, $3); }
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
        |
        options { in_perspective_options = true; }
        perspective_optsub
;

perspective_optsub:
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
          switch (map_data_type)
            {
              case BYTE:
                map_factory_byte.reset (
                  new MaRC::PolarStereographic<FITS::byte_type> (oblate_spheroid,
                                                               max_lat,
                                                               north_pole));
                break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::PolarStereographic<FITS::short_type> (oblate_spheroid,
                                                       max_lat,
                                                       north_pole));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::PolarStereographic<FITS::long_type> (oblate_spheroid,
                                                            max_lat,
                                                            north_pole));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::PolarStereographic<FITS::float_type> (oblate_spheroid,
                                                       max_lat,
                                                       north_pole));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::PolarStereographic<FITS::double_type> (oblate_spheroid,
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
;

p_stereo_options:
        |
        options
        pole
        max_latitude
;

/* -------------- Simple Cylindrical (Rectangular ?) Projection ------------ */
simple_c:
        MAP_TYPE ':' _SIMPLE_C
        simple_c_options {
          switch (map_data_type)
            {
              case BYTE:
                map_factory_byte.reset (
                  new MaRC::SimpleCylindrical<FITS::byte_type>(
                        oblate_spheroid,
                        lo_lat,
                        hi_lat,
                        lo_lon,
                        hi_lon,
                        graphic_lat));
                break;

              case SHORT:
                map_factory_short.reset (
                  new MaRC::SimpleCylindrical<FITS::short_type>(oblate_spheroid,
                                                      lo_lat,
                                                      hi_lat,
                                                      lo_lon,
                                                      hi_lon,
                                                      graphic_lat));
                break;

              case LONG:
                map_factory_long.reset (
                  new MaRC::SimpleCylindrical<FITS::long_type> (
                    oblate_spheroid,
                    lo_lat,
                    hi_lat,
                    lo_lon,
                    hi_lon,
                    graphic_lat));
                break;

              case FLOAT:
                map_factory_float.reset (
                  new MaRC::SimpleCylindrical<FITS::float_type> (oblate_spheroid,
                                                      lo_lat,
                                                      hi_lat,
                                                      lo_lon,
                                                      hi_lon,
                                                      graphic_lat));
                break;

              case DOUBLE:
                map_factory_double.reset (
                  new MaRC::SimpleCylindrical<FITS::double_type>(oblate_spheroid,
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
;

simple_c_options:
        |
        options
        lat_type
        simple_c_latlonrange
;

lat_type:
        LATITUDE_TYPE ':' CENTRIC   { graphic_lat = false; }
        | LATITUDE_TYPE ':' GRAPHIC { graphic_lat = true;  }
/* The following are only for backward compatibility. */
        | CENTRIC   { graphic_lat = false; }
        | GRAPHIC   { graphic_lat = true;  }
;

simple_c_latlonrange:
        | lat_range
        | lon_range
        | lat_range lon_range
        | lon_range lat_range
;

lat_range:
        LO_LAT ':' latitude
        HI_LAT ':' latitude  {
          if ($3 < $6)
            {
              lo_lat = $3;
              hi_lat = $6;
            }
          else
            std::cerr << "Error: LO_LAT is greater than HI_LAT" << std::endl;
        }
        | HI_LAT ':' latitude
          LO_LAT ':' latitude  {
          if ($3 < $6)
            {
              lo_lat = $6;
              hi_lat = $3;
            }
          else
            std::cerr << "Error: LO_LAT is greater than HI_LAT" << std::endl;
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
        | AVERAGING ':' UNWEIGHTED {
              averaging_type = MaRC::MosaicImage::AVG_UNWEIGHTED; }
        | AVERAGING ':' WEIGHTED {
              averaging_type = MaRC::MosaicImage::AVG_WEIGHTED; }
        | AVERAGING ':' NONE {
              averaging_type = MaRC::MosaicImage::AVG_NONE; }
;

/* ----------------------- General Subroutines ---------------------------- */
/*
one_std_lat:
        | STD_LAT ':' latitude {
          if ((north_pole && $3 > 0 && $3 < 90) ||
              (!north_pole && $3 < 0 && $3 > -90)) {
            MapEntry->setStdLat ($3);
          }
          else {
            cerr << "ERROR: Standard latitude must be greater than zero for"
                 << endl
                 << "       north polar projections and negative for south"
                 << endl
                 << "       polar projections.  Absolute value of standard"
                 << endl
                 << "       latitude must also be less than 90 degrees."
                 << endl;
            cerr << "       STD_LAT: " << $3 << endl;
          }
        }
;

two_std_lats:
        | STD_LAT_1 ':' latitude
          STD_LAT_2 ':' latitude {
            if (::fabs ($6) < fabs ($3) &&
                (north_pole && $3 < 90 && $3 > 0) ||
                (!north_pole && $3 > -90 && $3 < 0)) {
              MapEntry->setStdLat ($3, $6);
            }
            else {
              cerr << "ERROR: Absolute value of first standard latitude must"
                   << endl
                   << "       be greater than absolute value of second"
                   << endl
                   << "       standard latitude.  First standard latitude must"
                   << endl
                   << "       also be greater than zero for north polar"
                   << endl
                   << "       projections and negative for south polar" << endl
                   << "       projections.  The absolute value of either"
                   << endl
                   << "       standard latitude cannot be zero or 90 degrees."
                   << endl;
              cerr << "       STD_LAT_1: " << $3 << endl
                   << "       STD_LAT_2: " << $6 << endl;
            }
        }
;
*/

max_latitude:
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
            $$ = oblate_spheroid->centric_latitude ($1 * C::degree)
                   / C::degree; }
;

latitude_sub:
        expr    {
          if (::fabs ($1) <= 90)
            $$ = $1;
          else {
            std::cerr << "Incorrect latitude entered: " << $1 << std::endl;
          }
        }
        | expr 'N' {
          if ($1 >= 0 && $1 <= 90)
            $$ = $1;
          else {
            std::cerr << "Incorrect latitude entered: " << $1 << " N"
                      << std::endl;
          }
        }
        | expr 'S' {
            if ($1 >= 0 && $1 <= 90)
              $$ = -$1;
            else {
              std::cerr << "Incorrect latitude entered: " << $1 << " S"
                        << std::endl;
            }
        }
;

longitude:
          expr          {
                if (::fabs ($1) <= 360)
                  {
                    if ($1 < 0)
                      $1+= 360;
                    $$ = $1;
                  }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << $1
                              << std::endl;
                  }
          }
        | expr 'E'      {
                if (::fabs ($1) <= 360)
                  {
                    if ($1 < 0)
                      $1+= 360;
                    if (oblate_spheroid->prograde ())
                      $$ = 360 - $1;
                    else
                      $$ = $1;
                }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << $1
                              << std::endl;
                  }
        }
        | expr 'W'      {
                if (::fabs ($1) <= 360)
                  {
                    if ($1 < 0)
                      $1+= 360;
                    if (oblate_spheroid->prograde ())
                      $$ = $1;
                    else
                      $$ = 360 - $1;
                  }
                else
                  {
                    std::cerr << "Incorrect longitude entered: " << $1
                              << std::endl;
                  }
        }
;

/* --------------- Multifunction Infix Notation Calculator ----------------- */
/* All numbers will be handled with double precision here. */
expr:     NUM                   { $$ = $1;                         }
        | VAR                   { $$ = $1->value.var;              }
        | VAR '=' expr          { $$ = $3; $1->value.var = $3;     }
        | FNCT '(' expr ')'     { $$ = (* ($1->value.fnctptr)) ($3); }
        | expr '+' expr         { $$ = $1 + $3;                    }
        | expr '-' expr         { $$ = $1 - $3;                    }
        | expr '*' expr         { $$ = $1 * $3;                    }
        | expr '/' expr         { if ($3 != 0)
                                    $$ = $1 / $3;
                                  else
                                    {
                                      $$ = $1;
                                      fprintf (stderr,
                                               "%d.%d-%d.%d: division by zero",
                                               @3.first_line,
                                               @3.first_column,
                                               @3.last_line,
                                               @3.last_column);
                                    }
                                }
        | '-' expr  %prec NEG   { $$ = -$2;                        }
        | expr '^' expr         { $$ = ::pow ($1, $3);             }
        | '(' expr ')'          { $$ = $2;                         }
;
/* ------------------------------------------------------------------------- */

%%
