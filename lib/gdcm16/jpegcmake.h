/* jpegcmake.h file generated by CMake http://www.cmake.org */

#ifndef __jpegcmake_h
#define __jpegcmake_h

/* See jconfig.h */
/* For all plateforms: */
#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT

#define BITS_IN_JSAMPLE 16

//#cmakedefine HAVE_STDDEF_H
//#cmakedefine HAVE_STDLIB_H

// extern void jpeg_memory_src(j_decompress_ptr cinfo, const JOCTET *buffer, size_t bufsize);

/* These macros are used in all function definitions and extern declarations.
 * You could modify them if you need to change function linkage conventions;
 * in particular, you'll need to do that to make the library a Windows DLL.
 * Another application is to make all functions global for use with debuggers
 * or code profilers that require it.
 */

#define BITS_IN_JSAMPLE 16

/*
 * This header file mangles all symbols exported from the jpeg library.
 * It is included in all files while building the jpeg library.  Due to
 * namespace pollution, no jpeg headers should be included in .h files in
 * GDCM. This also allows us to generate two jpeg library (8bits and 12bits).
 *
 * The following command was used to obtain the symbol list:
 *
 * nm libGDCMJPEG16.a | grep " [R|T] " | colrm 1 11 | sort
 */

#define jcopy_block_row GDCMJPEG16_jcopy_block_row
#define jcopy_sample_rows GDCMJPEG16_jcopy_sample_rows
#define jdiv_round_up GDCMJPEG16_jdiv_round_up
#define jinit_1pass_quantizer GDCMJPEG16_jinit_1pass_quantizer
#define jinit_2pass_quantizer GDCMJPEG16_jinit_2pass_quantizer
#define jinit_arith_decoder GDCMJPEG16_jinit_arith_decoder
#define jinit_arith_encoder GDCMJPEG16_jinit_arith_encoder
#define jinit_c_codec GDCMJPEG16_jinit_c_codec
#define jinit_c_coef_controller GDCMJPEG16_jinit_c_coef_controller
#define jinit_c_diff_controller GDCMJPEG16_jinit_c_diff_controller
#define jinit_c_main_controller GDCMJPEG16_jinit_c_main_controller
#define jinit_c_master_control GDCMJPEG16_jinit_c_master_control
#define jinit_c_prep_controller GDCMJPEG16_jinit_c_prep_controller
#define jinit_c_scaler GDCMJPEG16_jinit_c_scaler
#define jinit_color_converter GDCMJPEG16_jinit_color_converter
#define jinit_color_deconverter GDCMJPEG16_jinit_color_deconverter
#define jinit_compress_master GDCMJPEG16_jinit_compress_master
#define jinit_d_codec GDCMJPEG16_jinit_d_codec
#define jinit_d_coef_controller GDCMJPEG16_jinit_d_coef_controller
#define jinit_d_diff_controller GDCMJPEG16_jinit_d_diff_controller
#define jinit_d_main_controller GDCMJPEG16_jinit_d_main_controller
#define jinit_d_post_controller GDCMJPEG16_jinit_d_post_controller
#define jinit_d_scaler GDCMJPEG16_jinit_d_scaler
#define jinit_differencer GDCMJPEG16_jinit_differencer
#define jinit_downsampler GDCMJPEG16_jinit_downsampler
#define jinit_forward_dct GDCMJPEG16_jinit_forward_dct
#define jinit_input_controller GDCMJPEG16_jinit_input_controller
#define jinit_inverse_dct GDCMJPEG16_jinit_inverse_dct
#define jinit_lhuff_decoder GDCMJPEG16_jinit_lhuff_decoder
#define jinit_lhuff_encoder GDCMJPEG16_jinit_lhuff_encoder
#define jinit_lossless_c_codec GDCMJPEG16_jinit_lossless_c_codec
#define jinit_lossless_d_codec GDCMJPEG16_jinit_lossless_d_codec
#define jinit_lossy_c_codec GDCMJPEG16_jinit_lossy_c_codec
#define jinit_lossy_d_codec GDCMJPEG16_jinit_lossy_d_codec
#define jinit_marker_reader GDCMJPEG16_jinit_marker_reader
#define jinit_marker_writer GDCMJPEG16_jinit_marker_writer
#define jinit_master_decompress GDCMJPEG16_jinit_master_decompress
#define jinit_memory_mgr GDCMJPEG16_jinit_memory_mgr
#define jinit_merged_upsampler GDCMJPEG16_jinit_merged_upsampler
#define jinit_phuff_decoder GDCMJPEG16_jinit_phuff_decoder
#define jinit_phuff_encoder GDCMJPEG16_jinit_phuff_encoder
#define jinit_shuff_decoder GDCMJPEG16_jinit_shuff_decoder
#define jinit_shuff_encoder GDCMJPEG16_jinit_shuff_encoder
#define jinit_undifferencer GDCMJPEG16_jinit_undifferencer
#define jinit_upsampler GDCMJPEG16_jinit_upsampler
#define jpeg_CreateCompress GDCMJPEG16_jpeg_CreateCompress
#define jpeg_CreateDecompress GDCMJPEG16_jpeg_CreateDecompress
#define jpeg_abort GDCMJPEG16_jpeg_abort
#define jpeg_abort_compress GDCMJPEG16_jpeg_abort_compress
#define jpeg_abort_decompress GDCMJPEG16_jpeg_abort_decompress
#define jpeg_add_quant_table GDCMJPEG16_jpeg_add_quant_table
#define jpeg_alloc_huff_table GDCMJPEG16_jpeg_alloc_huff_table
#define jpeg_alloc_quant_table GDCMJPEG16_jpeg_alloc_quant_table
#define jpeg_calc_output_dimensions GDCMJPEG16_jpeg_calc_output_dimensions
#define jpeg_consume_input GDCMJPEG16_jpeg_consume_input
#define jpeg_copy_critical_parameters GDCMJPEG16_jpeg_copy_critical_parameters
#define jpeg_default_colorspace GDCMJPEG16_jpeg_default_colorspace
#define jpeg_destroy GDCMJPEG16_jpeg_destroy
#define jpeg_destroy_compress GDCMJPEG16_jpeg_destroy_compress
#define jpeg_destroy_decompress GDCMJPEG16_jpeg_destroy_decompress
#define jpeg_fdct_float GDCMJPEG16_jpeg_fdct_float
#define jpeg_fdct_ifast GDCMJPEG16_jpeg_fdct_ifast
#define jpeg_fdct_islow GDCMJPEG16_jpeg_fdct_islow
#define jpeg_fill_bit_buffer GDCMJPEG16_jpeg_fill_bit_buffer
#define jpeg_finish_compress GDCMJPEG16_jpeg_finish_compress
#define jpeg_finish_decompress GDCMJPEG16_jpeg_finish_decompress
#define jpeg_finish_output GDCMJPEG16_jpeg_finish_output
#define jpeg_free_large GDCMJPEG16_jpeg_free_large
#define jpeg_free_small GDCMJPEG16_jpeg_free_small
#define jpeg_gen_optimal_table GDCMJPEG16_jpeg_gen_optimal_table
#define jpeg_get_large GDCMJPEG16_jpeg_get_large
#define jpeg_get_small GDCMJPEG16_jpeg_get_small
#define jpeg_has_multiple_scans GDCMJPEG16_jpeg_has_multiple_scans
#define jpeg_huff_decode GDCMJPEG16_jpeg_huff_decode
#define jpeg_idct_1x1 GDCMJPEG16_jpeg_idct_1x1
#define jpeg_idct_2x2 GDCMJPEG16_jpeg_idct_2x2
#define jpeg_idct_4x4 GDCMJPEG16_jpeg_idct_4x4
#define jpeg_idct_float GDCMJPEG16_jpeg_idct_float
#define jpeg_idct_ifast GDCMJPEG16_jpeg_idct_ifast
#define jpeg_idct_islow GDCMJPEG16_jpeg_idct_islow
#define jpeg_input_complete GDCMJPEG16_jpeg_input_complete
#define jpeg_make_c_derived_tbl GDCMJPEG16_jpeg_make_c_derived_tbl
#define jpeg_make_d_derived_tbl GDCMJPEG16_jpeg_make_d_derived_tbl
#define jpeg_mem_available GDCMJPEG16_jpeg_mem_available
#define jpeg_mem_init GDCMJPEG16_jpeg_mem_init
#define jpeg_mem_term GDCMJPEG16_jpeg_mem_term
#define jpeg_natural_order GDCMJPEG16_jpeg_natural_order
#define jpeg_new_colormap GDCMJPEG16_jpeg_new_colormap
#define jpeg_open_backing_store GDCMJPEG16_jpeg_open_backing_store
#define jpeg_quality_scaling GDCMJPEG16_jpeg_quality_scaling
#define jpeg_read_coefficients GDCMJPEG16_jpeg_read_coefficients
#define jpeg_read_header GDCMJPEG16_jpeg_read_header
#define jpeg_read_raw_data GDCMJPEG16_jpeg_read_raw_data
#define jpeg_read_scanlines GDCMJPEG16_jpeg_read_scanlines
#define jpeg_resync_to_restart GDCMJPEG16_jpeg_resync_to_restart
#define jpeg_save_markers GDCMJPEG16_jpeg_save_markers
#define jpeg_set_colorspace GDCMJPEG16_jpeg_set_colorspace
#define jpeg_set_defaults GDCMJPEG16_jpeg_set_defaults
#define jpeg_set_linear_quality GDCMJPEG16_jpeg_set_linear_quality
#define jpeg_set_marker_processor GDCMJPEG16_jpeg_set_marker_processor
#define jpeg_set_quality GDCMJPEG16_jpeg_set_quality
#define jpeg_simple_lossless GDCMJPEG16_jpeg_simple_lossless
#define jpeg_simple_progression GDCMJPEG16_jpeg_simple_progression
#define jpeg_start_compress GDCMJPEG16_jpeg_start_compress
#define jpeg_start_decompress GDCMJPEG16_jpeg_start_decompress
#define jpeg_start_output GDCMJPEG16_jpeg_start_output
#define jpeg_std_error GDCMJPEG16_jpeg_std_error
#define jpeg_std_message_table GDCMJPEG16_jpeg_std_message_table
#define jpeg_stdio_dest GDCMJPEG16_jpeg_stdio_dest
#define jpeg_stdio_src GDCMJPEG16_jpeg_stdio_src
#define jpeg_suppress_tables GDCMJPEG16_jpeg_suppress_tables
#define jpeg_write_coefficients GDCMJPEG16_jpeg_write_coefficients
#define jpeg_write_m_byte GDCMJPEG16_jpeg_write_m_byte
#define jpeg_write_m_header GDCMJPEG16_jpeg_write_m_header
#define jpeg_write_marker GDCMJPEG16_jpeg_write_marker
#define jpeg_write_raw_data GDCMJPEG16_jpeg_write_raw_data
#define jpeg_write_scanlines GDCMJPEG16_jpeg_write_scanlines
#define jpeg_write_tables GDCMJPEG16_jpeg_write_tables
#define jround_up GDCMJPEG16_jround_up
#define jzero_far GDCMJPEG16_jzero_far

#define jpeg_memory_src GDCMJPEG16_memory_src
#define jpeg_memory_mgr GDCMJPEG16_memory_mgr
#define jpeg_common_struct GDCMJPEG16_common_struct
#define jpeg_error_mgr GDCMJPEG16_error_mgr
#define jpeg_progress_mgr GDCMJPEG16_progress_mgr
#define jpeg_decompress_struct GDCMJPEG16_decompress_struct
#define jpeg_compress_struct GDCMJPEG16_compress_struct
#define jpeg_destination_mgr GDCMJPEG16_destination_mgr
#define jpeg_source_mgr GDCMJPEG16_source_mgr
#define jpeg_std_message_table GDCMJPEG16_MsgTable

#endif /* __jpegcmake_h */
