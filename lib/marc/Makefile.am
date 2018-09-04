## Copyright (C) 1996-1999, 2017-2018  Ossama Othman
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
## 02110-1301  USA

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
  optional.h \
  plot_info.h \
  root_find.h \
  utility.h \
  \
  Constants.h \
  \
  Export.h \
  \
  details/scale_and_offset.h \
  details/vector.h

# noinst_HEADERS =

clean-local: code-coverage-clean
dist-clean-local: code-coverage-dist-clean
