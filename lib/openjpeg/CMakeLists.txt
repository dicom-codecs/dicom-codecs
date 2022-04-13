# disable threading
add_definitions( -DMUTEX_stub)

set(OPENJP2_COMPILE_OPTIONS ${OPENJP2_COMPILE_OPTIONS} "$<$<CONFIG:Release>:-ffast-math>" -Wall -Wextra -Wconversion -Wunused-parameter -Wdeclaration-after-statement -Werror=declaration-after-statement)

set(OPENJPEG_SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/openjpeg/src/lib/openjp2")

set(OPENJPEG_SRCS
  ${OPENJPEG_SOURCE_DIR}/thread.c
  ${OPENJPEG_SOURCE_DIR}/thread.h
  ${OPENJPEG_SOURCE_DIR}/bio.c
  ${OPENJPEG_SOURCE_DIR}/bio.h
  ${OPENJPEG_SOURCE_DIR}/cio.c
  ${OPENJPEG_SOURCE_DIR}/cio.h
  ${OPENJPEG_SOURCE_DIR}/dwt.c
  ${OPENJPEG_SOURCE_DIR}/dwt.h
  ${OPENJPEG_SOURCE_DIR}/event.c
  ${OPENJPEG_SOURCE_DIR}/event.h
  ${OPENJPEG_SOURCE_DIR}/ht_dec.c
  ${OPENJPEG_SOURCE_DIR}/image.c
  ${OPENJPEG_SOURCE_DIR}/image.h
  ${OPENJPEG_SOURCE_DIR}/invert.c
  ${OPENJPEG_SOURCE_DIR}/invert.h
  ${OPENJPEG_SOURCE_DIR}/j2k.c
  ${OPENJPEG_SOURCE_DIR}/j2k.h
  ${OPENJPEG_SOURCE_DIR}/jp2.c
  ${OPENJPEG_SOURCE_DIR}/jp2.h
  ${OPENJPEG_SOURCE_DIR}/mct.c
  ${OPENJPEG_SOURCE_DIR}/mct.h
  ${OPENJPEG_SOURCE_DIR}/mqc.c
  ${OPENJPEG_SOURCE_DIR}/mqc.h
  ${OPENJPEG_SOURCE_DIR}/mqc_inl.h
  ${OPENJPEG_SOURCE_DIR}/openjpeg.c
  ${OPENJPEG_SOURCE_DIR}/openjpeg.h
  ${OPENJPEG_SOURCE_DIR}/opj_clock.c
  ${OPENJPEG_SOURCE_DIR}/opj_clock.h
  ${OPENJPEG_SOURCE_DIR}/pi.c
  ${OPENJPEG_SOURCE_DIR}/pi.h
  ${OPENJPEG_SOURCE_DIR}/t1.c
  ${OPENJPEG_SOURCE_DIR}/t1.h
  ${OPENJPEG_SOURCE_DIR}/t2.c
  ${OPENJPEG_SOURCE_DIR}/t2.h
  ${OPENJPEG_SOURCE_DIR}/tcd.c
  ${OPENJPEG_SOURCE_DIR}/tcd.h
  ${OPENJPEG_SOURCE_DIR}/tgt.c
  ${OPENJPEG_SOURCE_DIR}/tgt.h
  ${OPENJPEG_SOURCE_DIR}/function_list.c
  ${OPENJPEG_SOURCE_DIR}/function_list.h
  ${OPENJPEG_SOURCE_DIR}/opj_codec.h
  ${OPENJPEG_SOURCE_DIR}/opj_includes.h
  ${OPENJPEG_SOURCE_DIR}/opj_intmath.h
  ${OPENJPEG_SOURCE_DIR}/opj_malloc.c
  ${OPENJPEG_SOURCE_DIR}/opj_malloc.h
  ${OPENJPEG_SOURCE_DIR}/opj_stdint.h
  ${OPENJPEG_SOURCE_DIR}/sparse_array.c
  ${OPENJPEG_SOURCE_DIR}/sparse_array.h
)

add_library(openjp2 
    ${HEADER_LIST}
    ${OPENJPEG_SRCS} 
)

target_compile_options(openjp2 PUBLIC "SHELL:-iquote ${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(openjp2 PRIVATE ${OPENJP2_COMPILE_OPTIONS})
target_include_directories(openjp2 PUBLIC "${PROJECT_SOURCE_DIR}/extern/openjpeg/src/lib/openjp2" )

