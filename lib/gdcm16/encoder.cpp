#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include "jpeglib.h"
#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>
#include <vector>

using namespace dicomcodecs;
using namespace std;

extern "C" {

GLOBAL(void)
jpeg_mem_dest(j_compress_ptr cinfo, unsigned char **outbuffer, size_t *outsize);
}

void gdcmjpeg16encoder(const image &sourceImage,
                       vector<uint8_t> &encodedBytes) {
  int quality = 100;

  /* This struct contains the JPEG compression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   * It is possible to have several such structures, representing multiple
   * compression/decompression processes, in existence at once.  We refer
   * to any one struct (and its associated working data) as a "JPEG object".
   */
  struct jpeg_compress_struct cinfo;
  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * (see the second half of this file for an example).  But here we just
   * take the easy way out and use the standard error handler, which will
   * print a message on stderr and call exit() if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  // FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
  int row_stride;          /* physical row width in image buffer */

  /* Step 1: allocate and initialize JPEG compression object */

  /* We have to set up the error handler first, in case the initialization
   * step fails.  (Unlikely, but it could happen if you are out of memory.)
   * This routine fills in the contents of struct jerr, and returns jerr's
   * address which we place into the link field in cinfo.
   */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
   * stdio stream.  You can also write your own code to do something else.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to write binary files.
   */
  // if ((outfile = fopen(filename, "wb")) == NULL) {
  //  fprintf(stderr, "can't open %s\n", filename);
  //  exit(1);
  //}
  // jpeg_stdio_dest(&cinfo, outfile);
  size_t maxBufferSize =
      sourceImage.width * sourceImage.height * sourceImage.componentCount * 2;
  encodedBytes.resize(maxBufferSize);
  size_t outsize = encodedBytes.size();
  unsigned char *pData = encodedBytes.data();
  jpeg_mem_dest(&cinfo, &pData, &outsize);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = sourceImage.width; /* image width and height, in pixels */
  cinfo.image_height = sourceImage.height;
  cinfo.input_components =
      sourceImage.componentCount; /* # of color components per pixel */
  if (sourceImage.componentCount == 1) {
    cinfo.in_color_space = JCS_GRAYSCALE; /* colorspace of input image */
  } else {
    cinfo.in_color_space = JCS_RGB; /* colorspace of input image */
  }
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
  // predictor = 5 (hard coded for now but can be 1-7)
  // point_transform = 0 (NOTE: must be 0 to be lossless!)
  jpeg_simple_lossless(&cinfo, 5, 0);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  //
  std::vector<unsigned short> rawDataAsShorts;
  unsigned short *pIn;
  if (sourceImage.bitsPerSample <= 8) {
    size_t numPixels =
        sourceImage.height * sourceImage.width * sourceImage.componentCount;
    rawDataAsShorts.resize(numPixels);
    for (int i = 0; i < numPixels; i++) {
      rawDataAsShorts[i] = sourceImage.rawBytes[i];
    }
    pIn = rawDataAsShorts.data();
  } else {
    pIn = (unsigned short *)sourceImage.rawBytes.data();
  }

  row_stride = sourceImage.width * sourceImage.componentCount;

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = (pIn + (cinfo.next_scanline * row_stride));
    (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  // fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  // set the encoded bytes size
  encodedBytes.resize(outsize);

  /* And we're done! */
}
