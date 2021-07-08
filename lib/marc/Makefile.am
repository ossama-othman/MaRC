## Copyright (C) 1996-1999, 2017-2018  Ossama Othman
##
## SPDX-License-Identifier: LGPL-2.1-or-later

##  Process this file with automake to create Makefile.in

include $(top_srcdir)/aminclude_static.am

if BUILDING_DLL
# libMaRC is being built as a shared library.  Export all symbols in
# public API.
DLL_CPPFLAGS = -DMARC_DLL -DMARC_DLL_EXPORTS
endif

AM_CPPFLAGS =                   \
  -I$(top_builddir)/lib         \
  -I$(top_srcdir)/lib           \
  $(FMTLIB_CPPFLAGS)            \
  $(CODE_COVERAGE_CPPFLAGS)     \
  $(DLL_CPPFLAGS)

lib_LTLIBRARIES = libMaRC.la

libMaRC_la_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)
libMaRC_la_LIBADD   = $(CODE_COVERAGE_LIBS)
libMaRC_la_LDFLAGS  = $(MARC_UNUSED_ARGUMENT_LDFLAGS)

libMaRC_la_SOURCES = \
  Log.cpp \
  Notifier.cpp \
  \
  Geometry.cpp \
  \
  OblateSpheroid.cpp \
  \
  ViewingGeometry.cpp \
  \
  GLLGeometricCorrection.cpp \
  NullGeometricCorrection.cpp \
  \
  NullPhotometricCorrection.cpp \
  \
  BilinearInterpolation.cpp \
  NullInterpolation.cpp \
  \
  SourceImage.cpp \
  VirtualImage.cpp \
  MuImage.cpp \
  Mu0Image.cpp \
  CosPhaseImage.cpp \
  LatitudeImage.cpp \
  LongitudeImage.cpp \
  PhotoImageParameters.cpp \
  PhotoImage.cpp \
  MosaicImage.cpp \
  \
  MapFactory.cpp \
  Mercator.cpp \
  Orthographic.cpp \
  PolarStereographic.cpp \
  SimpleCylindrical.cpp \
  \
  root_find.cpp

nobase_pkginclude_HEADERS = \
  Log.h \
  Observer.h \
  Notifier.h \
  DefaultConfiguration.h \
  config.h \
  \
  Mathematics.h   \
  Matrix.h \
  Vector.h \
  \
  Geometry.h \
  \
  BodyData.h \
  OblateSpheroid.h \
  \
  ViewingGeometry.h \
  \
  SourceImage.h \
  VirtualImage.h \
  MuImage.h \
  Mu0Image.h \
  CosPhaseImage.h \
  LatitudeImage.h \
  LongitudeImage.h \
  PhotoImageParameters.h \
  PhotoImage.h \
  MosaicImage.h \
  \
  GeometricCorrection.h \
  GLLGeometricCorrection.h \
  NullGeometricCorrection.h \
  \
  InterpolationStrategy.h \
  BilinearInterpolation.h \
  NullInterpolation.h \
  \
  PhotometricCorrection.h \
  NullPhotometricCorrection.h \
  \
  Map_traits.h \
  MapFactory.h \
  MapFactory_t.cpp \
  Mercator.h \
  Orthographic.h \
  PolarStereographic.h \
  SimpleCylindrical.h \
  Validate.h \
  plot_info.h \
  root_find.h \
  scale_and_offset.h \
  utility.h \
  \
  Constants.h \
  \
  Export.h \
  \
  details/scale_and_offset.h \
  details/vector.h

# noinst_HEADERS =

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = marc.pc

clean-local: code-coverage-clean
