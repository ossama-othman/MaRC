dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

dnl  Copyright  1996-1999, 2003-2004, 2017  Ossama Othman
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License along
dnl with this program; if not, write to the Free Software Foundation, Inc.,
dnl 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

dnl An autoconf script to automatically configure MaRC.


AC_PREREQ([2.69])
AC_INIT([MaRC], [0.9.9-beta], [othman@users.sourceforge.net], [marc])

dnl Additional configure script arguments
AX_IS_RELEASE([git-directory])
AX_CHECK_ENABLE_DEBUG([yes])

dnl Check what platform we are running on.
dnl AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.14.1 foreign silent-rules -Wall -Werror -Wno-portability])
AM_SILENT_RULES([yes])
LT_INIT([disable-static])

AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_SRCDIR([lib/MaRC/PhotoImage.h])

dnl Generate a header file with all settings.
AC_CONFIG_HEADERS([lib/MaRC/config.h])

dnl Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_14
AC_PROG_LN_S
AC_PROG_YACC
AM_PROG_LEX

# Check for Doxygen
DX_DOXYGEN_FEATURE(ON)
DX_DOT_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([marc], [Doxyfile], [doc/doxygen])

AC_LANG([C++])

dnl
dnl config.h template
dnl

dnl Text to be placed at the top of the `MaRC/config.h' header.
AH_TOP([
#ifndef MARC_CONFIG_H
#define MARC_CONFIG_H

// MaRC configuration header file


])

dnl Text to be placed at the bottom of the `MaRC/config.h' header.
AH_BOTTOM([

#endif   /* MARC_CONFIG_H */

// Local] [Variables:
// mode:C++
// End:
])

dnl AH_TEMPLATE([DEBUG],[Symbol used to turn off array bounds checking in TNT header files])
AH_TEMPLATE([NDEBUG],[Disable all debugging code.])
dnl AH_TEMPLATE([TNT_BOUNDS_CHECK],[Track TNT index bounds.])

AH_TEMPLATE([MARC_HAS_STL_QUIET_NAN_SUPPORT],[STL supports quiet NaN.])
AH_TEMPLATE([MARC_C_LIB_QUIET_NAN],[C library quiet NaN value.])

AH_TEMPLATE([MARC_HAS_COVARIANT_RETURN_TYPES],[C++ compiler supports covariant return types.])

dnl Checks for libraries.

dnl We're of course going to need the C math library.
AC_SEARCH_LIBS([cos],[m])

dnl Check if the socket library is available and needed.  CFITSIO
dnl requires socket support.
AC_SEARCH_LIBS([socket],[socket],,,[nsl])

dnl Check for gethostbyname in -lnsl since some platforms (e.g. Solaris)
dnl put it there.  CFITSIO may need it.
AX_LIB_SOCKET_NSL

dnl Check for the CFITSIO library.
dnl   fits_create_file is a preprocessor macro for ffinit.  *sigh*
AC_SEARCH_LIBS([ffinit],[cfitsio],
  [],
  [AC_MSG_ERROR([MaRC requires CFITSIO.  CFITSIO library not found.])])

dnl Checks for library functions.
AC_CHECK_FUNCS([rint])

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_CACHE_CHECK([if $CXX supports covariant return types],
  [ac_cv_cxx_covariant_return_types_supported],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
class Base
{
public:
  virtual void foo () = 0;
};

class Derived : public Base
{
public:
  virtual void foo () {}
};

class Factory
{
public:
  virtual Base * make () = 0;
};

class ConcreteFactory : public Factory
{
public:
  virtual Derived * make () { return new Derived; }
};
      ]],
      [[
ConcreteFactory factory;
Derived * d = factory.make ();
      ]])
    ],
    [
     ac_cv_cxx_covariant_return_types_supported=yes
    ],
    [
     ac_cv_cxx_covariant_return_types_supported=no
    ])
  ])

AS_IF([test "x$ac_cv_cxx_covariant_return_types_supported" = "xyes"],
      [AC_DEFINE([MARC_HAS_COVARIANT_RETURN_TYPES])])

dnl System specific configuration
AS_IF([test "x$ax_enable_debug" != "xyes"],
      [AS_IF([test "x$GXX" = "xyes"], [XCXXFLAGS="$XCXXFLAGS -O2"])
       AX_GCC_ARCHFLAG([yes], [XCXXFLAGS="$XCXXFLAGS $ax_cv_gcc_archflag"])])

AX_CXXFLAGS_WARN_ALL([CXXFLAGS])
AS_IF([test "x$GXX" = "xyes"],
      [XCXXFLAGS="$XCXXFLAGS -Wextra -Wformat=2 -pedantic"],
      [])

dnl Combine package set flags with user's flags.
dnl User's flags go after package flags to allow user to override
dnl package defaults.
CXXFLAGS="$XCXXFLAGS $CXXFLAGS"
CPPFLAGS="$XCPPFLAGS $CPPFLAGS"
LDFLAGS="$XLDFLAGS $LDFLAGS"

dnl AC_CONFIG_SUBDIRS(lib/popt)dnl

AC_CONFIG_FILES([
  Makefile
  doc/Makefile
  lib/Makefile
  lib/MaRC/Makefile
  src/Makefile
  tests/Makefile
])
AC_OUTPUT
