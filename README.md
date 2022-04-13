# dicom-codecs
dicom-codecs

C++ library for encoding and decoding all dicom transfer syntaxes

## Status

WIP - do not use yet

Implemented Codecs:
* OpenJPEG - JPEG 2000 Image Compression (Lossless Only) (1.2.840.10008.1.2.4.90, 1.2.840.10008.1.2.4.90)
* OpenJPEG - JPEG 2000 Image Compression (1.2.840.10008.1.2.4.90, 1.2.840.10008.1.2.4.91)
* CharLS - JPEG-LS Lossless Image Compression (1.2.840.10008.1.2.4.80, 1.2.840.10008.1.2.4.80)
* CharLS - JPEG-LS Lossy (Near- Lossless) Image Compression (1.2.840.10008.1.2.4.80, 1.2.840.10008.1.2.4.81)
* libjpeg-turb - JPEG Baseline (Process 1): Default Transfer Syntax for Lossy JPEG 8-bit Image Compression (1.2.840.10008.1.2.4.50)
* ijg - JPEG Baseline (Processes 2 & 4): Default Transfer Syntax for Lossy JPEG 12-bit Image Compression (Process 4 only) (Processes 2 & 4 - 12 bit) (1.2.840.10008.1.2.4.51 ) )
* RLE Lossless (1.2.840.10008.1.2.5 RLE Lossless)
* ljpeg-6b - JPEG Lossless, Nonhierarchical (Processes 14) (1.2.840.10008.1.2.4.57)
* ljpeg-6b - JPEG Lossless, Nonhierarchical (Processes 14 [Selection 1]) (1.2.840.10008.1.2.4.70)


TODO:
* Add codec openjph for HTJ2K
* Add codec libjxl for JPEG-XL
* Add codec Kakadujs for J2K/HTJ2K
* Add codec GROK for J2K/HTJ2K
* Add support for codec specific options (e.g. JPEG quality, J2K progression order, etc)
* throw dicomcodecs::exception for all failure cases
* Add support for getting the codec version
* Add support for codecs registering the transfersyntaxes they can decode/encode


## Building

This project uses git submodules to pull in external codec linbraries, initialize them first:

> git submodule update --init --recursive

To build:

> ./build.sh

## Development

To update external dependencies to latest versions:

> git submodule update --remote --merge
