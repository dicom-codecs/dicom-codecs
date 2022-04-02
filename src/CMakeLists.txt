

# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${dicomcodecs_SOURCE_DIR}/include/dicomcodecs/*.hpp")

if(DICOM_CODECS_BUILD_OPENJPEG)
set(OPENJPEG_SOURCES "openjpeg/decoder.cpp")
endif(DICOM_CODECS_BUILD_OPENJPEG)

if(DICOM_CODECS_BUILD_CHARLS)
set(CHARLS_SOURCES "charls/decoder.cpp"  "charls/encoder.cpp")
endif(DICOM_CODECS_BUILD_CHARLS)

if(DICOM_CODECS_BUILD_LIBJPEGTURBO)
set(LIBJPEGTURBO_SOURCES "libjpeg-turbo/decoder.cpp" "libjpeg-turbo/encoder.cpp")
endif(DICOM_CODECS_BUILD_LIBJPEGTURBO)

# Make an automatic library - will be static or dynamic based on user setting
add_library(dicomcodecs 
    ${HEADER_LIST}
    lib.cpp 
    ${OPENJPEG_SOURCES} 
    ${CHARLS_SOURCES} 
    ${LIBJPEGTURBO_SOURCES} 
)

# We need this directory, and users of our library will need it too
target_include_directories(dicomcodecs PUBLIC ../include)

# This depends on (header only) boost
if(DICOM_CODECS_BUILD_OPENJPEG)
target_link_libraries(dicomcodecs PRIVATE openjp2)
target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_OPENJPEG)
endif(DICOM_CODECS_BUILD_OPENJPEG)

if(DICOM_CODECS_BUILD_CHARLS)
target_link_libraries(dicomcodecs PRIVATE charls)
target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_CHARLS)
endif(DICOM_CODECS_BUILD_CHARLS)

if(DICOM_CODECS_BUILD_LIBJPEGTURBO)
target_link_libraries(dicomcodecs PRIVATE turbojpeg-static)
target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_LIBJPEGTURBO)
endif(DICOM_CODECS_BUILD_LIBJPEGTURBO)

if(DICOM_CODECS_BUILD_IJG)
target_link_libraries(dicomcodecs PRIVATE jpeg12)
target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_IJG)
endif(DICOM_CODECS_BUILD_IJG)

#target_link_libraries(dicomcodecs 
#    PRIVATE openjp2
#    PRIVATE charls
#    PRIVATE turbojpeg-static
#    PRIVATE jpeg12
#) 

# All users of this library will need at least C++11
target_compile_features(dicomcodecs PUBLIC cxx_std_11)

include_directories("../extern/openjpeg/src/lib/openjp2" "../build/extern/openjpeg/src/lib/openjp2")

# IDEs should put the headers in a nice place
#source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
