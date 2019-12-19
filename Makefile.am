## Copyright (C) 1996-1999, 2017  Ossama Othman
##
## SPDX-License-Identifier: GPL-2.0-or-later

##  Process this file with automake to create Makefile.in

SUBDIRS = doc lib src tests

ACLOCAL_AMFLAGS = -I m4

## Doxygen
@DX_RULES@
MOSTLYCLEANFILES = $(DX_CLEANFILES)

README: $(top_srcdir)/README.md
	@test -z "$(PANDOC)" || $(PANDOC) --from gfm --to plain --output=$@ $<

dist-hook: README

MAINTAINERCLEANFILES = README

EXTRA_DIST =		\
	FAQ		\
	BUGS		\
	ChangeLog.0	\
	Doxyfile	\
	marc.dox	\
	CODE_OF_CONDUCT.md \
	README.md       \
	opt

include $(top_srcdir)/aminclude_static.am

clean-local: code-coverage-clean
