
# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${dicomcodecs_SOURCE_DIR}/include/dicomcodecs/*.hpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(dicomcodecs 
    ${HEADER_LIST}
    lib.cpp 
    openjpegdecoder.cpp
    charlsdecoder.cpp)

# We need this directory, and users of our library will need it too
target_include_directories(dicomcodecs PUBLIC ../include)

# This depends on (header only) boost
target_link_libraries(dicomcodecs 
    PRIVATE openjp2
    PRIVATE charls) 

# All users of this library will need at least C++11
target_compile_features(dicomcodecs PUBLIC cxx_std_11)

include_directories("../extern/openjpeg/src/lib/openjp2" "../build/extern/openjpeg/src/lib/openjp2")

# IDEs should put the headers in a nice place
#source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
