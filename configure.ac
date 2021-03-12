dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

dnl Copyright 1996-1999, 2003-2004, 2017-2019  Ossama Othman
dnl
dnl SPDX-License-Identifier: GPL-2.0-or-later

dnl An autoconf script to automatically configure MaRC.


AC_PREREQ([2.69])
AC_INIT([MaRC],
        [0.10],
        [https://github.com/ossama-othman/MaRC/issues],
        [marc],
        [https://ossama-othman.github.io/MaRC/])

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
AM_INIT_AUTOMAKE([1.14.1 check-news silent-rules std-options readme-alpha -Wall -Werror -Wno-portability])
AM_SILENT_RULES([yes])
LT_INIT([disable-static])

AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_SRCDIR([lib/marc/PhotoImage.h])

dnl Generate a header file with all settings.
AC_CONFIG_HEADERS([lib/marc/config.h])

dnl Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_17
AC_PROG_LN_S
AX_PROG_BISON
AM_PROG_LEX

AC_LANG([C++])

dnl Check for SIMD CPU instructions.
AX_EXT

MARC_CLANG_TIDY

MARC_CODE_COVERAGE

dnl Check for pkg-config
PKG_PROG_PKG_CONFIG

dnl Check for Doxygen
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

dnl
dnl config.h template
dnl

dnl Text to be placed at the top of the `marc/config.h' header.
AH_TOP([
/**
 * @file config.h
 *
 * MaRC configuration header file.
 *
 * @note This is an internal header, and not meant for use
 *       outside of MaRC.
 */
#ifndef MARC_CONFIG_H
#define MARC_CONFIG_H

])

dnl Text to be placed at the bottom of the `marc/config.h' header.
AH_BOTTOM([
#endif   /* MARC_CONFIG_H */

// Local] [Variables:
// mode:C++
// End:
])


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
dnl AX_CXX_PTHREAD
dnl LIBS="$PTHREAD_LIBS $LIBS"
dnl CXXFLAGS="$CXXFLAGS $PTHREAD_CXXFLAGS"
dnl CXX="$PTHREAD_CXX"

dnl Workaround AX_CXX_PTHREAD's inability to detect the need for
dnl Clang's -Qunused-arguments command line option in some cases, such
dnl as when building a shared library through Libtool.
dnl AS_IF([test "x$ax_pthread_clang" = "xyes"],
dnl       [MARC_UNUSED_ARGUMENT_LDFLAGS="-Xcompiler -Qunused-arguments"
dnl        AC_SUBST([MARC_UNUSED_ARGUMENT_LDFLAGS])])

dnl @todo Switch to std::format once C++20 is supported by MaRC.
AC_CACHE_CHECK([for fmt library >= 6.1.2],
  [marc_cv_lib_fmt],
  [
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#define FMT_HEADER_ONLY
#include <fmt/format.h>

#if FMT_VERSION < 60102
# error "fmt >= 6.1.2 is required"
#endif

void vlog_info(char const * format, fmt::format_args args)
{
    fmt::print("INFO: ");
    fmt::vprint(format, args);
}

template <typename ... Args>
void log_info(char const * format, Args const & ... args)
{
    vlog_info(format, fmt::make_format_args(args...));
}
      ]],
      [[
log_info("{}", "foo");
      ]])
    ],
    [
     marc_cv_lib_fmt=yes
    ],
    [
     marc_cv_lib_fmt=no
    ])
  ])
dnl  ])

AS_IF([test "x$marc_cv_lib_fmt" = "xyes"],
      [],
      [AC_MSG_WARN([fmt library not found, or it is old - using fallback])
       FMTLIB_CPPFLAGS="-I\$(top_srcdir)/opt/fmt-6.1.2/include"
       AC_SUBST([FMTLIB_CPPFLAGS])
      ])

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

       dnl NDEBUG is defined in <marc/config.h> but also define it on
       dnl the command line to avoid having to include <marc/config.h>
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
  lib/marc/Makefile
  lib/marc/marc.pc
  src/Makefile
  tests/Makefile
])
AC_OUTPUT
