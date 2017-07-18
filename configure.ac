dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

dnl  Copyright  1996-1999, 2003, 2004  Ossama Othman
dnl
dnl  All Rights Reserved
dnl
dnl  This program is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation; either version 2 of the License, or
dnl  (at your option) any later version.
dnl
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU General Public License for more details.
dnl
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program; if not, write to the Free Software
dnl  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

dnl
dnl An autoconf script to automatically configure MaRC.
dnl Process this file with autoconf to produce a configure script.
dnl

AC_PREREQ([2.69])
AC_INIT([MaRC], [0.9.8], [othman@users.sourceforge.edu], [marc])

dnl Check what platform we are running on.
dnl AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.15 foreign silent-rules -Wall -Werror -Wno-portability])
LT_INIT([disable-static])

AC_COPYRIGHT([MaRC is copyrighted by Ossama Othman 1996-1999, 2003, 2004, 2017
all rights reserved.  You are free to use, modify, and distribute the
MaRC source code and object code produced from the source, as long as
you include this copyright statement along with code built using
MaRC.

Please see the file "COPYING" in the top level MaRC directory for
additional details.])

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
##AC_CHECK_PROG([TEXI2DVI], [texi2dvi], [texi2dvi], [])
##AC_CHECK_PROGS([TEXI2PDF], [texi2pdf ps2pdf], [])
##AC_CHECK_PROG([DVIPS], [dvips], [dvips], [])

AC_LANG([C++])

dnl The following YACC and LEX stuff will probably be removed soon.
dnl if test "$YACC" = "bison -y"; then
dnl   YFLAGS="-d $YFLAGS"
dnl else
dnl   ./missing bison
dnl fi

dnl if test -z "$LEX"; then
dnl   AC_MSG_ERROR([GNU Flex not found.  GNU Flex 2.5.4a or newer is REQUIRED.])
dnl fi

dnl Enable C99 extensions if not already enabled.  Needed for NaN
dnl support on some platforms.
XCPPFLAGS="-D_ISOC99_SOURCE"

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

dnl Additional configure script arguments
AC_ARG_ENABLE([debug],
  AC_HELP_STRING([--enable-debug],[enable debugging [[no]]]), [
  if test "x$GXX" = "xno"; then
    XCXXFLAGS="-g"
  fi
dnl   AC_DEFINE([TNT_BOUNDS_CHECK])
dnl   AC_DEFINE([DEBUG])
  echo "enabling debugging for C++:  $XCXXFLAGS $CXXFLAGS"
], [AC_DEFINE([NDEBUG])])

dnl Checks for libraries.

dnl We're of course going to need the C math library.
AC_SEARCH_LIBS([cos],[m])

dnl Check if the socket library is available and needed.  CFITSIO
dnl requires socket support.
AC_SEARCH_LIBS([socket],[socket],,,[nsl])

dnl Check for gethostbyname in -lnsl since some platforms (e.g. Solaris)
dnl put it there.  CFITSIO may need it.
AC_SEARCH_LIBS([gethostbyname],[nsl],,)

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
AC_TYPE_SIZE_T
dnl AC_CHECK_SIZEOF(short)
dnl AC_CHECK_SIZEOF([int], [4])
dnl AC_CHECK_SIZEOF([long], [4])
dnl AC_CHECK_SIZEOF(float)
dnl AC_CHECK_SIZEOF(double)

dnl Some systems have different type sizes then what may be expected.
dnl Custom define the desired types.
dnl if test $ac_cv_sizeof_int = 4; then
dnl 	long_type=int
dnl else
dnl 	long_type=long
dnl fi
dnl AC_SUBST([long_type])

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

if test "$ac_cv_cxx_covariant_return_types_supported" = "yes"; then
  AC_DEFINE([MARC_HAS_COVARIANT_RETURN_TYPES])
fi

AC_CACHE_CHECK([if $CXX STL implementation supports quiet NaN constant],
  [ac_cv_STL_quiet_NaN_supported],[
    AC_RUN_IFELSE([AC_LANG_PROGRAM([[
#include <limits>
      ]],
      [[
if (!std::numeric_limits<double>::has_quiet_NaN)
  return -1;
      ]])
    ],
    [
     ac_cv_STL_quiet_NaN_supported=yes
    ],
    [
     ac_cv_STL_quiet_NaN_supported=no
    ])
  ])

if test $ac_cv_STL_quiet_NaN_supported = yes; then
  AC_DEFINE([MARC_HAS_STL_QUIET_NAN_SUPPORT])
else
  AC_CACHE_CHECK([for C library implementation of quiet NaN constant],
  [ac_cv_C_lib_quiet_NaN],[
    for ac_nan in NAN 'nan ("char-sequence")' 'strtod ("NAN", 0)' 'strtod ("NAN(char-sequence)", 0)'; do
      AC_RUN_IFELSE([AC_LANG_PROGRAM([[
#include <cmath>
#include <cstdlib>
        ]],
        [[
if (isnan ($ac_nan) == 0)
  return -1;  // NaN not supported.
        ]])
      ],
      [
       ac_cv_C_lib_quiet_NaN="$ac_nan"
       break
      ],
      [
       ac_cv_C_lib_quiet_NaN=none
      ])
    done
  ])

  if test "$ac_cv_C_lib_quiet_NaN" != none; then
    AC_DEFINE_UNQUOTED([MARC_C_LIB_QUIET_NAN],[$ac_cv_C_lib_quiet_NaN])
  fi
fi

if test "$ac_cv_STL_quiet_NaN_supported" = no &&
   test "$ac_cv_C_lib_quiet_NaN" = none; then
  AC_MSG_ERROR([Unable to determine if NaN constant is supported.])
fi

dnl dnl System specific configuration
dnl case "$target" in
dnl i386-*)
dnl         if test -n "$GXX"; then
dnl                 XCXXFLAGS="$XCXXFLAGS -mtune=i386"
dnl         fi
dnl         ;;
dnl i486-*)
dnl 	if test -n "$GXX"; then
dnl 		XCXXFLAGS="$XCXXFLAGS -mtune=i486"
dnl 	fi
dnl 	;;
dnl i586-*)
dnl 	if test -n "$GXX"; then
dnl 		XCXXFLAGS="$XCXXFLAGS -mtune=i586"
dnl 	fi
dnl 	;;
dnl i686-*)
dnl 	if test -n "$GXX"; then
dnl 		XCXXFLAGS="$XCXXFLAGS -mtune=i686"
dnl 	fi
dnl 	;;
dnl *osf*)
dnl 	if test -z "$GXX"; then
dnl 		XCXXFLAGS="$XCXXFLAGS -define_templates"
dnl 	fi
dnl 	;;
dnl *)
dnl 	dnl nothing
dnl 	;;
dnl esac

AX_CFLAGS_WARN_ALL([CXXFLAGS])
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
  doc/examples/Makefile
  lib/Makefile
  lib/MaRC/Makefile
  src/Makefile
  tests/Makefile
])
AC_OUTPUT

dnl Local Variables:
dnl mode:autoconf
dnl End:
