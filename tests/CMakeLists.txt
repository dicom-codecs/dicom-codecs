
# Tests need to be added as executables first
add_executable(cpptest 
    main.cpp
    readFile.cpp
    readFileAndDecode.cpp
    printImage.cpp
    roundTrip.cpp
    readFileAndRoundTrip.cpp
    benchmark.cpp
    writeFile.cpp
    readRawFile.cpp
    makeRawFileName.cpp
)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries(cpptest PRIVATE dicomcodecs)

#C++ 14
target_compile_features(cpptest PUBLIC cxx_std_14)
