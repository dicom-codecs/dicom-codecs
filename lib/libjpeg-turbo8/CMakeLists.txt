#add_subdirectory(${PROJECT_SOURCE_DIR}/extern/libjpeg-turbo ${CMAKE_BINARY_DIR}/extern/libjpeg-turbo EXCLUDE_FROM_ALL)

add_library(libjpeg-turbo8 
    encoder.cpp
    decoder.cpp
)

target_link_libraries(libjpeg-turbo8 PRIVATE turbojpeg-static)
