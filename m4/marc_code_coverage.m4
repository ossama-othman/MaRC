# Copyright (C) 2018  Ossama Othman
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

# MARC_CODE_COVERAGE
#
# Add support for gcov/lcov based code coverage instrumentation and
# result generation.
AC_DEFUN([MARC_CODE_COVERAGE],[
  AC_REQUIRE([AX_CODE_COVERAGE])

  # The AX_CODE_COVERAGE macro changed how it should be used.
  # Previously, users had to add @CODE_COVERAGE_RULES@ to each
  # Makefile.am file that listed code to be instrumented for code
  # coverage.  However, recent versions of that macro changed that to
  # instead require users to add:
  #     include $(top_srcdir)/aminclude_static.am
  # to their Makefile.am files.  Work around that break in backward
  # compatibility by explicitly adding the @CODE_COVERAGE_RULES@ to
  # the `aminclude_static.am' file, and clearing out that substitution
  # variable if it exists.  It will only exist if the older
  # AX_CODE_COVERAGE macro is in use.  Otherwise it becomes a no-op.
  AX_ADD_AM_MACRO_STATIC([@CODE_COVERAGE_RULES@])
  AC_CONFIG_COMMANDS_PRE([AC_SUBST([CODE_COVERAGE_RULES])])
])
