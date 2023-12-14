add_library(libopenjph
    encoder.cpp
    decoder.cpp
)
target_compile_features(libopenjph PUBLIC cxx_std_11)

target_link_libraries(libopenjph PRIVATE openjph)
