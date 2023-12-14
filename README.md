# dicom-codecs

dicom-codecs

C++ library for encoding and decoding all dicom transfer syntaxes

## Status

WIP - do not use yet

## Transfer Syntaxes to codec mapping

### Lossless

- 1.2.840.10008.1.2.4.57 - JPEG Lossless
  - gdcm-jpeg16
- 1.2.840.10008.1.2.4.70 - JPEG Lossless First Order
  - gdcm-jpeg16
- 1.2.840.10008.1.2.5 - RLE (Lossless)
  - rle
- 1.2.840.10008.1.2.4.80 - JPEG-LS (Lossless)
  - charls
- 1.2.840.10008.1.2.4.90 - JPEG 2000 (Lossless)
  - kakadu
  - openjpeg
- 1.2.840.10008.1.2.4.201 - HTJ2K (Lossless)
  - kakadu
  - openjph
- 1.2.840.10008.1.2.4.202 - HTJ2KLosslessRPCL (Lossless)
  - kakadu
  - openjph

### Lossy

- 1.2.840.10008.1.2.4.50 - JPEG Baseline (Process 1): Default Transfer Syntax for Lossy JPEG 8-bit Image Compression
  - libjpeg-turbo8
- 1.2.840.10008.1.2.4.51 - JPEG Extended (12 bit)
  - ijg12
  - ljpeg6b16
- 1.2.840.10008.1.2.4.81 - JPEG-LS (Lossy)
  - charls
- 1.2.840.10008.1.2.4.91 - JPEG 2000 (Lossy)
  - kakadu
  - openjpeg
- 1.2.840.10008.1.2.4.203 - HTJ2KLosslessRPCL (Lossy)
  - kakadu
  - openjph

###

- ljpeg6b16
  - JPEG Lossless, Nonhierarchical (Processes 14) (1.2.840.10008.1.2.4.57)

## Benchmark Results

Apple MacBook Pro M1 Pro:

```
** Testing all codecs 8 bit color US**
Decode libjpeg-turbo8 with - average time of 4.06 ms(1 iterations)
Decode ljpeg6b16 with - average time of 5.96 ms(1 iterations)
Decode ijg12 with - average time of 15.11 ms(1 iterations)
Decode gdcm-jpeg16 with - average time of 6.29 ms(1 iterations)
Decode charls with - average time of 7.46 ms(1 iterations)
Decode openjpeg with - average time of 24.73 ms(1 iterations)
Decode openjph with - average time of 4.09 ms(1 iterations)
Decode rle with - average time of 0.19 ms(1 iterations)
Decode kakadu with - average time of 1.13 ms(1 iterations)
** Testing all codecs 12 bit gray MR**
Decode charls with - average time of 3.14 ms(1 iterations)
Decode gdcm-jpeg16 with - average time of 2.12 ms(1 iterations)
Decode openjpeg with - average time of 15.65 ms(1 iterations)
Decode openjph with - average time of 1.43 ms(1 iterations)
Decode rle with - average time of 0.32 ms(1 iterations)
Decode kakadu with - average time of 0.51 ms(1 iterations)
** Testing all codecs 16 bit gray CT**
Decode openjpeg with - average time of 22.27 ms(1 iterations)
Decode openjph with - average time of 1.53 ms(1 iterations)
Decode rle with - average time of 0.33 ms(1 iterations)
Decode kakadu with - average time of 0.64 ms(1 iterations)
Decode charls with - average time of 3.46 ms(1 iterations)
** Testing all codecs 12 bit gray MG**
Decode gdcm-jpeg16 with - average time of 142.10 ms(1 iterations)
Decode charls with - average time of 212.92 ms(1 iterations)
Decode openjpeg with - average time of 1447.63 ms(1 iterations)
Decode openjph with - average time of 89.20 ms(1 iterations)
Decode rle with - average time of 21.35 ms(1 iterations)
Decode kakadu with - average time of 29.36 ms(1 iterations)
```

TODO:

- Add codec libjxl for JPEG-XL
- Add codec GROK for J2K/HTJ2K
- Add support for codec specific options (e.g. JPEG quality, J2K progression order, etc)
- throw dicomcodecs::exception for all failure cases
- Add support for getting the codec version
- Add support for codecs registering the transfersyntaxes they can decode/encode

## Building

This project uses git submodules to pull in external codec linbraries, initialize them first:

> git submodule update --init --recursive

To build:

> ./build.sh

## Development

To update external dependencies to latest versions:

> git submodule update --remote --merge
