#include <gtest/gtest.h>

#include "utilities.hpp"
#include <iostream>

TEST(UtilMathTest, MathClampWithNumbers) {
    EXPECT_EQ(mia::math::clamp(5, 1, 10), 5);
    EXPECT_EQ(mia::math::clamp(-5, 0, 10), 0);
    EXPECT_EQ(mia::math::clamp(15, 0, 10), 10);
}

TEST(UtilMathTest, MathLerpWithNumbers) {
    EXPECT_FLOAT_EQ(mia::math::lerp(0.0f, 10.0f, 0.0f), 0.0f);
    EXPECT_FLOAT_EQ(mia::math::lerp(0.0f, 10.0f, 1.0f), 10.0f);
    EXPECT_FLOAT_EQ(mia::math::lerp(0.0f, 10.0f, 0.5f), 5.0f);
}

TEST(UtilMathTest, Random) {
    for (int i = 0; i < 100; i++) {
        float r = mia::math::random();
        EXPECT_GE(r, 0.0f);
        EXPECT_LE(r, 1.0f);
    }
}

TEST(UtilMathTest, RandomRange) {
    for (int i = 0; i < 100; i++) {
        float r = mia::math::random_in_range(5.0f, 15.0f);
        EXPECT_GE(r, 5.0f);
        EXPECT_LE(r, 15.0f);
    }
}

TEST(UtilTets, A) {
    mia::simd_allocator<int> a;
    mia::simd_allocator<int>::pointer p;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
