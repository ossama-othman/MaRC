// -*- C++ -*-
/**
 * @file marc_export.h
 *
 * Copyright (C) 2017  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
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
#    define MARC_DLL_IMPORT __attribute__((visibility ("default")))
#    define MARC_DLL_EXPORT __attribute__((visibility ("default")))
#    define MARC_DLL_LOCAL  __attribute__((visibility ("hidden")))
#  else
#    define MARC_DLL_IMPORT
#    define MARC_DLL_EXPORT
#    define MARC_DLL_LOCAL
#    endif
#endif

// Automatically enable symbol visibility (shared library) support if
// the "PIC" preprocessor symbol is defined.
#if defined(PIC) && !defined(MARC_DLL) and !defined(MARC_DLL_EXPORTS)
#  define MARC_DLL
#  define MARC_DLL_EXPORTS
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
