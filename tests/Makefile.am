##  $Id: Makefile.am,v 1.5 2004/11/08 19:05:08 othman Exp $
##
##  Copyright © 1999, 2004 Ossama Othman
##
##  All Rights Reserved
##
##  This program is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, write to the Free Software
##  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



##
##  Process this file with automake to create Makefile.in
##

MARC_LIB = $(top_builddir)/lib/MaRC/libMaRC.la

AM_CPPFLAGS = -I$(top_srcdir)/lib

check_PROGRAMS = \
  Data_Type_Test \
  NaN_Test

dist_check_SCRIPTS = MaRC_Prog_Test.sh

dist_check_DATA = \
  test_map.inp \
  test.fits.gz

TESTS = \
  $(check_PROGRAMS) \
  $(dist_check_SCRIPTS)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir) top_srcdir=$(top_srcdir)

Data_Type_Test_SOURCES = Data_Type_Test.cpp

NaN_Test_SOURCES = NaN_Test.cpp
NaN_Test_LDADD = $(MARC_LIB)

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

