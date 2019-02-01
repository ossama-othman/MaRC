#! /bin/sh

# Set $top_builddir default if it was not set in the environment.
if test -z "${top_builddir+set}"; then
  top_builddir=..
fi

# Defined in <sysexits.h>
EX_USAGE=64

marc=$top_builddir/src/marc
# No command line arguments.  Short usage message should be dumped.
$marc > /dev/null 2>&1
test $? -eq $EX_USAGE || exit 1

# Usage message.  Full usage message dumped.
$marc --usage > /dev/null
test $? -eq 0 || exit 1

# Help message.  Full command line argument details dumped.
$marc --help > /dev/null
test $? -eq 0 || exit 1

$marc -? > /dev/null
test $? -eq 0 || exit 1

# Version message.  Dump MaRC program version.
$marc --version > /dev/null
test $? -eq 0 || exit 1

$marc -V > /dev/null
test $? -eq 0 || exit 1

# Explictly mark end of command line options.
$marc -- > /dev/null 2>&1
test $? -eq $EX_USAGE || exit 1

$marc -- foo > /dev/null 2>&1
test $? -ne 0 || exit 1
