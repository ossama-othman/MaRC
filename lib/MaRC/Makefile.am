##  $Id: Makefile.am,v 1.26 2005/11/12 07:51:29 othman Exp $

##
##  Process this file with automake to create Makefile.in
##


INCLUDES = -I$(top_builddir)/lib -I$(top_srcdir)/lib

lib_LTLIBRARIES = libMaRC.la

libMaRC_la_SOURCES = \
  NaN.cpp \
  \
  Geometry.cc \
  \
  BodyData.cc \
  OblateSpheroid.cc \
  \
  GeometricCorrection.cc \
  GLLGeometricCorrection.cc \
  NullGeometricCorrection.cc \
  \
  PhotometricCorrection.cc \
  NullPhotometricCorrection.cc \
  \
  InterpolationStrategy.cpp \
  PhotoInterpolationStrategy.cpp \
  NullInterpolationStrategy.cpp \
  \
  SourceImage.cc \
  VirtualImage.cc \
  MuImage.cc \
  Mu0Image.cc \
  CosPhaseImage.cc \
  LatitudeImage.cc \
  LongitudeImage.cc \
  PhotoImage.cc \
  PhotoImageParameters.cc \
  MosaicImage.cc \
  \
  Grid.cc \
  \
  map_version.cc

pkginclude_HEADERS = \
  config.h \
  \
  NaN.h \
  \
  IfThenElse.h \
  \
  Long.h \
  \
  Matrix.h \
  Vector.h \
  \
  Geometry.h \
  \
  BodyData.h \
  OblateSpheroid.h \
  \
  misc.h \
  root_find.h \
  \
  SourceImage.h \
  VirtualImage.h \
  MuImage.h \
  Mu0Image.h \
  CosPhaseImage.h \
  LatitudeImage.h \
  LongitudeImage.h \
  PhotoImage.h \
  PhotoImageParameters.h \
  MosaicImage.h \
  \
  GeometricCorrection.h \
  GLLGeometricCorrection.h \
  NullGeometricCorrection.h \
  \
  InterpolationStrategy.h \
  \
  PhotometricCorrection.h \
  NullPhotometricCorrection.h \
  \
  Image.h \
  Image.cpp \
  Grid.h \
  \
  Map_traits.h \
  MapFactory.h \
  MapFactory.cc \
  Mercator.h \
  Mercator.cpp \
  Orthographic.h \
  Orthographic.cc \
  PolarStereographic.h \
  PolarStereographic.cpp \
  SimpleCylindrical.h \
  SimpleCylindrical.cc \
  \
  Constants.h \
  ValuePtr.h

noinst_HEADERS = \
  NullInterpolationStrategy.h \
  PhotoInterpolationStrategy.h

BUILT_SOURCES = map_version.cc

map_version.cc: stamp-v
stamp-v: Makefile
	rm -f t-version.cc
#	echo '#include <MaRC/config.h>' > t-version.cc
#	echo '#include "version.h"' >> t-version.cc
	echo 'char const * marc_lib_version_string = "@MARC_LIB_VERSION@";' \
		>> t-version.cc
	if cmp -s map_version.cc t-version.cc; then   \
		rm t-version.cc;                      \
	else                                          \
		mv t-version.cc map_version.cc;       \
	fi
	echo timestamp > $@

DISTCLEANFILES = stamp-v map_version.cc
