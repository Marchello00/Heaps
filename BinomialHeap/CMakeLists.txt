cmake_minimum_required(VERSION 2.8)

project(BinomialHeap)

set(SOURCE_LIB BinomialHeap.cpp)

add_library(BinomialHeap STATIC ${SOURCE_LIB})