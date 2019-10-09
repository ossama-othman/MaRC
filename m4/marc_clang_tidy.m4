# Copyright (C) 2019  Ossama Othman
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

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
