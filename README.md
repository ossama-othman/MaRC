[![Build Status](https://travis-ci.org/ossama-othman/MaRC.svg?branch=master)](https://travis-ci.org/ossama-othman/MaRC)

# MaRC - Map Reprojections and Conversions #

## Dependencies ##

### Build Dependencies ###
* C++ compiler
  * The C++ compiler should support the C++14 standard or better.

* CFITSIO Library
  * Often available as prepackaged binaries on various platforms.
 Additional details are available on the  [CFITSIO homepage](https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html).

* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)

### Maintenance Dependencies ###

* GNU Autoconf
* GNU Automake
* GNU Libtool
* GNU Flex >= 2.54a
* GNU Bison >= 3.0.4
* GNU Autoconf Archive
* Doxygen

## Quick Start ##
Type:
        ./configure
        make

The MaRC binary will reside in the "src" subdirectory.  Documentation will be
in the "doc" subdirectory if your system has the necessary text utilities to
build them.

## Installation ##
   (REQUIRES SUPER USER PRIVILEGES IF INSTALLING IN SYSTEM DIR)
Type:  make install

## Bugs / Features ##
...

Contact the maintainer at the below address if problems arise.
__________________________________________________
Ossama Othman <othman@users.sourceforge.net>
