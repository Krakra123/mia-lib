#include "math/utilities.hpp"

#include <gtest/gtest.h>

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

TEST(UtilTets, A) {
    mia::simd_allocator<int> a;
    [[maybe_unused]] mia::simd_allocator<int>::pointer p;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
