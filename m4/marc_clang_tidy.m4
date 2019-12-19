# Copyright (C) 2019  Ossama Othman
#
# SPDX-License-Identifier: LGPL-2.1-or-later

#serial 1

m4_define(_MARC_CLANG_TIDY_RULES,[
  AX_ADD_AM_MACRO_STATIC([
clang-tidy-local:
	@if test -n \"\$(CLANG_TIDY)\" && test -n \"\$(SOURCES)\"; then \
	  \$(CLANG_TIDY) -header-filter=.* -checks=-*,bugprone-*,clang-analyzer-*,modernize-*,performance-* \`echo \$(SOURCES) | sed -e 's/[^ ]*\.\(ll\|yy\)//g'\` -- \$(DEFS) \$(DEFAULT_INCLUDES) \$(INCLUDES) \$(AM_CPPFLAGS) \$(CPPFLAGS); \
	fi
])
])

# MARC_CLANG_TIDY
#
# Add support for the clang-tidy linter.  This adds a recursive
# "clang-tidy" target to the Makefiles (e.g. "make clang-tidy") that
# runs the `clang-tidy' program on any source files tracked by
# GNU Automake generated Makefiles.
AC_DEFUN([MARC_CLANG_TIDY],[
  AC_ARG_VAR([CLANG_TIDY],[path for clang-tidy program])
  AC_CHECK_PROGS([CLANG_TIDY], [clang-tidy])

  _MARC_CLANG_TIDY_RULES

  AM_EXTRA_RECURSIVE_TARGETS([clang-tidy])
])
