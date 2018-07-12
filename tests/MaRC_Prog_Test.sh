#! /bin/sh

set -e

# Set $top_builddir default if it was not set in the environment.
if test -z "${top_builddir+set}"; then
  top_builddir=..
fi

# Set $top_srcdir default if it was not set in the environment.
if test -z "${top_srcdir+set}"; then
  top_srcdir=..
fi


XDG_CONFIG_HOME=$top_srcdir/tests/config \
    $top_builddir/src/marc $top_srcdir/tests/test_map.marc
