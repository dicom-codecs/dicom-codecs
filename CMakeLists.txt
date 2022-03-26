cmake_minimum_required(VERSION 3.15)

project(dicomcodecs, 
    LANGUAGES CXX)

# set the build type if not specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()

option(BUILD_SHARED_LIBS "" OFF)
option(BUILD_STATIC_LIBS "" ON)

# NOTE - Add OpenJPEG first since its CMakeLists.txt messes variables up that impact other projects
option(OPJ_USE_THREAD "Build with thread/mutex support " OFF)
add_subdirectory(extern/openjpeg EXCLUDE_FROM_ALL)
# BEGIN - Correct the bugs in the OpenJPEG CMakeLists.txt file here..
set(EXECUTABLE_OUTPUT_PATH "")
set(LIBRARY_OUTPUT_PATH "")
# END - Correct the bugs in the OpenJPEG CMakeLists.txt file here..

add_subdirectory(extern/charls EXCLUDE_FROM_ALL)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
#add_subdirectory(apps)

add_subdirectory(tests)




