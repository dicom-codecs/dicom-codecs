
set(GDCM_COMPILE_OPTIONS ${GDCM_COMPILE_OPTIONS} "$<$<CONFIG:Release>:-ffast-math>")

set(GDCM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/GDCM/Utilities/gdcmjpeg")

set(GDCM_SRCS
    # memmgr back ends: compile only one of these into a working library
    # (For now, let's use the mode that requires the image fit into memory.
    # This is the recommended mode for Win32 anyway.)
    ${GDCM_SOURCE_DIR}/jmemnobs.c
    # library object files common to compression and decompression
    ${GDCM_SOURCE_DIR}/jcomapi.c
    ${GDCM_SOURCE_DIR}/jutils.c
    ${GDCM_SOURCE_DIR}/jerror.c
    ${GDCM_SOURCE_DIR}/jmemmgr.c
    # compression library object files
    ${GDCM_SOURCE_DIR}/jcapimin.c
    ${GDCM_SOURCE_DIR}/jcapistd.c
    ${GDCM_SOURCE_DIR}/jctrans.c
    ${GDCM_SOURCE_DIR}/jcparam.c
    ${GDCM_SOURCE_DIR}/jcinit.c
    ${GDCM_SOURCE_DIR}/jcmaster.c
    ${GDCM_SOURCE_DIR}/jcmarker.c
    ${GDCM_SOURCE_DIR}/jcmainct.c
    ${GDCM_SOURCE_DIR}/jcprepct.c
    ${GDCM_SOURCE_DIR}/jccoefct.c
    ${GDCM_SOURCE_DIR}/jccolor.c
    ${GDCM_SOURCE_DIR}/jcsample.c
    ${GDCM_SOURCE_DIR}/jchuff.c
    ${GDCM_SOURCE_DIR}/jcphuff.c
    ${GDCM_SOURCE_DIR}/jcdctmgr.c
    ${GDCM_SOURCE_DIR}/jfdctfst.c
    ${GDCM_SOURCE_DIR}/jfdctflt.c
    #${GDCM_SOURCE_DIR}/jdatadst.c
    ${GDCM_SOURCE_DIR}/jmemsrc.c
    # Lossy (DCT) codec:
    ${GDCM_SOURCE_DIR}/jfdctint.c
    ${GDCM_SOURCE_DIR}/jclossy.c
    # lossless
    ${GDCM_SOURCE_DIR}/jclossls.c
    #jdarith.c
    ${GDCM_SOURCE_DIR}/jcodec.c
    ${GDCM_SOURCE_DIR}/jcscale.c
    ${GDCM_SOURCE_DIR}/jcshuff.c
    ${GDCM_SOURCE_DIR}/jclhuff.c
    ${GDCM_SOURCE_DIR}/jcpred.c
    ${GDCM_SOURCE_DIR}/jcdiffct.c
    # decompression library object files
    ${GDCM_SOURCE_DIR}/jdapimin.c
    ${GDCM_SOURCE_DIR}/jdapistd.c
    ${GDCM_SOURCE_DIR}/jdtrans.c
    ${GDCM_SOURCE_DIR}/jdmaster.c
    ${GDCM_SOURCE_DIR}/jdinput.c
    ${GDCM_SOURCE_DIR}/jdmarker.c
    ${GDCM_SOURCE_DIR}/jdhuff.c
    ${GDCM_SOURCE_DIR}/jdphuff.c
    ${GDCM_SOURCE_DIR}/jdmainct.c
    ${GDCM_SOURCE_DIR}/jdcoefct.c
    ${GDCM_SOURCE_DIR}/jdpostct.c
    ${GDCM_SOURCE_DIR}/jddctmgr.c
    ${GDCM_SOURCE_DIR}/jidctfst.c
    ${GDCM_SOURCE_DIR}/jidctflt.c
    ${GDCM_SOURCE_DIR}/jidctint.c
    ${GDCM_SOURCE_DIR}/jidctred.c
    ${GDCM_SOURCE_DIR}/jdsample.c
    ${GDCM_SOURCE_DIR}/jdcolor.c
    ${GDCM_SOURCE_DIR}/jquant1.c
    ${GDCM_SOURCE_DIR}/jquant2.c
    ${GDCM_SOURCE_DIR}/jdmerge.c
    #{GDCM_SOURCE_DIR}/jdatasrc.c
    # decomp_lossy_SRCS
    ${GDCM_SOURCE_DIR}/jdlossls.c
    ${GDCM_SOURCE_DIR}/jdlossy.c
    #${GDCM_SOURCE_DIR}/jcarith.c
    ${GDCM_SOURCE_DIR}/jdlhuff.c
    ${GDCM_SOURCE_DIR}/jddiffct.c
    ${GDCM_SOURCE_DIR}/jdpred.c
    ${GDCM_SOURCE_DIR}/jdscale.c
    ${GDCM_SOURCE_DIR}/jdshuff.c
)

set(JPEG_HEADER_FILES
    ${GDCM_SOURCE_DIR}/jchuff.h
    ${GDCM_SOURCE_DIR}/jdct.h
    ${GDCM_SOURCE_DIR}/jdhuff.h
    ${GDCM_SOURCE_DIR}/jerror.h
    ${GDCM_SOURCE_DIR}/jinclude.h
    ${GDCM_SOURCE_DIR}/jmemsys.h
    ${GDCM_SOURCE_DIR}/jmorecfg.h
    ${GDCM_SOURCE_DIR}/jpegint.h
    ${GDCM_SOURCE_DIR}/jpeglib.h
    ${GDCM_SOURCE_DIR}/jversion.h
    ${GDCM_SOURCE_DIR}/jlossy.h
    ${GDCM_SOURCE_DIR}/jlossls.h
  )

set(JPEGSTATIC 1)

add_library(gdcm16 
    ${JPEG_HEADER_FILES}
    ${GDCM_SRCS} 
    encoder.cpp
    decoder.cpp
    jdatadst.c
)

target_compile_options(gdcm16 PUBLIC "SHELL:-iquote ${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(gdcm16 PRIVATE ${GDCM_COMPILE_OPTIONS})
target_include_directories(gdcm16 PUBLIC "${GDCM_SOURCE_DIR}")