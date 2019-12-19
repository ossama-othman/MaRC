# Copyright (C) 2018  Ossama Othman
#
# SPDX-License-Identifier: LGPL-2.1-or-later

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
