# dicom-codecs
dicom-codecs

C++ library for encoding and decoding all dicom transfer syntaxes

## Status

WIP - do not use yet

Implemented Codecs:
* OpenJPEG - JPEG 2000 (1.2.840.10008.1.2.4.90, 1.2.840.10008.1.2.4.91)
* CharLS - JPEG-LS (1.2.840.10008.1.2.4.80, 1.2.840.10008.1.2.4.81)
* libjpeg-turb - JPEG 8 bit lossy (1.2.840.10008.1.2.4.50)
* ijg - JPEG Baseline (Processes 2 & 4 - 12 bit) (1.2.840.10008.1.2.4.51 ) )

TODO:
* Add codec ?? for RLE Lossless (1.2.840.10008.1.2.5 RLE Lossless)
* Add codec ?? for JPEG Lossless, Nonhierarchical (Processes 14) (1.2.840.10008.1.2.4.57)
* Add codec ?? for JPEG Lossless, Nonhierarchical (Processes 14 [Selection 1]) (1.2.840.10008.1.2.4.70)
* Add codec openjph for HTJ2K decoding
* Add codec libjxl for JPEG-XL decoding
* Add codec Kakadujs for J2K/HTJ2K decoding
* Add support for codec specific options (e.g. JPEG quality, J2K progression order, etc)
* throw dicomcodecs::exception for all failure cases
* refactor encoder and decoder from ijg/lib.cpp into separate files
* refactor ijg so CMakelists.txt is ourside of src directory (maybe in a separate repo via extern, or a /lib folder)

## Building

This project uses git submodules to pull in external codec linbraries, initialize them first:

> git submodule update --init --recursive

To build:

> ./build.sh

## Development

To update external dependencies to latest versions:

> git submodule update --remote --merge
