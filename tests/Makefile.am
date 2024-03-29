## Copyright (C) 1999, 2004, 2017, 2020  Ossama Othman
##
## SPDX-License-Identifier: GPL-2.0-or-later

##  Process this file with automake to create Makefile.in

include $(top_srcdir)/aminclude_static.am

MARC_LIB = $(top_builddir)/lib/marc/libMaRC.la

AM_CPPFLAGS =               \
  -I$(top_srcdir)/lib       \
  -I$(top_builddir)/lib     \
  $(FMTLIB_CPPFLAGS)        \
  $(CODE_COVERAGE_CPPFLAGS)
AM_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)
AM_LIBS = $(CODE_COVERAGE_LIBS)

library_tests =                 \
  DefaultConfiguration_Test     \
  Math_Test                     \
  Vector_Test                   \
  Matrix_Test                   \
  Geometry_Test                 \
  OblateSpheroid_Test           \
  Scale_Offset_Test             \
  LatitudeImage_Test            \
  LongitudeImage_Test           \
  ViewingGeometry_Test          \
  Mercator_Test                 \
  Orthographic_Test             \
  PolarStereographic_Test       \
  compositing_strategy_test     \
  extrema_test                  \
  log_test                      \
  root_find_test                \
  utility_test                  \
  validate_test

program_tests = \
  map_parameters_test

check_PROGRAMS = $(library_tests) $(program_tests)

dist_check_SCRIPTS = command_line_test.sh

## MaRC input files with intentionally invalid values.  These are used
## to exercise error handling in the MaRC input file parser.
BAD_MARC_FILES = \
  bad/test_arcsec_per_pix.marc \
  bad/test_byte_blank.marc \
  bad/test_short_blank.marc \
  bad/test_long_blank.marc \
  bad/test_longlong_blank.marc \
  bad/test_float_blank.marc \
  bad/test_double_blank.marc \
  bad/test_data_min_max.marc \
  bad/test_grid_interval.marc \
  bad/test_km_per_pixel.marc \
  bad/test_lat_grid_interval.marc \
  bad/test_latitude.marc \
  bad/test_latitude_n.marc \
  bad/test_latitude_s.marc \
  bad/test_lines.marc \
  bad/test_longitude_e.marc \
  bad/test_longitude.marc \
  bad/test_longitude_w.marc \
  bad/test_lon_grid_interval.marc \
  bad/test_nibble_bottom.marc \
  bad/test_nibble_left.marc \
  bad/test_nibble.marc \
  bad/test_nibble_right.marc \
  bad/test_nibble_top.marc \
  bad/test_plane_number.marc \
  bad/test_planes_count.marc \
  bad/test_planes_size.marc \
  bad/test_position_angle_ccw.marc \
  bad/test_position_angle_cw.marc \
  bad/test_position_angle.marc \
  bad/test_radius.marc \
  bad/test_samples.marc

nobase_dist_check_DATA = \
  config/marc            \
  test_map.marc          \
  test.fits.gz           \
  $(BAD_MARC_FILES)

## -------------------------------------------------------------------

check_LTLIBRARIES = libMaRC_test_images.la

libMaRC_test_images_la_SOURCES = \
  fixed_value_image.h            \
  fixed_value_image.cpp

## -------------------------------------------------------------------

DefaultConfiguration_Test_SOURCES = DefaultConfiguration_Test.cpp
DefaultConfiguration_Test_LDADD   = $(CODE_COVERAGE_LIBS)

Math_Test_SOURCES = Math_Test.cpp
Math_Test_LDADD   = $(CODE_COVERAGE_LIBS)

Vector_Test_SOURCES = Vector_Test.cpp
Vector_Test_LDADD   = $(CODE_COVERAGE_LIBS)

Matrix_Test_SOURCES = Matrix_Test.cpp
Matrix_Test_LDADD   = $(CODE_COVERAGE_LIBS)

Geometry_Test_SOURCES = Geometry_Test.cpp
Geometry_Test_LDADD = \
   $(MARC_LIB) \
   $(CODE_COVERAGE_LIBS)

OblateSpheroid_Test_SOURCES = OblateSpheroid_Test.cpp
OblateSpheroid_Test_LDADD = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

Scale_Offset_Test_SOURCES = Scale_Offset_Test.cpp
Scale_Offset_Test_LDADD   = $(CODE_COVERAGE_LIBS)

LatitudeImage_Test_SOURCES = LatitudeImage_Test.cpp
LatitudeImage_Test_LDADD = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

LongitudeImage_Test_SOURCES = LongitudeImage_Test.cpp
LongitudeImage_Test_LDADD = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

ViewingGeometry_Test_SOURCES = ViewingGeometry_Test.cpp
ViewingGeometry_Test_LDADD = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

Mercator_Test_SOURCES = Mercator_Test.cpp
Mercator_Test_LDADD = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

Orthographic_Test_SOURCES = Orthographic_Test.cpp
Orthographic_Test_LDADD = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

PolarStereographic_Test_SOURCES = PolarStereographic_Test.cpp
PolarStereographic_Test_LDADD = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

compositing_strategy_test_SOURCES = compositing_strategy_test.cpp
compositing_strategy_test_LDADD   = \
  -lMaRC_test_images \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)
compositing_strategy_test_DEPENDENCIES = libMaRC_test_images.la

extrema_test_SOURCES = extrema_test.cpp
extrema_test_LDADD   = \
  $(CODE_COVERAGE_LIBS)

log_test_SOURCES = log_test.cpp
log_test_LDADD   = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

root_find_test_SOURCES = root_find_test.cpp
root_find_test_LDADD   = \
  $(MARC_LIB) \
  $(CODE_COVERAGE_LIBS)

utility_test_SOURCES = utility_test.cpp
utility_test_LDADD   = $(CODE_COVERAGE_LIBS)

validate_test_SOURCES = validate_test.cpp
validate_test_LDADD   = $(CODE_COVERAGE_LIBS)

## -------------------------------------------------------------------

map_parameters_test_SOURCES = map_parameters_test.cpp
map_parameters_test_LDADD = \
  $(top_builddir)/src/libMaRC_private.la \
  $(CODE_COVERAGE_LIBS)

## -------------------------------------------------------------------

TESTS =                  \
  $(check_PROGRAMS)      \
  $(dist_check_SCRIPTS)  \
  test_map.marc          \
  $(BAD_MARC_FILES)

TEST_EXTENSIONS = .marc

MARC_LOG_COMPILER = $(top_builddir)/src/marc

TESTS_ENVIRONMENT = \
  XDG_CONFIG_HOME=$(srcdir)/config; \
  top_builddir=$(top_builddir); \
  top_srcdir=$(top_srcdir); \
  export XDG_CONFIG_HOME top_builddir top_srcdir;

## Tests expected to fail.
XFAIL_TESTS = \
  $(BAD_MARC_FILES)

CLEANFILES = \
  test_bad_map.fits \
  test_map.fits \
  test_map2.fits \
  test_map3.fits \
  test_map4.fits \
  test_map5.fits \
  test_map6.fits \
  test_map7.fits \
  test_map8.fits \
  test_map_ortho.fits \
  test_map_mercator.fits \
  test_map_polar_stereographic.fits

all-local:
	@if test ! -r test.fits.gz; then \
	  $(LN_S) $(srcdir)/test.fits.gz test.fits.gz; \
	  touch link-stamp; \
	fi

clean-local: code-coverage-clean
	@if test -f link-stamp; then \
	  $(RM) test.fits.gz link-stamp; \
	fi

# The LD_LIBRARY_PATH environment variable may need to be explicitly
# set when running `make installcheck' if the directory where the MaRC
# library was installed is not be known to the dynamic linker, e.g.:
#
#     LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib make installcheck
installcheck-local:
	$(DESTDIR)$(bindir)/marc$(EXEEXT) $(srcdir)/test_map.marc > /dev/null
