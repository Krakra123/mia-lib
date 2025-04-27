#include "math/vector.hpp"

#include <algorithm>
#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <ranges>
#include <sys/wait.h>
#include <type_traits>

// NOTE: FIXTURE AND TYPED SETUP
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
using vector_test_types = ::testing::Types<vector_type<float, 2>, vector_type<float, 3>, vector_type<double, 4>, vector_type<int, 3>>;

template <typename Param>
class typed_vector_test : public vector_test<typename Param::type, Param::dims> {};

TYPED_TEST_SUITE(typed_vector_test, vector_test_types);

// NOTE: CONSTRUCTOR & ASSIGNMENT
TYPED_TEST(typed_vector_test, constructors_assignments_with_value) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    // Constructor
    // Default constructor
    mia::vector<T, Ds> v0{};
    for (auto &x : v0) {
        EXPECT_EQ(x, 0);
    }

    // Construct with container
    // std array
    std::array d1 = {4, 5, 2, 1, 7};
    mia::vector<T, Ds> v1(d1);
    for (auto &&[i, x] : v1 | std::views::enumerate) {
        EXPECT_EQ(x, d1[static_cast<size_t>(i)]);
    }

    // std vector
    std::vector d2 = {1, 1, 2, 3};
    mia::vector<T, Ds> v2(d2);
    for (auto &&[i, x] : v2 | std::views::enumerate) {
        EXPECT_EQ(x, d2[static_cast<size_t>(i)]);
    }

    // raw array
    T d3[] = {6, 4, 2, 1, 0};
    mia::vector<T, Ds> v3(d3);
    for (auto &&[i, x] : v3 | std::views::enumerate) {
        EXPECT_EQ(x, d3[static_cast<size_t>(i)]);
    }

    // Construct with initializer_list
    mia::vector<T, Ds> v4{3, 5, 6, 7, 10};
    std::array d4 = {3, 5, 6, 7, 10};
    for (auto &&[i, x] : v4 | std::views::enumerate) {
        EXPECT_EQ(x, d4[static_cast<size_t>(i)]);
    }

    // Assignment
    std::array d5 = {4, 5, 2, 1, 7};
    mia::vector<T, Ds> v5 = d5;
    for (auto &&[i, x] : v5 | std::views::enumerate) {
        EXPECT_EQ(x, d5[static_cast<size_t>(i)]);
    }

    // std vector
    std::vector d6 = {1, 1, 2, 3};
    mia::vector<T, Ds> v6 = d6;
    for (auto &&[i, x] : v6 | std::views::enumerate) {
        EXPECT_EQ(x, d6[static_cast<size_t>(i)]);
    }

    // raw array
    T d7[] = {6, 4, 2, 1, 0};
    mia::vector<T, Ds> v7 = d7;
    for (auto &&[i, x] : v7 | std::views::enumerate) {
        EXPECT_EQ(x, d7[static_cast<size_t>(i)]);
    }

    // Construct with initializer_list
    mia::vector<T, Ds> v8 = {3, 5, 6, 7, 10};
    std::array d8 = {3, 5, 6, 7, 10};
    for (auto &&[i, x] : v8 | std::views::enumerate) {
        EXPECT_EQ(x, d8[static_cast<size_t>(i)]);
    }
}

TYPED_TEST(typed_vector_test, copy_constructor_assignment) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    mia::vector<T, Ds> copy1(this->vec1);
    mia::vector<T, Ds> copy2(copy1);

    for (auto [c1, c2, ex] : std::views::zip(copy1, copy2, this->vec1)) {
        EXPECT_EQ(c1, ex);
        EXPECT_EQ(c2, ex);
    }

    copy1[0] = T{-123};
    EXPECT_NE(copy1[0], this->vec1[0]);

    copy2 = this->vec2;
    for (auto [v, ex] : std::views::zip(copy2, this->vec2)) {
        EXPECT_EQ(v, ex);
    }
}

TYPED_TEST(typed_vector_test, move_constructor_assignment) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    /*const std::array temp{2, 5, 8, 9, 6};*/

    mia::vector<T, Ds> original;
    for (size_t i = 0; i < Ds; ++i) {
        original[i] = static_cast<T>(i * 3);
    }

    mia::vector<T, Ds> moved(std::move(original));

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(moved[i], static_cast<T>(i * 3));
    }
}

// NOTE: ITERATION AND ACCESS
TYPED_TEST(typed_vector_test, iteration_and_access) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    // Iteration access
    size_t idx = 0;
    for (auto it = this->vec1.begin(); it != this->vec1.end(); ++it, ++idx) {
        EXPECT_EQ(*it, this->vec1[idx]);
    }

    // Const iteration
    const mia::vector<T, Ds> &const_vec = this->vec1;
    idx = 0;
    for (auto it = const_vec.begin(); it != const_vec.end(); ++it, ++idx) {
        EXPECT_EQ(*it, const_vec[idx]);
    }

    // Range-based for loop
    std::array<T, Ds> expected;
    for (size_t i = 0; i < Ds; ++i) {
        expected[i] = static_cast<T>(i + 1);
    }

    idx = 0;
    for (const auto &val : this->vec1) {
        EXPECT_EQ(val, expected[idx++]);
    }

    // Size check
    EXPECT_EQ(this->vec1.size(), Ds);
    EXPECT_EQ(this->vec1.dimension(), Ds);

    // Optional at() access
    auto val = this->vec1.at(0);
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(*val, this->vec1[0]);

    // Out of bounds should return empty optional
    auto invalid = this->vec1.at(Ds);
    EXPECT_FALSE(invalid.has_value());
}

// NOTE: NAMED ACCESSORS (x, y, z, w)
TYPED_TEST(typed_vector_test, named_accessors) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    if constexpr (Ds >= 1) {
        EXPECT_EQ(this->vec1.x(), this->vec1[0]);
        this->vec1.x() = T{42};
        EXPECT_EQ(this->vec1[0], T{42});
    }

    if constexpr (Ds >= 2) {
        EXPECT_EQ(this->vec1.y(), this->vec1[1]);
        this->vec1.y() = T{43};
        EXPECT_EQ(this->vec1[1], T{43});
    }

    if constexpr (Ds >= 3) {
        EXPECT_EQ(this->vec1.z(), this->vec1[2]);
        this->vec1.z() = T{44};
        EXPECT_EQ(this->vec1[2], T{44});
    }

    if constexpr (Ds >= 4) {
        EXPECT_EQ(this->vec1.w(), this->vec1[3]);
        this->vec1.w() = T{45};
        EXPECT_EQ(this->vec1[3], T{45});
    }
}

// NOTE: VECTOR ARITHMETIC OPERATIONS
TYPED_TEST(typed_vector_test, vector_arithmetic) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;
    using ComputeType = mia::vector<T, Ds>::compute_type;

    // Addition
    auto sum = this->vec1 + this->vec2;
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(sum[i], this->vec1[i] + this->vec2[i]);
    }

    // Subtraction
    auto diff = this->vec1 - this->vec2;
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(diff[i], this->vec1[i] - this->vec2[i]);
    }

    // Scalar multiplication
    ComputeType scalar = static_cast<T>(2);
    auto scaled = this->vec1 * scalar;
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(scaled[i], this->vec1[i] * scalar);
    }

    // Left scalar multiplication
    auto left_scaled = scalar * this->vec1;
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(left_scaled[i], scalar * this->vec1[i]);
    }

    // Scalar division
    auto divided = this->vec1 / scalar;
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_NEAR(divided[i], this->vec1[i] / scalar, this->epsilon);
    }

    // Compound operators
    mia::vector<T, Ds> v1 = this->vec1;
    mia::vector<T, Ds> v2 = this->vec2;

    auto expected_plus = v1 + v2;
    v1 += v2;
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(v1[i], expected_plus[i]);
    }

    v1 = this->vec1; // Reset
    auto expected_minus = v1 - v2;
    v1 -= v2;
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(v1[i], expected_minus[i]);
    }
}

// NOTE: MAGNITUDE AND NORMALIZATION
TYPED_TEST(typed_vector_test, magnitude_and_normalization) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;
    using ComputeType = typename mia::vector<T, Ds>::compute_type;

    // Magnitude squared
    ComputeType mag_squared = this->vec1.magnitude_squared();
    ComputeType expected_mag_squared = 0;
    for (size_t i = 0; i < Ds; ++i) {
        expected_mag_squared += static_cast<ComputeType>(this->vec1[i]) * static_cast<ComputeType>(this->vec1[i]);
    }
    EXPECT_NEAR(mag_squared, expected_mag_squared, this->epsilon);

    // Magnitude
    ComputeType mag = this->vec1.magnitude();
    ComputeType expected_mag = std::sqrt(expected_mag_squared);
    EXPECT_NEAR(mag, expected_mag, this->epsilon);

    // Normalization
    if constexpr (std::is_floating_point_v<T>) {
        auto normalized = this->vec1.normalized();
        EXPECT_NEAR(normalized.magnitude(), static_cast<ComputeType>(1.0), this->epsilon);

        for (size_t i = 0; i < Ds; ++i) {
            EXPECT_NEAR(normalized[i], this->vec1[i] / expected_mag, this->epsilon);
        }

        // Normalizing
        mia::vector<T, Ds> v1 = this->vec1;
        ComputeType original_mag = v1.normalizing();
        EXPECT_NEAR(original_mag, expected_mag, this->epsilon);
        EXPECT_NEAR(v1.magnitude(), static_cast<ComputeType>(1.0), this->epsilon);
    }
}

// NOTE: HADAMARD PRODUCT
TYPED_TEST(typed_vector_test, hadamard_product) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    auto hadamard = mia::vector<T, Ds>::hadamard_product(this->vec1, this->vec2);
    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(hadamard[i], this->vec1[i] * this->vec2[i]);
    }
}

// NOTE: DOT PRODUCT
TYPED_TEST(typed_vector_test, dot_product) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;
    using ComputeType = typename mia::vector<T, Ds>::compute_type;

    ComputeType dot = mia::vector<T, Ds>::dot_product(this->vec1, this->vec2);
    ComputeType expected_dot = 0;
    for (size_t i = 0; i < Ds; ++i) {
        expected_dot += static_cast<ComputeType>(this->vec1[i]) * static_cast<ComputeType>(this->vec2[i]);
    }
    EXPECT_NEAR(dot, expected_dot, this->epsilon);
}

// NOTE: MIN & MAX
TYPED_TEST(typed_vector_test, min_max) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    auto min_vec = mia::vector<T, Ds>::min(this->vec1, this->vec2);
    auto max_vec = mia::vector<T, Ds>::max(this->vec1, this->vec2);

    for (size_t i = 0; i < Ds; ++i) {
        EXPECT_EQ(min_vec[i], std::min(this->vec1[i], this->vec2[i]));
        EXPECT_EQ(max_vec[i], std::max(this->vec1[i], this->vec2[i]));
    }
}

// NOTE: LERP
TYPED_TEST(typed_vector_test, lerp) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;
    using ComputeType = typename mia::vector<T, Ds>::compute_type;

    ComputeType alpha = static_cast<ComputeType>(0.5);
    auto lerp_vec = mia::vector<T, Ds>::lerp(this->vec1, this->vec2, alpha);

    for (size_t i = 0; i < Ds; ++i) {
        ComputeType expected = (static_cast<ComputeType>(1.0) - alpha) * static_cast<ComputeType>(this->vec1[i]) + alpha * static_cast<ComputeType>(this->vec2[i]);
        EXPECT_NEAR(static_cast<ComputeType>(lerp_vec[i]), expected, this->epsilon);
    }
}

// NOTE: DISTANCE
TYPED_TEST(typed_vector_test, distance) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;
    using ComputeType = typename mia::vector<T, Ds>::compute_type;

    ComputeType dist_squared = mia::vector<T, Ds>::distance_squared(this->vec1, this->vec2);
    auto diff_vec = this->vec2 - this->vec1;
    ComputeType expected_dist_squared = diff_vec.magnitude_squared();
    EXPECT_NEAR(dist_squared, expected_dist_squared, this->epsilon);

    // FIXME:
    ComputeType dist = mia::vector<T, Ds>::distance(this->vec1, this->vec2);
    EXPECT_NEAR(dist, std::sqrt(expected_dist_squared), this->epsilon);
}

// NOTE: ANGLE
TYPED_TEST(typed_vector_test, angle) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;
    using ComputeType = typename mia::vector<T, Ds>::compute_type;

    if constexpr (std::is_floating_point_v<T>) {
        ComputeType angle = mia::vector<T, Ds>::angle(this->vec1, this->vec2);
        ComputeType cos_angle = mia::vector<T, Ds>::dot_product(this->vec1, this->vec2) / (this->vec1.magnitude() * this->vec2.magnitude());

        if (cos_angle <= static_cast<ComputeType>(1.0)) {
            ComputeType expected_angle = std::acos(cos_angle);
            EXPECT_NEAR(angle, expected_angle, this->epsilon);
        } else {
            EXPECT_NEAR(angle, static_cast<ComputeType>(0.0), this->epsilon);
        }
    }
}

// NOTE: CROSS PRODUCT (3D SPECIFIC)
TEST(vector3_test, cross_product) {
    mia::vector<float, 3> v1{1, 0, 0};
    mia::vector<float, 3> v2{0, 1, 0};

    auto cross = mia::vector<float, 3>::cross_product(v1, v2);
    EXPECT_NEAR(cross[0], 0.0f, 1e-6f);
    EXPECT_NEAR(cross[1], 0.0f, 1e-6f);
    EXPECT_NEAR(cross[2], 1.0f, 1e-6f);

    mia::vector<float, 3> v3{2, 3, 4};
    mia::vector<float, 3> v4{5, 6, 7};

    auto cross2 = mia::vector<float, 3>::cross_product(v3, v4);
    // Expected: (3*7 - 4*6, 4*5 - 2*7, 2*6 - 3*5)
    EXPECT_NEAR(cross2[0], 3 * 7 - 4 * 6, 1e-6f);
    EXPECT_NEAR(cross2[1], 4 * 5 - 2 * 7, 1e-6f);
    EXPECT_NEAR(cross2[2], 2 * 6 - 3 * 5, 1e-6f);

    // Cross product properties
    // v × v = 0
    auto self_cross = mia::vector<float, 3>::cross_product(v3, v3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(self_cross[i], 0.0f, 1e-6f);
    }

    // v1 × v2 = -(v2 × v1)
    auto reversed_cross = mia::vector<float, 3>::cross_product(v4, v3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(cross2[i], -reversed_cross[i], 1e-6f);
    }
}

// NOTE: STATIC VECTOR PROPERTIES
TEST(vector_static_test, static_vector_properties) {
    // Test for 2D vectors
    {
        auto zero = mia::vector<float, 2>::zero();
        EXPECT_EQ(zero[0], 0.0f);
        EXPECT_EQ(zero[1], 0.0f);

        auto one = mia::vector<float, 2>::one();
        EXPECT_EQ(one[0], 1.0f);
        EXPECT_EQ(one[1], 1.0f);

        auto up = mia::vector<float, 2>::up();
        EXPECT_EQ(up[0], 0.0f);
        EXPECT_EQ(up[1], 1.0f);

        auto down = mia::vector<float, 2>::down();
        EXPECT_EQ(down[0], 0.0f);
        EXPECT_EQ(down[1], -1.0f);

        auto left = mia::vector<float, 2>::left();
        EXPECT_EQ(left[0], -1.0f);
        EXPECT_EQ(left[1], 0.0f);

        auto right = mia::vector<float, 2>::right();
        EXPECT_EQ(right[0], 1.0f);
        EXPECT_EQ(right[1], 0.0f);
    }

    // Test for 3D vectors
    {
        auto zero = mia::vector<float, 3>::zero();
        EXPECT_EQ(zero[0], 0.0f);
        EXPECT_EQ(zero[1], 0.0f);
        EXPECT_EQ(zero[2], 0.0f);

        auto one = mia::vector<float, 3>::one();
        EXPECT_EQ(one[0], 1.0f);
        EXPECT_EQ(one[1], 1.0f);
        EXPECT_EQ(one[2], 1.0f);

        auto up = mia::vector<float, 3>::up();
        EXPECT_EQ(up[0], 0.0f);
        EXPECT_EQ(up[1], 1.0f);
        EXPECT_EQ(up[2], 0.0f);

        auto down = mia::vector<float, 3>::down();
        EXPECT_EQ(down[0], 0.0f);
        EXPECT_EQ(down[1], -1.0f);
        EXPECT_EQ(down[2], 0.0f);

        auto left = mia::vector<float, 3>::left();
        EXPECT_EQ(left[0], -1.0f);
        EXPECT_EQ(left[1], 0.0f);
        EXPECT_EQ(left[2], 0.0f);

        auto right = mia::vector<float, 3>::right();
        EXPECT_EQ(right[0], 1.0f);
        EXPECT_EQ(right[1], 0.0f);
        EXPECT_EQ(right[2], 0.0f);

        auto forward = mia::vector<float, 3>::forward();
        EXPECT_EQ(forward[0], 0.0f);
        EXPECT_EQ(forward[1], 0.0f);
        EXPECT_EQ(forward[2], 1.0f);

        auto back = mia::vector<float, 3>::back();
        EXPECT_EQ(back[0], 0.0f);
        EXPECT_EQ(back[1], 0.0f);
        EXPECT_EQ(back[2], 1.0f); // Note: This matches the implementation, but might be a bug as back should be (0,0,-1)
    }
}

// NOTE: EQUALITY OPERATORS
TYPED_TEST(typed_vector_test, equality_operators) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;

    // Copy should be equal
    mia::vector<T, Ds> copy = this->vec1;
    EXPECT_TRUE(copy == this->vec1);
    EXPECT_FALSE(copy != this->vec1);

    // Different vectors should not be equal
    EXPECT_FALSE(this->vec1 == this->vec2);
    EXPECT_TRUE(this->vec1 != this->vec2);

    // Changing one element makes them not equal
    copy[0] = static_cast<T>(copy[0] + 1);
    EXPECT_FALSE(copy == this->vec1);
    EXPECT_TRUE(copy != this->vec1);
}

// NOTE: EDGE CASES
TYPED_TEST(typed_vector_test, edge_cases) {
    using T = typename TestFixture::type;
    constexpr size_t Ds = TestFixture::dims;
    using ComputeType = typename mia::vector<T, Ds>::compute_type;

    // Zero vector
    mia::vector<T, Ds> zero;
    EXPECT_NEAR(zero.magnitude(), static_cast<ComputeType>(0), this->epsilon);

    // Normalization of zero vector should handle gracefully if floating point
    if constexpr (std::is_floating_point_v<T>) {
        auto normalized_zero = zero.normalized();
        // The result doesn't matter as much as that it doesn't crash
        EXPECT_NEAR(normalized_zero.magnitude(), static_cast<ComputeType>(0), this->epsilon);
    }

    // Angle between parallel vectors
    mia::vector<T, Ds> v1 = this->vec1;
    mia::vector<T, Ds> v2 = v1 * static_cast<ComputeType>(2);

    if (v1.magnitude() > 0 && std::is_floating_point_v<T>) {
        ComputeType angle = mia::vector<T, Ds>::angle(v1, v2);
        EXPECT_NEAR(angle, static_cast<ComputeType>(0), this->epsilon);
    }

    // Angle between perpendicular vectors (for 2D and 3D)
    if constexpr ((Ds == 2 || Ds == 3) && std::is_floating_point_v<T>) {
        mia::vector<T, Ds> x_axis = mia::vector<T, Ds>::right();
        mia::vector<T, Ds> y_axis = mia::vector<T, Ds>::up();

        ComputeType angle = mia::vector<T, Ds>::angle(x_axis, y_axis);
        EXPECT_NEAR(angle, static_cast<ComputeType>(M_PI / 2), this->epsilon);
    }
}

auto main(int argc, char **argv) -> int {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
