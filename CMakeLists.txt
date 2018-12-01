cmake_minimum_required(VERSION 3.12)

project(Lab1)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        Array
        List
        PriorityQueue
        Heap
        BinomialHeap
        FibHeap)

include_directories(Testing)

add_subdirectory(Array)
add_subdirectory(List)
add_subdirectory(PriorityQueue)
add_subdirectory(Heap)
add_subdirectory(BinomialHeap)
add_subdirectory(FibHeap)

enable_testing()
add_subdirectory(Testing)
