#! /bin/sh

set -e

usage() {
    echo `basename $0`: ERROR: $* 1>&2
    echo usage: `basename $0` '[-p marc_program_path] [-t marc_test_dir]' 1>&2
    exit 1
}

# Set $top_builddir default if it was not set in the environment.
if test -z "${top_builddir+set}"; then
  top_builddir=..
fi

# Set $top_srcdir default if it was not set in the environment.
if test -z "${top_srcdir+set}"; then
  top_srcdir=..
fi

marc_program=$top_builddir/src/marc
marc_test_dir=$top_srcdir/tests

while :
do
    case "$1" in
	-p) shift; marc_program="$1";;
	-t) shift; marc_test_dir=$1;;
	--) shift; break;;
	-*) usage "invalid argument $1";;
	*) break;;
    esac
    shift
done

XDG_CONFIG_HOME=$marc_test_dir/config $marc_program $marc_test_dir/test_map.marc
