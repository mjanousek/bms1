cmake_minimum_required(VERSION 3.3)
project(bms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    rscode-1.3/berlekamp.c
    rscode-1.3/crcgen.c
    rscode-1.3/ecc.h
    rscode-1.3/example.c
    rscode-1.3/galois.c
    rscode-1.3/rs.c)

add_executable(bms ${SOURCE_FILES})