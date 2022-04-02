// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include <openjpeg.h>

typedef struct opj_buffer_info {
    OPJ_BYTE* buf;
    OPJ_BYTE* cur;
    OPJ_SIZE_T len;
} opj_buffer_info_t;


opj_stream_t* OPJ_CALLCONV opj_stream_create_buffer_stream (opj_buffer_info_t* psrc, OPJ_BOOL input);
