#add_subdirectory(${PROJECT_SOURCE_DIR}/extern/libjpeg-turbo ${CMAKE_BINARY_DIR}/extern/libjpeg-turbo EXCLUDE_FROM_ALL)


add_library(libopenjph 
    encoder.cpp
    decoder.cpp
)

target_link_libraries(libopenjph PRIVATE openjph)
