# dicom-codecs
dicom-codecs

C++ library for encoding and decoding all dicom transfer syntaxes

## Status

WIP - do not use yet

Implemented Codecs:
* OpenJPEG - JPEG 2000 (decode only)
* CharLS - JPEG-LS (decode only)
* libjpeg-turb - JPEG 8 bit lossy (1.2.840.10008.1.2.4.50)

TODO:
* RLE Lossless (1.2.840.10008.1.2.5 RLE Lossless) - from ??
* JPEG Baseline (Processes 2 & 4 - 12 bit) (1.2.840.10008.1.2.4.51 ) - from IJG
* JPEG Lossless, Nonhierarchical (Processes 14) (1.2.840.10008.1.2.4.57) - from ??
* JPEG Lossless, Nonhierarchical (Processes 14 [Selection 1]) (1.2.840.10008.1.2.4.70) - from ??

## Building

This project uses git submodules to pull in external codec linbraries, initialize them first:

> git submodule update --init --recursive

To build:

> ./build.sh

## Development

To update external dependencies to latest versions:

> git submodule update --remote --merge
