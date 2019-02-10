#include "myfunction.h"
#include "gtest/gtest.h"

TEST(MyFunction, PositiveNum) {
    int expected = 7;

    ASSERT_EQ(expected, myfunction(6));
}

TEST(MyFunction, NegativeNum) {
    int expected = -7;

    ASSERT_EQ(expected, myfunction(-6));
}
