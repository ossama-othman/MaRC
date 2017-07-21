## Copyright (C) 1999, 2004, 2017 Ossama Othman
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## by the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

##
##  Process this file with automake to create Makefile.in
##

MARC_LIB = $(top_builddir)/lib/MaRC/libMaRC.la

AM_CPPFLAGS = -I$(top_srcdir)/lib

check_PROGRAMS = \
  NaN_Test \
  Vector_Test

dist_check_SCRIPTS = MaRC_Prog_Test.sh

dist_check_DATA = \
  test_map.inp \
  test.fits.gz

TESTS = \
  $(check_PROGRAMS) \
  $(dist_check_SCRIPTS)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir) top_srcdir=$(top_srcdir)

NaN_Test_SOURCES = NaN_Test.cpp
NaN_Test_LDADD = $(MARC_LIB)

Vector_Test_SOURCES = Vector_Test.cpp

CLEANFILES = test_map.fits

all-local:
	@if test ! -r test.fits.gz; then \
	  $(LN_S) $(top_srcdir)/tests/test.fits.gz test.fits.gz; \
	  touch link-stamp; \
	fi

clean-local:
	@if test -f link-stamp; then \
	  $(RM) test.fits.gz link-stamp; \
	fi
