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
option(DICOM_CODECS_BUILD_LIBJPEGTURBO "Build libjpeg-turbo 8 bit" ON)
option(DICOM_CODECS_BUILD_IJG "Build IJG 12 bit" ON)

if(DICOM_CODECS_BUILD_OPENJPEG)
  # NOTE - Add OpenJPEG first since its CMakeLists.txt messes variables up that impact other projects
  option(OPJ_USE_THREAD "Build with thread/mutex support " OFF)
  add_subdirectory(extern/openjpeg EXCLUDE_FROM_ALL)
  # BEGIN - Correct the bugs in the OpenJPEG CMakeLists.txt file here..
  set(EXECUTABLE_OUTPUT_PATH "")
  set(LIBRARY_OUTPUT_PATH "")  
  # END - Correct the bugs in the OpenJPEG CMakeLists.txt file here..
endif(DICOM_CODECS_BUILD_OPENJPEG)

if(DICOM_CODECS_BUILD_CHARLS)
  add_subdirectory(extern/charls EXCLUDE_FROM_ALL)
endif(DICOM_CODECS_BUILD_CHARLS)

if(DICOM_CODECS_BUILD_IJG)
  add_subdirectory(src/ijg EXCLUDE_FROM_ALL)
endif(DICOM_CODECS_BUILD_IJG)

if(DICOM_CODECS_BUILD_LIBJPEGTURBO)
  add_subdirectory(extern/libjpeg-turbo EXCLUDE_FROM_ALL)
endif(DICOM_CODECS_BUILD_LIBJPEGTURBO)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
#add_subdirectory(apps)

add_subdirectory(tests)




