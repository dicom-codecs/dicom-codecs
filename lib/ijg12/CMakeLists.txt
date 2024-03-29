
set(IJG_ROOT "${PROJECT_SOURCE_DIR}/extern/ijg")

set(PUBLIC_HEADERS
"${IJG_ROOT}/jpeglib.h"
"${IJG_ROOT}/jerror.h"
"${IJG_ROOT}/jmorecfg.h"
)

include_directories(${IJG_ROOT})

set(LIBSOURCES
"${IJG_ROOT}/jaricom.c"
"${IJG_ROOT}/jcapimin.c"
"${IJG_ROOT}/jcapistd.c"
"${IJG_ROOT}/jcarith.c"
"${IJG_ROOT}/jccoefct.c"
"${IJG_ROOT}/jccolor.c"

"${IJG_ROOT}/jcdctmgr.c"
"${IJG_ROOT}/jchuff.c"
"${IJG_ROOT}/jcinit.c"
"${IJG_ROOT}/jcmainct.c"
"${IJG_ROOT}/jcmarker.c"
"${IJG_ROOT}/jcmaster.c"

"${IJG_ROOT}/jcomapi.c"
"${IJG_ROOT}/jcparam.c"
"${IJG_ROOT}/jcprepct.c"
"${IJG_ROOT}/jcsample.c"
"${IJG_ROOT}/jctrans.c"
"${IJG_ROOT}/jdapimin.c"

"${IJG_ROOT}/jdapistd.c"
"${IJG_ROOT}/jdarith.c"
"${IJG_ROOT}/jdatadst.c"
"${IJG_ROOT}/jdatasrc.c"
"${IJG_ROOT}/jdcoefct.c"
"${IJG_ROOT}/jdcolor.c"

"${IJG_ROOT}/jddctmgr.c"
"${IJG_ROOT}/jdhuff.c"
"${IJG_ROOT}/jdinput.c"
"${IJG_ROOT}/jdmainct.c"
"${IJG_ROOT}/jdmarker.c"
"${IJG_ROOT}/jdmaster.c"

"${IJG_ROOT}/jdmerge.c"
"${IJG_ROOT}/jdpostct.c"
"${IJG_ROOT}/jdsample.c"
"${IJG_ROOT}/jdtrans.c"
"${IJG_ROOT}/jerror.c"
"${IJG_ROOT}/jfdctflt.c"

"${IJG_ROOT}/jfdctfst.c"
"${IJG_ROOT}/jfdctint.c"
"${IJG_ROOT}/jidctflt.c"
"${IJG_ROOT}/jidctfst.c"
"${IJG_ROOT}/jidctint.c"
"${IJG_ROOT}/jquant1.c"

"${IJG_ROOT}/jquant2.c"
"${IJG_ROOT}/jutils.c"
"${IJG_ROOT}/jmemmgr.c"
)

set(SYSDEPSOURCES
"${IJG_ROOT}/jmemnobs.c"
)

#add_library (ijg8 
#${LIBSOURCES} 
#${SYSDEPSOURCES}
#)
#target_compile_options(ijg8 PUBLIC -DBITS_IN_JSAMPLE=8)
#target_compile_options(ijg8 PUBLIC -iquote ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(ijg8 PUBLIC ${IJG_ROOT})

add_library (ijg12 
  ${LIBSOURCES} 
  ${SYSDEPSOURCES}
  encoder.cpp
  decoder.cpp
)
target_compile_options(ijg12 PUBLIC -DBITS_IN_JSAMPLE=12)
target_compile_options(ijg12 BEFORE PUBLIC "SHELL:-iquote ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(ijg12 PUBLIC ${IJG_ROOT})


