## Copyright (C) 1996-1998, 2004, 2017  Ossama Othman
##
## SPDX-License-Identifier: GPL-2.0-or-later

##  Process this file with automake to create Makefile.in


#LEXLIB  = @LEXLIB@

#DISTCLEANFILES = stamp-v version.cpp ##lexer.cpp parse.{cc,h}

include $(top_srcdir)/aminclude_static.am

AM_CPPFLAGS =           \
  -I$(top_builddir)/lib \
  -I$(top_srcdir)/lib   \
  $(FMTLIB_CPPFLAGS)    \
  $(CODE_COVERAGE_CPPFLAGS)

bin_PROGRAMS = marc

AM_YFLAGS = -d

marc_SOURCES = \
  FITS_file.cpp \
  FITS_image.cpp \
  ProgressConsole.cpp \
  SourceImageFactory.cpp \
  MuImageFactory.cpp \
  Mu0ImageFactory.cpp \
  CosPhaseImageFactory.cpp \
  LatitudeImageFactory.cpp \
  LongitudeImageFactory.cpp \
  PhotoImageFactory.cpp \
  MosaicImageFactory.cpp \
  MapCommand.cpp \
  MapParameters.cpp \
  command_line.cpp \
  marc.cpp \
  parse_scan.cpp \
  calc.cpp \
  parse.yy \
  lexer.ll

marc_CXXFLAGS = $(CFITSIO_CFLAGS) $(CODE_COVERAGE_CXXFLAGS)
marc_LDADD    = \
  $(CFITSIO_LIBS) \
  $(ARGP_LIBS) \
  $(top_builddir)/lib/marc/libMaRC.la \
  $(CODE_COVERAGE_LIBS)

noinst_HEADERS = \
  FITS_traits.h \
  FITS_file.h \
  FITS_image.h \
  FITS_image_t.cpp \
  ProgressConsole.h \
  SourceImageFactory.h \
  MuImageFactory.h \
  Mu0ImageFactory.h \
  CosPhaseImageFactory.h \
  LatitudeImageFactory.h \
  LongitudeImageFactory.h \
  PhotoImageFactory.h \
  MosaicImageFactory.h \
  MapCommand.h \
  MapCommand_t.cpp \
  MapParameters.h \
  calc.h \
  command_line.h \
  parse_scan.h \
  strerror.h \
  lexer.hh

BUILT_SOURCES = parse.hh lexer.hh
lexer.hh: lexer.cc

## Override Automake generated rule.  Remove once it correctly supports
## C++ scanners generated by GNU Flex.  A fix for an incorrect call to
## the C++ operator delete() in a Flex 2.5.4a generated C++ scanner is
## also included.
#.ll.cc:
#	$(LEXCOMPILE) $<
#	sed '/^#/ s|$(LEX_OUTPUT_ROOT)\.cc|$@|' $(LEX_OUTPUT_ROOT).cc >$@
#	rm -f $(LEX_OUTPUT_ROOT).cc
#	sed -e 's/delete yy_state_buf/delete [] yy_state_buf/' $@ > $@.new
#	mv $@.new $@

clean-local: code-coverage-clean
