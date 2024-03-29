file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/dicomcodecs/*.hpp")

if(DICOM_CODECS_BUILD_OPENJPEG)
    set(OPENJPEG_SOURCES "openjpeg/decoder.cpp" "openjpeg/encoder.cpp" "openjpeg/buffer.cpp")
endif(DICOM_CODECS_BUILD_OPENJPEG)

if(DICOM_CODECS_BUILD_CHARLS)
    set(CHARLS_SOURCES "charls/decoder.cpp" "charls/encoder.cpp")
endif(DICOM_CODECS_BUILD_CHARLS)

if(DICOM_CODECS_BUILD_RLE)
    set(RLE_SOURCES "rle/decoder.cpp" "rle/encoder.cpp")
endif(DICOM_CODECS_BUILD_RLE)

if(DICOM_CODECS_BUILD_KAKADU)
    set(KAKADU_SOURCES "kakadu/decoder.cpp" "kakadu/encoder.cpp")
endif(DICOM_CODECS_BUILD_KAKADU)

add_library(dicomcodecs
    ${HEADER_LIST}
    lib.cpp
    ${OPENJPEG_SOURCES}
    ${CHARLS_SOURCES}
    ${LIBJPEGTURBO_SOURCES}
    ${RLE_SOURCES}
    ${KAKADU_SOURCES}
)

# We need this directory, and users of our library will need it too
target_include_directories(dicomcodecs PUBLIC ../include)

if(DICOM_CODECS_BUILD_RLE)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_RLE)
endif(DICOM_CODECS_BUILD_RLE)

if(DICOM_CODECS_BUILD_OPENJPEG)
    target_link_libraries(dicomcodecs PRIVATE openjp2)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_OPENJPEG)
endif(DICOM_CODECS_BUILD_OPENJPEG)

if(DICOM_CODECS_BUILD_CHARLS)
    target_link_libraries(dicomcodecs PRIVATE charls)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_CHARLS)
endif(DICOM_CODECS_BUILD_CHARLS)

if(DICOM_CODECS_BUILD_LIBJPEGTURBO8)
    target_link_libraries(dicomcodecs PRIVATE libjpeg-turbo8)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_LIBJPEGTURBO8)
endif(DICOM_CODECS_BUILD_LIBJPEGTURBO8)

if(DICOM_CODECS_BUILD_IJG12)
    target_link_libraries(dicomcodecs PRIVATE ijg12)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_IJG12)
endif(DICOM_CODECS_BUILD_IJG12)

if(DICOM_CODECS_BUILD_LJPEG6B16)
    target_link_libraries(dicomcodecs PRIVATE ljpeg-6b16)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_BUILD_LJPEG6B16)
endif(DICOM_CODECS_BUILD_LJPEG6B16)

if(DICOM_CODECS_GDCM_JPEG16)
    target_link_libraries(dicomcodecs PRIVATE gdcm16)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_GDCM_JPEG16)
endif(DICOM_CODECS_GDCM_JPEG16)

if(DICOM_CODECS_OPENJPH)
    target_link_libraries(dicomcodecs PRIVATE libopenjph)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_OPENJPH)
endif(DICOM_CODECS_OPENJPH)

if(DICOM_CODECS_KAKADU)
    target_link_libraries(dicomcodecs PRIVATE libkakadu)
    target_compile_options(dicomcodecs PUBLIC -DDICOM_CODECS_KAKADU)
endif(DICOM_CODECS_KAKADU)

# All users of this library will need at least C++11
target_compile_features(dicomcodecs PUBLIC cxx_std_11)

# include_directories("${PROJECT_SOURCE_DIR}/extern/openjpeg/src/lib/openjp2" "${OPENJPEG_BINARY_DIR}/src/lib/openjp2")

# IDEs should put the headers in a nice place
# source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
