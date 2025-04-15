#include "math/vector.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <type_traits>

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

TYPED_TEST(typed_vector_test, init_constructors_assignments) {
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

TYPED_TEST(typed_vector_test, copy_constructor) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    mia::vector<T, Ds> copy(this->vec1);

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(copy[i], this->vec1[i]);
    }

    // Verify deep copy
    copy[0] = static_cast<T>(100);
    EXPECT_NE(copy[0], this->vec1[0]);
}

TYPED_TEST(typed_vector_test, move_constructor) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    mia::vector<T, Ds> original;
    for (size_t i = 0; i < Ds; ++i) {
        original[i] = static_cast<T>(i * 3);
    }

    mia::vector<T, Ds> moved(std::move(original));

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(moved[i], static_cast<T>(i * 3));
    }
}

TYPED_TEST(typed_vector_test, copy_assignment) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    mia::vector<T, Ds> copy;
    copy = this->vec1;

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(copy[i], this->vec1[i]);
    }

    // Verify deep copy
    copy[0] = static_cast<T>(100);
    EXPECT_NE(copy[0], this->vec1[0]);
}

TYPED_TEST(typed_vector_test, move_assignment) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    mia::vector<T, Ds> original;
    for (size_t i = 0; i < Ds; ++i) {
        original[i] = static_cast<T>(i * 3);
    }

    mia::vector<T, Ds> moved;
    moved = std::move(original);

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(moved[i], static_cast<T>(i * 3));
    }
}

TYPED_TEST(typed_vector_test, element_access) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    // Test operator[]
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(this->vec1[i], static_cast<T>(i + 1));

        // Test non-const and const access
        const auto& const_vec = this->vec1;
        EXPECT_EQ(const_vec[i], static_cast<T>(i + 1));
    }
}

TYPED_TEST(typed_vector_test, magnitude_squared) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    T expected = 0;
    for (size_t i = 0; i < Ds; ++i) {
        expected += static_cast<T>((i + 1) * (i + 1));
    }

    EXPECT_NEAR(this->vec1.magnitude_squared(), expected, this->epsilon);
}

TYPED_TEST(typed_vector_test, magnitude) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    if constexpr (std::is_floating_point_v<T>) {
        T sum_squares = 0;
        for (size_t i = 0; i < Ds; ++i) {
            sum_squares += static_cast<T>((i + 1) * (i + 1));
        }
        T expected = std::sqrt(sum_squares);

        EXPECT_NEAR(this->vec1.magnitude(), expected, this->epsilon);
    }
}

TYPED_TEST(typed_vector_test, normalized) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    if constexpr (std::is_floating_point_v<T>) {
        auto normalized = this->vec1.normalized();

        // Magnitude of normalized vector should be 1.0
        EXPECT_NEAR(normalized.magnitude(), static_cast<T>(1.0), this->epsilon);

        // Check direction is preserved
        T mag = this->vec1.magnitude();
        for (size_t i = 0; i < Ds; ++i) {
            EXPECT_NEAR(normalized[i], this->vec1[i] / mag, this->epsilon);
        }
    }
}

TYPED_TEST(typed_vector_test, normalize) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    if constexpr (std::is_floating_point_v<T>) {
        auto normalized = this->vec1;
        normalized.normalize();

        // Magnitude of normalized vector should be 1.0
        EXPECT_NEAR(normalized.magnitude(), static_cast<T>(1.0), this->epsilon);

        // Check direction is preserved
        T mag = this->vec1.magnitude();
        for (size_t i = 0; i < Ds; ++i) {
            EXPECT_NEAR(normalized[i], this->vec1[i] / mag, this->epsilon);
        }
    }
}

TYPED_TEST(typed_vector_test, dot_product) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    T expected = 0;
    for (size_t i = 0; i < Ds; ++i) {
        expected += static_cast<T>((i + 1) * (i * 2));
    }
    
    auto result = mia::vector<T, Ds>::dot_product(this->vec1, this->vec2);

    EXPECT_NEAR(result, expected, this->epsilon);
}

TYPED_TEST(typed_vector_test, hadamard_product) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    auto result = mia::vector<T, Ds>::hadamard_product(this->vec1, this->vec2);

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_NEAR(result[i], this->vec1[i] * this->vec2[i], this->epsilon);
    }
}

/*TYPED_TEST(typed_vector_test, lerp) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    if constexpr (std::is_floating_point_v<T>) {*/
/*        // Test t=0 (should be vec1)*/
/*        auto result0 = this->vec1.lerp(this->vec2, 0.0);*/
/*        for (size_t i = 0; i < Ds; ++i) {*/
/*            EXPECT_NEAR(result0[i], this->vec1[i], this->epsilon);*/
/*        }*/
/**/
/*        // Test t=1 (should be vec2)*/
/*        auto result1 = this->vec1.lerp(this->vec2, 1.0);*/
/*        for (size_t i = 0; i < Ds; ++i) {*/
/*            EXPECT_NEAR(result1[i], this->vec2[i], this->epsilon);*/
/*        }*/
/**/
/*        // Test t=0.5 (should be midpoint)*/
/*        auto result_half = this->vec1.lerp(this->vec2, 0.5);*/
/*        for (size_t i = 0; i < Ds; ++i) {*/
/*            T expected = (this->vec1[i] + this->vec2[i]) / static_cast<T>(2.0);*/
/*            EXPECT_NEAR(result_half[i], expected, this->epsilon);*/
/*        }*/
/*    }*/
/*}*/
/**/
/*TYPED_TEST(typed_vector_test, max_min) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    auto max_result = this->vec1.max(this->vec2);*/
/*    auto min_result = this->vec1.min(this->vec2);*/
/**/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_EQ(max_result[i], std::max(this->vec1[i], this->vec2[i]));*/
/*        EXPECT_EQ(min_result[i], std::min(this->vec1[i], this->vec2[i]));*/
/*    }*/
/*}*/
/**/
/*TYPED_TEST(typed_vector_test, distance_squared) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    T expected = 0;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        T diff = this->vec1[i] - this->vec2[i];*/
/*        expected += diff * diff;*/
/*    }*/
/**/
/*    EXPECT_NEAR(this->vec1.distance_squared(this->vec2), expected, this->epsilon);*/
/*}*/
/**/
/*TYPED_TEST(typed_vector_test, distance) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    if constexpr (std::is_floating_point_v<T>) {*/
/*        T sum_squared_diff = 0;*/
/*        for (size_t i = 0; i < Ds; ++i) {*/
/*            T diff = this->vec1[i] - this->vec2[i];*/
/*            sum_squared_diff += diff * diff;*/
/*        }*/
/*        T expected = std::sqrt(sum_squared_diff);*/
/**/
/*        EXPECT_NEAR(this->vec1.distance(this->vec2), expected, this->epsilon);*/
/*    }*/
/*}*/
/**/
/*TYPED_TEST(typed_vector_test, angle) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    if constexpr (std::is_floating_point_v<T>) {*/
/*        // Compute expected angle*/
/*        T dot = this->vec1.dot_product(this->vec2);*/
/*        T mag1 = this->vec1.magnitude();*/
/*        T mag2 = this->vec2.magnitude();*/
/*        T expected = std::acos(dot / (mag1 * mag2));*/
/**/
/*        EXPECT_NEAR(this->vec1.angle(this->vec2), expected, this->epsilon);*/
/*    }*/
/*}*/
/**/
/*TYPED_TEST(typed_vector_test, operators_arithmetic) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    // Addition*/
/*    auto result_add = this->vec1 + this->vec2;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_add[i], this->vec1[i] + this->vec2[i], this->epsilon);*/
/*    }*/
/**/
/*    // Subtraction*/
/*    auto result_sub = this->vec1 - this->vec2;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_sub[i], this->vec1[i] - this->vec2[i], this->epsilon);*/
/*    }*/
/**/
/*    // Scalar multiplication*/
/*    T scalar = static_cast<T>(2);*/
/*    auto result_mul = this->vec1 * scalar;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_mul[i], this->vec1[i] * scalar, this->epsilon);*/
/*    }*/
/**/
/*    // Scalar multiplication (reverse order)*/
/*    auto result_mul_rev = scalar * this->vec1;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_mul_rev[i], scalar * this->vec1[i], this->epsilon);*/
/*    }*/
/**/
/*    // Scalar division*/
/*    auto result_div = this->vec1 / scalar;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_div[i], this->vec1[i] / scalar, this->epsilon);*/
/*    }*/
/**/
/*    // Negation*/
/*    auto result_neg = -this->vec1;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_neg[i], -this->vec1[i], this->epsilon);*/
/*    }*/
/*}*/
/**/
/*TYPED_TEST(typed_vector_test, operators_compound_assignment) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    // Addition assignment*/
/*    auto result_add = this->vec1;*/
/*    result_add += this->vec2;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_add[i], this->vec1[i] + this->vec2[i], this->epsilon);*/
/*    }*/
/**/
/*    // Subtraction assignment*/
/*    auto result_sub = this->vec1;*/
/*    result_sub -= this->vec2;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_sub[i], this->vec1[i] - this->vec2[i], this->epsilon);*/
/*    }*/
/**/
/*    // Scalar multiplication assignment*/
/*    T scalar = static_cast<T>(2);*/
/*    auto result_mul = this->vec1;*/
/*    result_mul *= scalar;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_mul[i], this->vec1[i] * scalar, this->epsilon);*/
/*    }*/
/**/
/*    // Scalar division assignment*/
/*    auto result_div = this->vec1;*/
/*    result_div /= scalar;*/
/*    for (size_t i = 0; i < Ds; ++i) {*/
/*        EXPECT_NEAR(result_div[i], this->vec1[i] / scalar, this->epsilon);*/
/*    }*/
/*}*/
/**/
/*TYPED_TEST(typed_vector_test, operators_comparison) {*/
/*    using T = typename TestFixture::type;*/
/*    constexpr size_t Ds = TestFixture::dims;*/
/**/
/*    // Create identical vectors*/
/*    mia::vector<T, Ds> vec1_copy = this->vec1;*/
/**/
/*    // Test equality*/
/*    EXPECT_TRUE(this->vec1 == vec1_copy);*/
/*    EXPECT_FALSE(this->vec1 == this->vec2);*/
/**/
/*    // Test inequality*/
/*    EXPECT_FALSE(this->vec1 != vec1_copy);*/
/*    EXPECT_TRUE(this->vec1 != this->vec2);*/
/**/
/*    // Modify one element and test again*/
/*    vec1_copy[0] += static_cast<T>(1);*/
/*    EXPECT_FALSE(this->vec1 == vec1_copy);*/
/*    EXPECT_TRUE(this->vec1 != vec1_copy);*/
/*}*/
/**/
auto main(int argc, char** argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
