cmake_minimum_required(VERSION 3.12)

project(Lab1)

set(CMAKE_CXX_STANDARD 11)

if(MINGW)
    set(GTEST_DISABLE_PTHREADS ON)
endif()

if(WIN32)
    set(WINDOWS_CMAKE_ARGS
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
            )
else(WIN32)
    set(WINDOWS_CMAKE_ARGS )
endif(WIN32)


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
