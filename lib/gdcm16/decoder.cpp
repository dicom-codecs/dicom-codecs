#include "../../include/dicomcodecs/exception.hpp"
#include "../../include/dicomcodecs/image.hpp"
#include "jpeglib.h"
#include <setjmp.h>
#include <stdio.h>
#include <vector>

using namespace dicomcodecs;
using namespace std;

extern "C"
{
  extern void jpeg_memory_src(j_decompress_ptr cinfo, const JOCTET *buffer,
                              size_t bufsize);
}

extern JSAMPLE *image_buffer; /* Points to large array of R,G,B-order data */
extern int image_height;      /* Number of rows in image */
extern int image_width;       /* Number of columns in image */

struct my_error_mgr
{
  struct jpeg_error_mgr pub; /* "public" fields */

  jmp_buf setjmp_buffer; /* for return to caller */
};

typedef struct my_error_mgr *my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
  printf("my_error_exit\n");
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr)cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message)(cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

void gdcmjpeg16decoder(const vector<uint8_t> &encodedBytes,
                       image &targetImage)
{
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;

  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;
  /* More stuff */
  JSAMPARRAY buffer; /* Output row buffer */
  int row_stride;    /* physical row width in output buffer */

  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer))
  {
    printf("setjmp\n");
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    throw dicom_codec_exception("ijg12", "decode error");
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);
  /* Step 2: specify data source (eg, a file) */

  // jpeg_stdio_src(&cinfo, infile);
  jpeg_memory_src(&cinfo, encodedBytes.data(), encodedBytes.size());

  /* Step 3: read file parameters with jpeg_read_header() */

  (void)jpeg_read_header(&cinfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.txt for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  (void)jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  targetImage.width = cinfo.output_width;
  targetImage.height = cinfo.output_height;
  targetImage.bitsPerSample = cinfo.data_precision;
  targetImage.componentCount = cinfo.num_components;
  targetImage.isSigned = false;

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;

  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE,
                                      row_stride, 1);

  targetImage.rawBytes.resize(cinfo.output_width * cinfo.output_components *
                              cinfo.output_height * sizeof(unsigned short));

  JSAMPROW pBuffer = (JSAMPROW)targetImage.rawBytes.data();
  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
  while (cinfo.output_scanline < cinfo.output_height)
  {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    buffer[0] = pBuffer + (row_stride * cinfo.output_scanline);

    (void)jpeg_read_scanlines(&cinfo, buffer, 1);
  }
  /* Step 7: Finish decompression */

  (void)jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */
}
