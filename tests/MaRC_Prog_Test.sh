#! /bin/sh

# $Id: MaRC_Prog_Test.sh,v 1.2 2004/11/08 19:05:08 othman Exp $

set -e

# Set $top_builddir default if it was not set in the environment.
if test -z "${top_builddir+set}"; then
  top_builddir=..
fi

# Set $top_srcdir default if it was not set in the environment.
if test -z "${top_srcdir+set}"; then
  top_srcdir=..
fi

$top_builddir/src/marc $top_srcdir/tests/test_map.inp > /dev/null 2>&1
