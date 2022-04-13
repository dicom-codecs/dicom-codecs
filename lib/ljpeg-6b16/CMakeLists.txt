
#set(LJPEG6b_COMPILE_OPTIONS ${LJPEG6b_COMPILE_OPTIONS} "$<$<CONFIG:Release>:-ffast-math>")

set(LJPEG6b_SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/ljpeg-6b")

# source files: JPEG library proper
set(LJPEG6b_LIBSOURCES
    ${LJPEG6b_SOURCE_DIR}/jcapimin.c
    ${LJPEG6b_SOURCE_DIR}/jcapistd.c
    ${LJPEG6b_SOURCE_DIR}/jccoefct.c
    ${LJPEG6b_SOURCE_DIR}/jccolor.c
    ${LJPEG6b_SOURCE_DIR}/jcdctmgr.c
    ${LJPEG6b_SOURCE_DIR}/jcdiffct.c

    ${LJPEG6b_SOURCE_DIR}/jchuff.c
    ${LJPEG6b_SOURCE_DIR}/jcinit.c
    ${LJPEG6b_SOURCE_DIR}/jclhuff.c
    ${LJPEG6b_SOURCE_DIR}/jclossls.c
    ${LJPEG6b_SOURCE_DIR}/jclossy.c
    ${LJPEG6b_SOURCE_DIR}/jcmainct.c

    ${LJPEG6b_SOURCE_DIR}/jcmarker.c
    ${LJPEG6b_SOURCE_DIR}/jcmaster.c
    ${LJPEG6b_SOURCE_DIR}/jcodec.c
    ${LJPEG6b_SOURCE_DIR}/jcomapi.c
    ${LJPEG6b_SOURCE_DIR}/jcparam.c
    ${LJPEG6b_SOURCE_DIR}/jcphuff.c
    ${LJPEG6b_SOURCE_DIR}/jcpred.c

    ${LJPEG6b_SOURCE_DIR}/jcprepct.c
    ${LJPEG6b_SOURCE_DIR}/jcsample.c
    ${LJPEG6b_SOURCE_DIR}/jcscale.c
    ${LJPEG6b_SOURCE_DIR}/jcshuff.c
    ${LJPEG6b_SOURCE_DIR}/jctrans.c
    ${LJPEG6b_SOURCE_DIR}/jdapimin.c

    ${LJPEG6b_SOURCE_DIR}/jdapistd.c
    ${LJPEG6b_SOURCE_DIR}/jdatadst.c
    ${LJPEG6b_SOURCE_DIR}/jdatasrc.c
    ${LJPEG6b_SOURCE_DIR}/jdcoefct.c
    ${LJPEG6b_SOURCE_DIR}/jdcolor.c
    ${LJPEG6b_SOURCE_DIR}/jddctmgr.c

    ${LJPEG6b_SOURCE_DIR}/jddiffct.c
    ${LJPEG6b_SOURCE_DIR}/jdhuff.c
    ${LJPEG6b_SOURCE_DIR}/jdinput.c
    ${LJPEG6b_SOURCE_DIR}/jdlhuff.c
    ${LJPEG6b_SOURCE_DIR}/jdlossls.c
    ${LJPEG6b_SOURCE_DIR}/jdlossy.c

    ${LJPEG6b_SOURCE_DIR}/jdmainct.c
    ${LJPEG6b_SOURCE_DIR}/jdmarker.c
    ${LJPEG6b_SOURCE_DIR}/jdmaster.c
    ${LJPEG6b_SOURCE_DIR}/jdmerge.c
    ${LJPEG6b_SOURCE_DIR}/jdphuff.c
    ${LJPEG6b_SOURCE_DIR}/jdpostct.c

    ${LJPEG6b_SOURCE_DIR}/jdpred.c
    ${LJPEG6b_SOURCE_DIR}/jdsample.c
    ${LJPEG6b_SOURCE_DIR}/jdscale.c
    ${LJPEG6b_SOURCE_DIR}/jdshuff.c
    ${LJPEG6b_SOURCE_DIR}/jdtrans.c
    ${LJPEG6b_SOURCE_DIR}/jerror.c
    ${LJPEG6b_SOURCE_DIR}/jfdctflt.c

    ${LJPEG6b_SOURCE_DIR}/jfdctfst.c
    ${LJPEG6b_SOURCE_DIR}/jfdctint.c
    ${LJPEG6b_SOURCE_DIR}/jidctflt.c
    ${LJPEG6b_SOURCE_DIR}/jidctfst.c
    ${LJPEG6b_SOURCE_DIR}/jidctint.c
    ${LJPEG6b_SOURCE_DIR}/jidctred.c

    ${LJPEG6b_SOURCE_DIR}/jquant1.c
    ${LJPEG6b_SOURCE_DIR}/jquant2.c
    ${LJPEG6b_SOURCE_DIR}/jutils.c
    ${LJPEG6b_SOURCE_DIR}/jmemmgr.c
)

# memmgr back ends: compile only one of these into a working library
set(LJPEG6b_SYSDEPSOURCES
#    ${LJPEG6b_SOURCE_DIR}/jmemansi.c
#    ${LJPEG6b_SOURCE_DIR}/jmemname.c
    ${LJPEG6b_SOURCE_DIR}/jmemnobs.c
#    ${LJPEG6b_SOURCE_DIR}/jmemdos.c
#    ${LJPEG6b_SOURCE_DIR}/jmemmac.c
)

set(LJPEG6b_CODEC
    jmemsrc.c
    decoder.cpp
    encoder.cpp
    jdatadst.c
)

set(LJPEG6b_INCLUDES
    ${LJPEG6b_SOURCE_DIR}/jchuff.h
    ${LJPEG6b_SOURCE_DIR}/jdhuff.h
    ${LJPEG6b_SOURCE_DIR}/jdct.h
    ${LJPEG6b_SOURCE_DIR}/jerror.h
    ${LJPEG6b_SOURCE_DIR}/jinclude.h
    ${LJPEG6b_SOURCE_DIR}/jlossls.h
    ${LJPEG6b_SOURCE_DIR}/jlossy.h

    ${LJPEG6b_SOURCE_DIR}/jmemsys.h
    ${LJPEG6b_SOURCE_DIR}/jmorecfg.h
    ${LJPEG6b_SOURCE_DIR}/jpegint.h
    ${LJPEG6b_SOURCE_DIR}/jpeglib.h
    ${LJPEG6b_SOURCE_DIR}/jversion.h
    ${LJPEG6b_SOURCE_DIR}/cdjpeg.h

    ${LJPEG6b_SOURCE_DIR}/cderror.h
    ${LJPEG6b_SOURCE_DIR}/transupp.h
)

add_library(ljpeg-6b16 
    ${LJPEG6b_LIBSOURCES}
    ${LJPEG6b_SYSDEPSOURCES}
    ${LJPEG6b_CODEC}
    ${LJPEG6b_INCLUDES}
)
target_compile_options(ljpeg-6b16 PUBLIC -DBITS_IN_JSAMPLE=12)
target_compile_options(ljpeg-6b16 PUBLIC "SHELL:-iquote ${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(ljpeg-6b16 PRIVATE ${LJPEG6b_COMPILE_OPTIONS})
target_include_directories(ljpeg-6b16 PUBLIC "${LJPEG6b_SOURCE_DIR}")