## Copyright (C) 2021  Ossama Othman
##
## SPDX-License-Identifier: GPL-2.0-or-later

##  Process this file with automake to create Makefile.in

if WITH_NCURSES
  MAYBE_PROGRESSBAR = progressbar
endif
SUBDIRS = $(MAYBE_PROGRESSBAR)

EXTRA_DIST = fmt-6.1.2

include $(top_srcdir)/aminclude_static.am

clean-local: code-coverage-clean
