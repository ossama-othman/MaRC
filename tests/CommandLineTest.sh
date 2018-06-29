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

marc=$top_builddir/src/marc
# No command line arguments.  Short usage message should be dumped.
$marc

# Usage message.  Full usage message dumped.
$marc --usage

# Help message.  Full command line argument details dumped.
$marc --help
$marc -?

# Version message.  Dump MaRC program version.
$marc --version
$marc -V

# Explictly mark end of command line options.
$marc --
$marc -- foo
