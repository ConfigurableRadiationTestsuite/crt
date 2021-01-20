# General Information

This is a configurable testsuite fitted for radiation testing (but can be used for other stuff)

# Documentation

The user manual can be found in the 'doc' folder

# Build Prerequisites

## QTCreator

Obviously for a qt-project you need the qtcreator package

## Labjack

To install the Labjack library download the [LJM Software](https://labjack.com/support/software/installers/ljm)

## Liblxi

### Debian Linux

The lxi library is in the official paket store:

```
$sudo apt-get install liblxi-dev
```

### Arch Linux

The lxi library for measurement devices is not officially supported on arch-linux, therefore it needs to be build manually. First install necessary packages like:

- autoconf
- autoreconf
- libtirpc
- rpcsvc-proto
- libxml2

and then get the git package and build it:

```
$git clone https://github.com/lxi-tools/liblxi
$cd liblxi
$./autogen.sh
$./configure
$make
$sudo make install
```

**IMPORTANT:** So far the lxi library seems to be not completly working on arch linux **!**
