cmake_minimum_required(VERSION 3.12)

add_executable(
        unit_tests
        mainh.cpp)

target_link_libraries(
        unit_tests
        gtest_main
        Heap
)

add_test(
        NAME
        unit
        COMMAND
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
)