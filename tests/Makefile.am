## Copyright (C) 1999, 2004, 2017, 2020  Ossama Othman
##
## SPDX-License-Identifier: GPL-2.0-or-later

##  Process this file with automake to create Makefile.in

include $(top_srcdir)/aminclude_static.am

MARC_LIB = $(top_builddir)/lib/marc/libMaRC.la

AM_CPPFLAGS =               \
  -I$(top_srcdir)/lib       \
  $(FMTLIB_CPPFLAGS)        \
  $(CATCH2_CPPFLAGS)        \
  $(CODE_COVERAGE_CPPFLAGS)
AM_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)
AM_LIBS = $(CODE_COVERAGE_LIBS)

check_PROGRAMS = marc_test

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

marc_test_SOURCES =			\
	DefaultConfiguration_Test.cpp	\
	Math_Test.cpp			\
	Vector_Test.cpp			\
	Matrix_Test.cpp			\
	Geometry_Test.cpp		\
	OblateSpheroid_Test.cpp		\
	Scale_Offset_Test.cpp		\
	LatitudeImage_Test.cpp		\
	LongitudeImage_Test.cpp		\
	ViewingGeometry_Test.cpp	\
	Mercator_Test.cpp		\
	Orthographic_Test.cpp		\
	PolarStereographic_Test.cpp	\
	optional_test.cpp		\
	root_find_test.cpp		\
	utility_test.cpp		\
	validate_test.cpp		\
	marc_test.cpp

marc_test_LDADD =				\
	$(top_builddir)/lib/marc/libMaRC.la	\
	$(CODE_COVERAGE_LIBS)

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
