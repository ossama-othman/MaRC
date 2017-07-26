##  Copyright 1996-1997, 2004, 2017  Ossama Othman
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
## 02110-1301  USA

##  Process this file with automake to produce Makefile.in

dist_man_MANS = marc.1

info_TEXINFOS = MaRC_input.texi
MaRC_input_TEXINFOS = fdl.texi

docdir = $(prefix)/share/doc/$(PACKAGE)
dist_doc_DATA = MaRC_input.pdf

exampledir = $(prefix)/share/doc/$(PACKAGE)/examples
dist_example_DATA = \
	examples/dot.marc \
	examples/101.inp \
	examples/mumu0.test.inp

all-local: pdf # ps html

#dist_noinst_DATA = MaRC_input.texi
