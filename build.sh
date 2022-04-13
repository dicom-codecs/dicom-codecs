#!/bin/bash

RED='\033[0;31m'
NC='\033[0m' # No Color

rm -rf build
mkdir -p build
#(cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..)
(cd build && cmake ..)
retVal=$?
if [ $retVal -ne 0 ]; then
    echo "${RED}CMAKE FAILED${NC}"
    exit 1
fi

#(cd build && make -j $NPROC)
(cd build && make VERBOSE=1 -j)
retVal=$?
if [ $retVal -ne 0 ]; then
    echo "${RED}MAKE FAILED${NC}"
    exit 1
fi
(build/tests/cpptest 20)  