
cmake_minimum_required(VERSION 3.13...3.19)

#project(jpeg VERSION 1.58 LANGUAGES CXX)

# Determine if project is built as a subproject (using add_subdirectory) or if it is the master project
set(MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MASTER_PROJECT ON)
  message(STATUS "Building as master project, CMake version: ${CMAKE_VERSION}")
endif ()

# The basic options to control what to build extra
option(LIBJPEG_BUILD_TOOL "Build JPEG tool application" ${MASTER_PROJECT})

# Provide BUILD_SHARED_LIBS as an option for GUI tools
option(BUILD_SHARED_LIBS "Will control if libjpeg is build as shared lib/DLL or as static library")

# Options to increase the warning level and warnings as errors
option(LIBJPEG_PEDANTIC_WARNINGS "Enable extra compilers warnings." OFF)
option(LIBJPEG_THREAT_WARNINGS_AS_ERRORS "Enable the option to treat warnings as errors." OFF)

# Options to increase additional debug checks
set(LIBJPEG_DEBUG_LEVEL 0 CACHE STRING "Defines the debug level {0 (off), 2, 3}")
set(LIBJPEG_CHECK_LEVEL 0 CACHE STRING "Defines the check level {0 (off), 2, 3}")

# Build with C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure the supported C++ compilers: gcc, clang and MSVC
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options(
    -fno-exceptions
    -Wall
    -Wextra      # Alias for -W
    -Wpedantic   # Alias for -pedantic (also includes: -Wpointer-arith)
    -Wunused
    -Wcast-qual
    -Wwrite-strings
    -Woverloaded-virtual
    -Wsign-promo
    -Wno-shift-negative-value
    -Wno-class-memaccess
    -Wimplicit-fallthrough=0 # Disable implicit-fallthrough warnings
    -Wno-unused-parameter  # Disable warning about unused parameters
    -Wno-clobbered
    -Wno-unused-result
  )

  set(PEDANTIC_CXX_COMPILE_FLAGS
    -pedantic-errors
    -Wold-style-cast
    -Wfloat-equal
    -Wlogical-op
    -Wundef
    -Wshadow
    -Wcast-qual
    -Wformat=2
    -Wmissing-include-dirs
    -Wcast-align
    -Wctor-dtor-privacy
    -Wdisabled-optimization
    -Winvalid-pch
    -Woverloaded-virtual
    -Wnon-virtual-dtor
    -Wnoexcept
    -Wdouble-promotion
    -Wtrampolines
    -Wzero-as-null-pointer-constant
    -Wuseless-cast
    -Wvector-operation-performance
    -Wsized-deallocation
  )
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
      set(PEDANTIC_CXX_COMPILE_FLAGS ${PEDANTIC_CXX_COMPILE_FLAGS}
        -Wshift-overflow=2
        -Wnull-dereference
        -Wduplicated-cond
      )
  endif()

  set(WERROR_FLAG -Werror)
endif()


if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(
    -fno-exceptions
    -Wall
    -Wextra      # Alias for -W
    -Wpedantic   # Alias for -pedantic (also includes: -Wpointer-arith)
    -Wcast-qual
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wno-unused-parameter  # Disable warning about unused parameters
  )

  set(PEDANTIC_CXX_COMPILE_FLAGS
      -Weverything
      -Wno-weak-vtables                       # Ignore, linker will remove the couple of extra vtables.
      -Wno-padded                             # Ignore, padding optimization is not needed.
      -Wno-global-constructors                # Ignore, by design CharLS uses types created at startup.
      -Wno-switch-enum                        # Ignore, cases are handled by default.
      -Wno-sign-conversion                    # Ignore, would just introduce ugly static_asserts.
      -Wno-exit-time-destructors              # Ignore, by design exit-time destructors are used.
      -Wno-missing-braces                     # Ignore, False warning in clang 5.0, fixed in 6.0.
      -Wno-c++98-compat-pedantic              # Ignore, target is C++11, ignore C++98 compatibility.
  )

  if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0.2)
    set(PEDANTIC_CXX_COMPILE_FLAGS ${PEDANTIC_CXX_COMPILE_FLAGS}
      -Wno-undefined-func-template            # Ignore, linker will complain if final template code is not available.
    )
  endif()

  set(WERROR_FLAG -Werror)
endif()


if(MSVC)
  set(PEDANTIC_CXX_COMPILE_FLAGS /W4)
  set(WERROR_FLAG /WX)

  # Disable C++ exceptions.
  string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")

  # Remove option /GR (/GR is added by default by CMake). /GR is already the default
  # and this makes it possible to use /GR- without warnings.
  string(REGEX REPLACE " /GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

  # All C++ source files are in utf-8 without signature (BOM), MSVC requires the utf-8 switch to read files as utf-8.
  add_compile_options("/utf-8")

  # Zc:__cplusplus: Will configure the MSVC compiler to use the correct value for the __cplusplus macro
  # (introduced with Visual Studio 2017 version 15.7)
  if(MSVC_VERSION GREATER_EQUAL 1914)
    add_compile_options("/Zc:__cplusplus")
  endif()

  # /Zc:throwingNew: Will configure the MSVC compiler that only the standard throwing operator new is used.
  add_compile_options("/Zc:throwingNew")
endif()


# When enabled apply the pedantic warnings options and warnings as errors globally.
if(LIBJPEG_PEDANTIC_WARNINGS)
  if(MSVC)
    # Remove existing warning level (W3 is added by default by CMake), duplicate level will generate cmd-line warnings.
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()

  add_compile_options(${PEDANTIC_CXX_COMPILE_FLAGS})
endif()
if(LIBJPEG_THREAT_WARNINGS_AS_ERRORS)
  add_compile_options(${WERROR_FLAG})
endif()

set(LIBJPEG_SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/libjpeg")

# Define the library "libjpeg"
add_library(jpeg "")

set_target_properties(jpeg PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(jpeg PUBLIC ${LIBJPEG_SOURCE_DIR})

target_compile_definitions(jpeg PUBLIC DEBUG_LEVEL=${LIBJPEG_DEBUG_LEVEL} CHECK_LEVEL=${LIBJPEG_CHECK_LEVEL})

# When building as DLL (Windows) or shared libray, define JPEG_EXPORT
if(BUILD_SHARED_LIBS)
  target_compile_definitions(jpeg PRIVATE BUILD_LIB)
endif()


# Add all .cpp and .hpp sources files. Adding .hpp is not needed for the build but it helps IDEs to show the files.
target_sources(jpeg
  PRIVATE
    "${LIBJPEG_SOURCE_DIR}/boxes/alphabox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/box.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/checksumbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/colortrafobox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/databox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/dctbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/filetypebox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/floattonemappingbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/floattransformationbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/inversetonemappingbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/lineartransformationbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/matrixbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/mergingspecbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/namespace.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/nonlineartrafobox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/outputconversionbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/parametrictonemappingbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/refinementspecbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/superbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/tonemapperbox.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/aclosslessscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/acrefinementscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/acsequentialscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/decoder.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/encoder.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/entropyparser.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/image.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/jpeglsscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/lineinterleavedlsscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/losslessscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/predictivescan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/predictor.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/predictorbase.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/rectanglerequest.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/refinementscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/sampleinterleavedlsscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/sequentialscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/singlecomponentlsscan.cpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/tables.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/actemplate.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/arithmetictemplate.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/arthdeco.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/blockrow.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/decodertemplate.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmancoder.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmandecoder.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmanstatistics.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmantemplate.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/qmcoder.cpp"
    "${LIBJPEG_SOURCE_DIR}/coding/quantizedrow.cpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/colortrafo.cpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/colortransformerfactory.cpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/floattrafo.cpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/lslosslesstrafo.cpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/multiplicationtrafo.cpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/trivialtrafo.cpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/ycbcrtrafo.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/bitmapctrl.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/blockbitmaprequester.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/blockbuffer.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/blocklineadapter.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/bufferctrl.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/hierarchicalbitmaprequester.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/lineadapter.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/linebitmaprequester.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/linebuffer.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/linelineadapter.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/linemerger.cpp"
    "${LIBJPEG_SOURCE_DIR}/control/residualblockhelper.cpp"
    "${LIBJPEG_SOURCE_DIR}/dct/dct.cpp"
    "${LIBJPEG_SOURCE_DIR}/dct/deringing.cpp"
    "${LIBJPEG_SOURCE_DIR}/dct/idct.cpp"
    "${LIBJPEG_SOURCE_DIR}/dct/liftingdct.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/bitmaphook.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/hooks.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/imagebitmap.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/jpeg.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/jpgtypes.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/parameters.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/tagitem.cpp"
    "${LIBJPEG_SOURCE_DIR}/interface/types.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/bitstream.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/bytestream.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/checksumadapter.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/decoderstream.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/iostream.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/memorystream.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/randomaccessstream.cpp"
    "${LIBJPEG_SOURCE_DIR}/io/staticstream.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/actable.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/adobemarker.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/component.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/exifmarker.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/frame.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/huffmantable.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/jfifmarker.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/lscolortrafo.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/quantization.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/quantizationtable.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/restartintervalmarker.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/scan.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/scantypes.cpp"
    "${LIBJPEG_SOURCE_DIR}/marker/thresholds.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/assert.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/ctype.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/errno.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/math.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/setjmp.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/stdarg.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/stddef.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/stdio.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/stdlib.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/string.cpp"
    "${LIBJPEG_SOURCE_DIR}/std/unistd.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/checksum.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/debug.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/environment.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/line.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/numerics.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/priorityqueue.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/rectangle.cpp"
    "${LIBJPEG_SOURCE_DIR}/tools/traits.cpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/downsampler.cpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/interdownsampler.cpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/downsamplerbase.cpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/upsampler.cpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/cositedupsampler.cpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/upsamplerbase.cpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/alphabox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/box.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/checksumbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/databox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/dctbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/filetypebox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/floattonemappingbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/floattransformationbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/inversetonemappingbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/lineartransformationbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/matrixbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/mergingspecbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/namespace.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/nonlineartrafobox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/outputconversionbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/parametrictonemappingbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/refinementspecbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/superbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/boxes/tonemapperbox.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/aclosslessscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/acrefinementscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/acsequentialscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/decoder.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/encoder.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/entropyparser.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/image.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/jpeglsscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/lineinterleavedlsscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/losslessscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/predictivescan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/predictor.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/predictorbase.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/rectanglerequest.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/refinementscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/sampleinterleavedlsscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/sequentialscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/singlecomponentlsscan.hpp"
    "${LIBJPEG_SOURCE_DIR}/codestream/tables.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/actemplate.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/arithmetictemplate.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/arthdeco.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/blockrow.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/decodertemplate.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmancoder.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmandecoder.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmanstatistics.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/huffmantemplate.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/qmcoder.hpp"
    "${LIBJPEG_SOURCE_DIR}/coding/quantizedrow.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/colortrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/colortransformerfactory.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/floattrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/integertrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/lslosslesstrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/multiplicationtrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/trivialtrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/colortrafo/ycbcrtrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/config.h"
    "${LIBJPEG_SOURCE_DIR}/control/bitmapctrl.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/blockbitmaprequester.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/blockbuffer.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/blocklineadapter.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/bufferctrl.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/hierarchicalbitmaprequester.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/lineadapter.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/linebitmaprequester.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/linebuffer.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/linelineadapter.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/linemerger.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/residualblockhelper.hpp"
    "${LIBJPEG_SOURCE_DIR}/control/residualbuffer.hpp"
    "${LIBJPEG_SOURCE_DIR}/dct/dct.hpp"
    "${LIBJPEG_SOURCE_DIR}/dct/deringing.hpp"
    "${LIBJPEG_SOURCE_DIR}/dct/idct.hpp"
    "${LIBJPEG_SOURCE_DIR}/dct/liftingdct.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/bitmaphook.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/hooks.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/imagebitmap.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/jpeg.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/jpgtypes.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/parameters.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/tagitem.hpp"
    "${LIBJPEG_SOURCE_DIR}/interface/types.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/bitstream.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/bytestream.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/checksumadapter.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/decoderstream.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/iostream.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/memorystream.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/randomaccessstream.hpp"
    "${LIBJPEG_SOURCE_DIR}/io/staticstream.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/actable.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/adobemarker.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/component.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/exifmarker.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/frame.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/huffmantable.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/jfifmarker.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/lscolortrafo.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/quantization.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/quantizationtable.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/restartintervalmarker.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/scan.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/scantypes.hpp"
    "${LIBJPEG_SOURCE_DIR}/marker/thresholds.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/assert.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/ctype.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/errno.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/math.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/setjmp.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/stdarg.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/stddef.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/stdio.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/stdlib.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/string.hpp"
    "${LIBJPEG_SOURCE_DIR}/std/unistd.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/checksum.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/debug.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/environment.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/line.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/numerics.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/priorityqueue.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/rectangle.hpp"
    "${LIBJPEG_SOURCE_DIR}/tools/traits.hpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/downsampler.hpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/interdownsampler.hpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/downsamplerbase.hpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/upsampler.hpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/cositedupsampler.hpp"
    "${LIBJPEG_SOURCE_DIR}/upsampling/upsamplerbase.hpp"
)


if(LIBJPEG_BUILD_TOOL)
  add_subdirectory(cmd)
endif()
