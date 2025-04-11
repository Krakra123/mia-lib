
#include "math/vector.hpp"

#include <gtest/gtest.h>

#include <array>

// Fixture and typed test
template <typename T, size_t Ds>
class vector_test : public ::testing::Test {
public:
    using type = T;
    static constexpr size_t dims = Ds;

protected:
    void SetUp() override {
        for (size_t i = 0; i < Ds; ++i) {
            vec1[i] = static_cast<T>(i + 1);
            vec2[i] = static_cast<T>(i * 2);
        }
    }

    mia::vector<T, Ds> vec1;
    mia::vector<T, Ds> vec2;
    const T epsilon = std::is_floating_point_v<T> ? static_cast<T>(1e-6) : static_cast<T>(0);
};

template <typename T, size_t Ds>
struct vector_type {
    using type = T;
    static constexpr size_t dims = Ds;
};
using vector_test_types =
    ::testing::Types<vector_type<float, 2>, vector_type<float, 3>, vector_type<float, 4>,
                     vector_type<double, 2>, vector_type<double, 3>, vector_type<double, 4>,
                     vector_type<int, 2>, vector_type<int, 3>, vector_type<int, 4> >;

template <typename Param>
class typed_vector_test : public vector_test<typename Param::type, Param::dims> {};

TYPED_TEST_SUITE(typed_vector_test, vector_test_types);

TYPED_TEST(typed_vector_test, constructors) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    T a[] = {1, 2, 3, 4, 5};
    T b[] = {6, 7, 8, 9};
    mia::vector<T, Ds> v1;
    mia::vector<T, Ds> v2(std::data(a));
    mia::vector<T, Ds> v3({10, 20, 30, 40, 50});
    mia::vector<T, Ds> v4 = std::data(b);
    mia::vector<T, Ds> v5 = {60, 70, 80, 90};

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(v1[i], static_cast<T>(0));
        EXPECT_EQ(v2[i], static_cast<T>(a[i]));
        EXPECT_EQ(v3[i], static_cast<T>(a[i] * 10));
        EXPECT_EQ(v4[i], static_cast<T>(b[i]));
        EXPECT_EQ(v5[i], static_cast<T>(b[i] * 10));
    }
}

auto main(int argc, char** argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
