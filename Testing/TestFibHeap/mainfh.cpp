#include "gtest/gtest.h"

#include "FibHeap.test"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}