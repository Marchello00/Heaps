cmake_minimum_required(VERSION 2.8)

project(FibHeap)

set(SOURCE_LIB FibHeap.cpp)

add_library(FibHeap STATIC ${SOURCE_LIB})