cmake_minimum_required(VERSION 3.15)

project(dicomcodecs,
  LANGUAGES C CXX)

# set the build type if not specified
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()

# set all projects to build static libraries
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) # need to explicitly set new behavior for option() to honor normal variables
set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

# options to control whether or not we build specific libraries.  This allows users to easily add just the codecs they want
option(DICOM_CODECS_BUILD_OPENJPEG "Build OpenJPEG" ON)
option(DICOM_CODECS_BUILD_CHARLS "Build CharLS" ON)
option(DICOM_CODECS_BUILD_LIBJPEGTURBO8 "Build libjpeg-turbo 8 bit" ON)
option(DICOM_CODECS_BUILD_IJG12 "Build IJG 12 bit" ON)
option(DICOM_CODECS_BUILD_RLE "Build RLE" ON)
option(DICOM_CODECS_BUILD_LJPEG6B16 "Build ljpeg-6b 16 bit Codec" ON)
option(DICOM_CODECS_GDCM_JPEG16 "Build GDCM JPEG16 Codec" ON)
option(DICOM_CODECS_OPENJPH "Build OpenJPH Codec" ON)
option(DICOM_CODECS_KAKADU "Build Kakadu Codec" ON)

if(DICOM_CODECS_BUILD_OPENJPEG)
  add_subdirectory(lib/openjpeg EXCLUDE_FROM_ALL)

  # option(OPJ_USE_THREAD "Build with thread/mutex support " OFF)
  # add_subdirectory(extern/openjpeg EXCLUDE_FROM_ALL)
  # set(EXECUTABLE_OUTPUT_PATH "")
  # set(LIBRARY_OUTPUT_PATH "")
endif(DICOM_CODECS_BUILD_OPENJPEG)

if(DICOM_CODECS_BUILD_CHARLS)
  add_subdirectory(extern/charls EXCLUDE_FROM_ALL)
endif(DICOM_CODECS_BUILD_CHARLS)

if(DICOM_CODECS_BUILD_IJG12)
  add_subdirectory(lib/ijg12 EXCLUDE_FROM_ALL)
endif(DICOM_CODECS_BUILD_IJG12)

if(DICOM_CODECS_BUILD_LIBJPEGTURBO8)
  add_subdirectory(extern/libjpeg-turbo EXCLUDE_FROM_ALL)
  add_subdirectory(lib/libjpeg-turbo8 EXCLUDE_FROM_ALL)
endif(DICOM_CODECS_BUILD_LIBJPEGTURBO8)

if(DICOM_CODECS_BUILD_LIBJPEG)
  add_subdirectory(lib/libjpeg EXCLUDE_FROM_ALL)
endif(DICOM_CODECS_BUILD_LIBJPEG)

if(DICOM_CODECS_BUILD_LJPEG6B16)
  add_subdirectory(lib/ljpeg-6b16 EXCLUDE_FROM_ALL)
endif()

if(DICOM_CODECS_GDCM_JPEG16)
  add_subdirectory(lib/gdcm16 EXCLUDE_FROM_ALL)
endif()

if(DICOM_CODECS_OPENJPH)
  add_subdirectory(extern/OpenJPH EXCLUDE_FROM_ALL)
  add_subdirectory(lib/openjph EXCLUDE_FROM_ALL)
endif()

if(DICOM_CODECS_KAKADU)
  add_subdirectory(extern/Kakadu EXCLUDE_FROM_ALL)
  add_subdirectory(lib/kakadu EXCLUDE_FROM_ALL)
endif()

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
# add_subdirectory(apps)
add_subdirectory(tests)
