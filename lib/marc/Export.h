// -*- C++ -*-
/**
 * @file Export.h
 *
 * Copyright (C) 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 *
 * @see http://gcc.gnu.org/wiki/Visibility
 */

#ifndef MARC_EXPORT_H
#define MARC_EXPORT_H

// Shared library support - internal macros
#if defined _WIN32 || defined __CYGWIN__
#  define MARC_DLL_IMPORT __declspec(dllimport)
#  define MARC_DLL_EXPORT __declspec(dllexport)
#  define MARC_DLL_LOCAL
#else
#  if __GNUC__ >= 4
#    define MARC_DLL_IMPORT [[gnu::visibility("default")]]
#    define MARC_DLL_EXPORT [[gnu::visibility("default")]]
#    define MARC_DLL_LOCAL  [[gnu::visibility("hidden")]]
#  else
#    define MARC_DLL_IMPORT
#    define MARC_DLL_EXPORT
#    define MARC_DLL_LOCAL
#    endif
#endif

// MaRC shared library macros: MARC_API and MARC_LOCAL
#ifdef MARC_DLL // MaRC library is compiled as a shared library (DLL)
#  ifdef MARC_DLL_EXPORTS // MaRC DLL being built rather than used
#    define MARC_API MARC_DLL_EXPORT
#  else
#    define MARC_API MARC_DLL_IMPORT
#  endif
#  define MARC_LOCAL MARC_DLL_LOCAL
#else // MaRC compiled as a static library
#  define MARC_API
#  define MARC_LOCAL
#endif // MARC_DLL

#endif  // MARC_EXPORT_H
