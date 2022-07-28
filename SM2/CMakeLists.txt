cmake_minimum_required(VERSION 3.0)
project(sm2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 ")

include_directories (/usr/include/miracl/)


find_library(LIBMIRACL miracl)      


add_executable(sm2 main.cpp sm3.h miracl.h mirdef.h SM2_sign.cpp SM2_sign.h  sm3.cpp)

target_link_libraries(sm2 ${LIBMIRACL})
