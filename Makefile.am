## Copyright (C) 1996-1999, 2017  Ossama Othman
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

##  Process this file with automake to create Makefile.in

SUBDIRS = doc lib src tests

ACLOCAL_AMFLAGS = -I m4

## Doxygen
@DX_RULES@
MOSTLYCLEANFILES = $(DX_CLEANFILES)

README: $(top_srcdir)/README.md
	@test -z "$(PANDOC)" || $(PANDOC) --from markdown_github --to plain --output=$@ --normalize $<

dist-hook: README

MAINTAINERCLEANFILES = README

EXTRA_DIST =		\
	FAQ		\
	BUGS		\
	ChangeLog.0	\
	Doxyfile	\
	marc.dox	\
	CODE_OF_CONDUCT.md \
	README.md \
	external/

include $(top_srcdir)/aminclude_static.am

clean-local: code-coverage-clean
dist-clean-local: code-coverage-dist-clean
