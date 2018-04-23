[![Build Status](https://travis-ci.org/ossama-othman/MaRC.svg?branch=master)](https://travis-ci.org/ossama-othman/MaRC)
[![Coverity Status](https://scan.coverity.com/projects/13233/badge.svg)](https://scan.coverity.com/projects/ossama-othman-marc)
[![Coverage Status](https://coveralls.io/repos/github/ossama-othman/MaRC/badge.svg?branch=master)](https://coveralls.io/github/ossama-othman/MaRC?branch=master)

# MaRC

MaRC - __*Ma*__p __*R*__eprojections and __*C*__onversions - is a
program and library designed to produce map projections from digital
images or from computed values of incidence, emission or phase angles,
and latitudes and longitudes.  Multi-plane maps can be created with
digital image data in several planes and incidence, emission, phase
angles, latitudes and longitudes in other planes.  Placement of data
in a given plane is completely arbitrary.  The user decides in what
plane to place a given set of data.  Created maps are stored in [FITS](https://fits.gsfc.nasa.gov/fits_standard.html)
(Flexible Image Transport System) format.

## Getting Started

MaRC may either be built from a source tarball or from a clone of its
source repository on GitHub.  If you're an end-user that just wants to
run MaRC, building from the self-contained source tarball will get you
up and running sooner.

### Building From the Source Tarball

The quickest way to get started with MaRC is to download the
self-contained source tarball (e.g. `marc-0.9.10.tar.gz`) from the
[releases page](https://github.com/ossama-othman/MaRC/releases).
Other than required third party programs and libraries it contains
everything needed to build and install MaRC.  Once the prerequisites
are installed, simply build MaRC as follows:

```sh
tar xf marc-0.9.10.tar.gz
cd marc-0.9.10
./configure
make
```

The MaRC binary will reside in the "src" subdirectory.  Documentation
will be in the "doc" subdirectory if your system has the necessary
text utilities to build them.

Additional build details and instructions may be found in the
`INSTALL` file shipped with the self-contained source tarball, as well
as by passing the `--help` command line option to the `configure`
script.

### Building From Git
If you plan on contributing to MaRC or want the latest unreleased
code, it is best to clone MaRC from its GitHub repository.  However,
you will need to satisfy additional build prerequisites.  See the
Prerequisites section below for additional details.  Once all
prerequisites have been satisfied, clone and build MaRC as follows:

```sh
git clone git@github.com:ossama-othman/MaRC.git
cd MaRC
./bootstrap
./configure
make
```

### Prerequisites

MaRC is currently only supported on Unix-like platforms, and has the
following build dependencies:

* C++ compiler
  * The C++ compiler should support the C++14 standard or better.
    MaRC is known to build with g++ and clang++, but it should work
    other C++14 conforming compilers.

* CFITSIO Library
  * Often available as prepackaged binaries on various platforms.
    Additional details are available on the [CFITSIO
    homepage](https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html).

* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)

#### Maintenance and Git Based Build Dependencies

Additional dependencies exist when building MaRC from Git or when
performing maintainer-like modifications to the MaRC package as a whole:

* GNU Autotools
  * [Autoconf](https://www.gnu.org/software/autoconf/)
  * [Autoconf Archive](https://www.gnu.org/software/autoconf-archive/)
  * [Automake](https://www.gnu.org/software/automake/)
  * [Libtool](https://www.gnu.org/software/libtool/)
* Text Parser and Scanner Generators
  * [GNU Bison](https://www.gnu.org/software/bison/)
  * [Flex](https://github.com/westes/flex)
* Documentation Generators
  * [Doxygen](http://www.doxygen.org/)
  * [GNU Texinfo](https://www.gnu.org/software/texinfo/)

#### Platform-specific Dependencies
* macOS
  * The easiest way to get MaRC up and running on macOS is to install
    the required software listed above through the
    [Homebrew](https://brew.sh/) package manager.

### Installing

MaRC's build infrastructure supports the usual GNU Autotool provided
build and install targets, including the `install` target.  To install
MaRC, run `make` as follows:

```sh
make install
```

For example, on Linux platforms where the default installation prefix
is `/usr/local` MaRC file installation locations could be the
following:

File | Location
---- | --------
`marc` | `/usr/local/bin`
`libmarc` | `/usr/local/lib`
MaRC headers | `/usr/local/include/MaRC`
documentation | `/usr/local/share/marc`
man pages | `/usr/local/share/man/`

Further details are available in the `INSTALL` file.

## Running the tests

MaRC's test suite may be run from the top-level source directory after
the `configure` script has been run like so:

```sh
make check
```

## Contributing

Please ensure you adhere to [MaRC's code of
conduct](CODE_OF_CONDUCT.md) when contributing to MaRC.

<!--
## Versioning
[SemVer](http://semver.org/)
-->

## Bug Reporting and Feature Requests
Please submit bug reports and feature requests through the [MaRC
issues](https://github.com/ossama-othman/MaRC/issues) page.

## License

MaRC is licensed under several open-source licenses:

* MaRC program: [GPLv2](../COPYING)
* MaRC library: [LGPLv2.1](../COPYING.LESSER)
* MaRC documentation: [GFDLv1.3](../COPYING.DOC)

## Acknowledgments

See the [THANKS](../THANKS) file for a list of people who contributed
significantly to MaRC.

<!-- Original layout based on Billie Thompson's (PurpleBooth)
     README-template.md at
     https://gist.github.com/PurpleBooth/109311bb0361f32d87a2 -->
