cmake_minimum_required(VERSION 2.8)

project(PriorityQueue)

set(SOURCE_LIB PriorityQueue.cpp)

add_library(PriorityQueue STATIC ${SOURCE_LIB})