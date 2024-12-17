#!/bin/bash -e
BUILD_TYPE=${BUILD_TYPE:-Debug}
N_PROC=${N_PROC:-24}
C_COMPILER=gcc-9 CXX_COMPILER=g++-9 CXX_STD=11 COMPILE_ONLY=On ./test.sh
C_COMPILER=gcc-9 CXX_COMPILER=g++-9 CXX_STD=14 COMPILE_ONLY=On ./test.sh
C_COMPILER=gcc-9 CXX_COMPILER=g++-9 CXX_STD=17 COMPILE_ONLY=On ./test.sh
C_COMPILER=gcc-9 CXX_COMPILER=g++-9 CXX_STD=20 COMPILE_ONLY=On ./test.sh
C_COMPILER=gcc-13 CXX_COMPILER=g++-13 CXX_STD=11 ./test.sh
C_COMPILER=gcc-13 CXX_COMPILER=g++-13 CXX_STD=14 ./test.sh
C_COMPILER=gcc-13 CXX_COMPILER=g++-13 CXX_STD=17 ./test.sh
C_COMPILER=gcc-13 CXX_COMPILER=g++-13 CXX_STD=20 ./test.sh
C_COMPILER=gcc-13 CXX_COMPILER=g++-13 CXX_STD=23 ./test.sh
C_COMPILER=clang-14 CXX_COMPILER=clang++-14 USE_LIBCPP=Off CXX_STD=11 COMPILE_ONLY=On ./test.sh
C_COMPILER=clang-14 CXX_COMPILER=clang++-14 USE_LIBCPP=Off CXX_STD=14 COMPILE_ONLY=On ./test.sh
C_COMPILER=clang-14 CXX_COMPILER=clang++-14 USE_LIBCPP=Off CXX_STD=17 COMPILE_ONLY=On ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=Off CXX_STD=11 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=Off CXX_STD=14 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=Off CXX_STD=17 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=Off CXX_STD=20 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=On CXX_STD=11 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=On CXX_STD=14 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=On CXX_STD=17 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=On CXX_STD=20 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=On CXX_STD=23 ./test.sh
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=On CXX_STD=20 BUILD_TYPE=Debug ./test.sh --include-category brute -j $N_PROC
C_COMPILER=clang-18 CXX_COMPILER=clang++-18 USE_LIBCPP=On CXX_STD=20 BUILD_TYPE=Release ./test.sh --include-category perf
echo "---- SUCCESS"
