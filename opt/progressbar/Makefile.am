## Copyright (C) 2021  Ossama Othman
##
## SPDX-License-Identifier: LGPL-2.1-or-later

##  Process this file with automake to create Makefile.in

include $(top_srcdir)/aminclude_static.am

AM_CPPFLAGS =                   \
  -I$(top_srcdir)/opt/progressbar/include/progressbar
  $(CODE_COVERAGE_CPPFLAGS)

noinst_LTLIBRARIES = lib/libprogressbar.la

noinst_HEADERS = include/progressbar/progressbar.h

lib_libprogressbar_la_CFLAGS = $(CODE_COVERAGE_CXXFLAGS)
lib_libprogressbar_la_LIBADD   = $(NCURSES_LIBS) $(CODE_COVERAGE_LIBS)
lib_libprogressbar_la_SOURCES  = lib/progressbar.c

clean-local: code-coverage-clean
