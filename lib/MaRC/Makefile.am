## Copyright (C) 1996-1999, 2017  Ossama Othman
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

AM_CPPFLAGS = -I$(top_builddir)/lib -I$(top_srcdir)/lib

lib_LTLIBRARIES = libMaRC.la

libMaRC_la_SOURCES = \
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
  Version.cpp

pkginclude_HEADERS = \
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
  Validate.h \
  \
  Constants.h \
  \
  Version.h

noinst_HEADERS = \
  misc.h \
  root_find.h \
  \
  NullInterpolationStrategy.h \
  PhotoInterpolationStrategy.h
