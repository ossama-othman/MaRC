dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

dnl Copyright 1996-1999, 2003-2004, 2017-2018  Ossama Othman
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
AC_INIT([MaRC], [0.9.10], [othman@users.sourceforge.net], [marc])

dnl Additional configure script arguments
AX_IS_RELEASE([git-directory])
AX_CHECK_ENABLE_DEBUG([yes])

AC_ARG_ENABLE([arch-detect],
     [AS_HELP_STRING([--enable-arch-detect],
                     [enable detection of GCC CPU architecture tuning flags (also see related "--with-gcc-arch" option)])],
     [],
     [enable_arch_detect=no])

dnl AC_ARG_WITH([spdlog],
dnl      [AS_HELP_STRING([--with-spdlog],
dnl                      [support improved logging through the spdlog library @<:@default=check@:>@])],
dnl      [],
dnl      [with_spdlog=check])

dnl Check what platform we are running on.
dnl AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.14.1 check-news silent-rules std-options -Wall -Werror -Wno-portability])
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

AX_CODE_COVERAGE

# Check for pkg-config
PKG_PROG_PKG_CONFIG

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

AC_ARG_VAR([PANDOC],[location of pandoc program])
AC_PATH_PROG([PANDOC], [pandoc])

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


AH_TEMPLATE([MARC_HAS_3_PARAM_STD_HYPOT],[C++ compiler supports C++17 three-parameter std::hypot().])

dnl Checks for libraries.

dnl We're of course going to need the C math library.
AC_SEARCH_LIBS([cos],[m])

MARC_SAVE_LIBS=$LIBS
ARGP_LIBS=
AC_SEARCH_LIBS([argp_parse],
               [argp],
	       [AC_DEFINE([HAVE_ARGP], [1], [Define if you have Argp])
	        dnl Prevent the Argp library from being link to all
		dnl MaRC binaries.  It is only used by the MaRC
		dnl program.
	        AS_IF([test "x$ac_cv_search_argp_parse" != "xno" \
		       -a "x$ac_cv_search_argp_parse" != "xnone required"],
		       [ARGP_LIBS="$ac_cv_search_argp_parse"
		        LIBS="$MARC_SAVE_LIBS"
		        AC_SUBST([ARGP_LIBS])
		       ])
	       ],
	       [dnl BSD-style EX_* exit codes.
                AC_CHECK_HEADERS([sysexits.h])
               ])

PKG_CHECK_MODULES([CFITSIO], [cfitsio])
AC_SUBST([CFITSIO_LIBS])
AC_SUBST([CFITSIO_CFLAGS])

dnl spdlog requires POSIX threads support.
AX_CXX_PTHREAD
LIBS="$PTHREAD_LIBS $LIBS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
CXX="$PTHREAD_CXX"

AC_CACHE_CHECK([for spdlog library],
  [marc_cv_lib_spdlog],
  [
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <spdlog/spdlog.h>
      ]],
      [[
auto logger = spdlog::stdout_color_mt("marc");

logger->info("foo");
      ]])
    ],
    [
     marc_cv_lib_spdlog=yes
    ],
    [
     marc_cv_lib_spdlog=no
    ])
  ])
dnl  ])

AS_IF([test "x$marc_cv_lib_spdlog" = "xyes"],
      [AC_DEFINE([HAVE_SPDLOG], [1], [Define if you have spdlog.])],
      [AC_MSG_FAILURE([spdlog library is required])])

dnl Check for C++17 three-parameter std::hypot() function.
AC_CACHE_CHECK([if $CXX supports C++17 three-parameter std::hypot()],
  [marc_cv_cxx_3_param_std_hypot_supported],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <cmath>
      ]],
      [[
double h = std::hypot(3, 4, 5);
      ]])
    ],
    [
     marc_cv_cxx_3_param_std_hypot_supported=yes
    ],
    [
     marc_cv_cxx_3_param_std_hypot_supported=no
    ])
  ])

AS_IF([test "x$marc_cv_cxx_3_param_std_hypot_supported" = "xyes"],
      [AC_DEFINE([MARC_HAS_3_PARAM_STD_HYPOT])])

dnl Checks for header files.
AC_HEADER_STDC

dnl System specific configuration
AS_IF([test "x$ax_enable_debug" != "xyes"],
      [
       dnl Compile-time optimization
       marc_optimization_flag="-O2"
       AX_CHECK_COMPILE_FLAG([$marc_optimization_flag],
                             [AX_APPEND_FLAG([$marc_optimization_flag],
                                             [XCXXFLAGS])])

       dnl Link-time optimization
       marc_lto_flag="-flto"
       AX_CHECK_COMPILE_FLAG([$marc_lto_flag],
                             [AX_APPEND_FLAG([$marc_lto_flag],
                                             [XCXXFLAGS])])

       dnl NDEBUG is defined in <MaRC/config.h> but also define it on
       dnl the command line to avoid having to include <MaRC/config.h>
       dnl in MaRC headers or template code.
       dnl AX_APPEND_FLAG([-DNDEBUG], [XCPPFLAGS])
      ])

AS_IF([test "x$enable_arch_detect" = "xyes"],
      [AX_GCC_ARCHFLAG([yes],
                        [AX_APPEND_FLAG([$ax_cv_gcc_archflag],
                                        [XCXXFLAGS])])
      ])

AX_CXXFLAGS_WARN_ALL([CXXFLAGS])
AS_IF([test "x$GXX" = "xyes"],
      [XCXXFLAGS="$XCXXFLAGS -Wextra -Wformat=2 -pedantic"],
      [])

AX_APPEND_COMPILE_FLAGS([-fvisibility=hidden -fvisibility-inlines-hidden])

dnl Export the public API from the MaRC shared library.
AM_CONDITIONAL([BUILDING_DLL], [test "x$enable_shared" = xyes])

dnl Combine package set flags with user's flags.
dnl User's flags go after package flags to allow user to override
dnl package defaults.
CXXFLAGS="$XCXXFLAGS $CXXFLAGS"
CPPFLAGS="$XCPPFLAGS $CPPFLAGS"
LDFLAGS="$XLDFLAGS $LDFLAGS"

AC_CONFIG_FILES([
  Makefile
  doc/Makefile
  lib/Makefile
  lib/MaRC/Makefile
  src/Makefile
  tests/Makefile
])
AC_OUTPUT
