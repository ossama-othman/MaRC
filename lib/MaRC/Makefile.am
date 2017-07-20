##
##  Process this file with automake to create Makefile.in
##


AM_CPPFLAGS = -I$(top_builddir)/lib -I$(top_srcdir)/lib

lib_LTLIBRARIES = libMaRC.la

libMaRC_la_SOURCES = \
  NaN.cpp \
  \
  Geometry.cpp \
  \
  BodyData.cpp \
  OblateSpheroid.cpp \
  \
  GeometricCorrection.cpp \
  GLLGeometricCorrection.cpp \
  NullGeometricCorrection.cpp \
  \
  PhotometricCorrection.cpp \
  NullPhotometricCorrection.cpp \
  \
  InterpolationStrategy.cpp \
  PhotoInterpolationStrategy.cpp \
  NullInterpolationStrategy.cpp \
  \
  SourceImage.cpp \
  VirtualImage.cpp \
  MuImage.cpp \
  Mu0Image.cpp \
  CosPhaseImage.cpp \
  LatitudeImage.cpp \
  LongitudeImage.cpp \
  PhotoImage.cpp \
  MosaicImage.cpp \
  \
  Grid.cpp \
  \
  Version.cpp

pkginclude_HEADERS = \
  config.h \
  \
  NaN.h \
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
  MapFactory.cpp \
  Mercator.h \
  Mercator.cpp \
  Orthographic.h \
  Orthographic.cpp \
  PolarStereographic.h \
  PolarStereographic.cpp \
  SimpleCylindrical.h \
  SimpleCylindrical.cpp \
  \
  Constants.h \
  ValuePtr.h \
  \
  Version.h

noinst_HEADERS = \
  NullInterpolationStrategy.h \
  PhotoInterpolationStrategy.h

