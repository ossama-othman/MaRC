## Copyright (C) 1999, 2004, 2017  Ossama Othman
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

##  Process this file with automake to create Makefile.in

include $(top_srcdir)/aminclude_static.am

MARC_LIB = $(top_builddir)/lib/MaRC/libMaRC.la

AM_CPPFLAGS = -I$(top_srcdir)/lib $(CODE_COVERAGE_CPPFLAGS)
AM_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)
AM_LIBS = $(CODE_COVERAGE_LIBS)

check_PROGRAMS =            \
  DefaultConfiguration_Test \
  Math_Test                 \
  Vector_Test               \
  Matrix_Test               \
  Geometry_Test             \
  OblateSpheroid_Test       \
  Scale_Offset_Test         \
  LatitudeImage_Test        \
  LongitudeImage_Test       \
  ViewingGeometry_Test      \
  Mercator_Test             \
  PolarStereographic_Test   \
  root_find_test            \
  utility_test

dist_check_SCRIPTS = MaRC_Prog_Test.sh command_line_test.sh

dist_check_DATA = \
  test_map.marc \
  test.fits.gz

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
   $(top_builddir)/lib/MaRC/libMaRC.la \
   $(CODE_COVERAGE_LIBS)

OblateSpheroid_Test_SOURCES = OblateSpheroid_Test.cpp
OblateSpheroid_Test_LDADD = \
  $(top_builddir)/lib/MaRC/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

Scale_Offset_Test_SOURCES = Scale_Offset_Test.cpp
Scale_Offset_Test_LDADD   = $(CODE_COVERAGE_LIBS)

LatitudeImage_Test_SOURCES = LatitudeImage_Test.cpp
LatitudeImage_Test_LDADD = \
  $(top_builddir)/lib/MaRC/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

LongitudeImage_Test_SOURCES = LongitudeImage_Test.cpp
LongitudeImage_Test_LDADD = \
  $(top_builddir)/lib/MaRC/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

ViewingGeometry_Test_SOURCES = ViewingGeometry_Test.cpp
ViewingGeometry_Test_LDADD = \
  $(top_builddir)/lib/MaRC/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

Mercator_Test_SOURCES = Mercator_Test.cpp
Mercator_Test_LDADD = \
  $(top_builddir)/lib/MaRC/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

PolarStereographic_Test_SOURCES = PolarStereographic_Test.cpp
PolarStereographic_Test_LDADD = \
  $(top_builddir)/lib/MaRC/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

root_find_test_SOURCES = root_find_test.cpp
root_find_test_LDADD   = \
  $(top_builddir)/lib/MaRC/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

utility_test_SOURCES = utility_test.cpp
utility_test_LDADD   = $(CODE_COVERAGE_LIBS)

TESTS = \
  $(check_PROGRAMS) \
  $(dist_check_SCRIPTS)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir) top_srcdir=$(top_srcdir)

## Tests known to fail.
XFAIL_TESTS =

CLEANFILES = \
  test_map.fits \
  test_map2.fits \
  test_map3.fits \
  test_map_ortho.fits \
  test_map_mercator.fits \
  test_map_polar_stereographic.fits

all-local:
	@if test ! -r test.fits.gz; then \
	  $(LN_S) $(top_srcdir)/tests/test.fits.gz test.fits.gz; \
	  touch link-stamp; \
	fi

clean-local: code-coverage-clean
	@if test -f link-stamp; then \
	  $(RM) test.fits.gz link-stamp; \
	fi

dist-clean-local: code-coverage-dist-clean
