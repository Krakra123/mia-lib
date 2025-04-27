#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <optional>
#include <ranges>
#include <type_traits>

namespace mia {

// FIXME: may failed on some edge case if value ~0
template <typename T, size_t Dims>
    requires std::is_arithmetic_v<T>
class vector {
  public:
    std::array<T, Dims> data;

    // NOTE: MEMBER TYPES

    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = value_type *;
    using const_iterator = const value_type *;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // NOTE: TYPES

    using compute_type = std::conditional_t<
        std::is_floating_point_v<T> && (sizeof(T) * 8 >= 64),
        float, T>;

    // NOTE: ITERATION

    constexpr auto begin() noexcept -> iterator {
        return data.begin();
    }
    constexpr auto begin() const noexcept -> const_iterator {
        return data.begin();
    }
    constexpr auto cbegin() const noexcept -> const_iterator {
        return data.cbegin();
    }
    constexpr auto end() noexcept -> iterator {
        return data.end();
    }
    constexpr auto end() const noexcept -> const_iterator {
        return data.end();
    }
    constexpr auto cend() const noexcept -> const_iterator {
        return data.cend();
    }
    [[nodiscard]] constexpr auto size() const noexcept -> size_type {
        return Dims;
    }
    [[nodiscard]] constexpr auto dimension() const noexcept -> size_type {
        return size();
    }

    // NOTE: DESTRUCTOR

    // NOTE: CONSTRUCTOR

    // :: Constructor with value
    // Default constructor
    constexpr vector() {
        data.fill(T{});
    }

    // Constructor with containers
    template <std::ranges::contiguous_range Container>
        requires std::is_convertible_v<std::ranges::range_reference_t<Container>, value_type>
    constexpr vector(const Container &container) {
        std::span container_span = container;

        assert(container_span.size() >= size());

        auto transformed_range = container_span
                                 | std::views::take(size())
                                 | std::views::transform([](auto &&v) { return static_cast<T>(v); });
        std::ranges::copy(transformed_range, data.begin());
    }

    // Constructor with initializer_list
    template <typename U>
        requires std::is_convertible_v<U, value_type>
    constexpr vector(std::initializer_list<U> list) {
        assert(list.size() >= size());

        auto transformed_range = list
                                 | std::views::take(size())
                                 | std::views::transform([](U v) { return static_cast<T>(v); });
        std::ranges::copy(transformed_range, begin());
    }

    // :: Copy contructor
    template <typename U, size_t N>
        requires std::is_convertible_v<U, value_type> && (N >= Dims)
    constexpr vector(const vector<U, N> &other) {
        auto transformed_range = other
                                 | std::views::transform([](U v) { return static_cast<T>(v); });
        std::ranges::copy(transformed_range, begin());
    }
    constexpr vector(const vector &other) {
        std::ranges::copy(other, begin());
    }

    // :: Move constructor
    constexpr vector(vector &&other) noexcept {
        std::ranges::move(other, begin());
    }

    // NOTE: ASSIGNMENT

    // :: Assignment with value
    // Assignment with containers
    template <std::ranges::contiguous_range Container>
        requires std::is_convertible_v<std::ranges::range_reference_t<Container>, value_type>
    constexpr auto operator=(const Container &container) -> vector & {
        return *this(container);
    }

    // Assignment with initializer_list
    constexpr auto operator=(std::initializer_list<T> list) -> vector & {
        return *this = vector(list);
    }

    // :: Copy assignment
    template <typename U, size_t N>
        requires std::is_convertible_v<U, value_type> && (N >= Dims)
    constexpr auto operator=(const vector<U, N> &other) -> vector & {
        auto transformed_range = other
                                 | std::views::transform([](U v) { return static_cast<T>(v); });
        std::ranges::copy(transformed_range, begin());
        return *this;
    }
    constexpr auto operator=(const vector &other) -> vector & {
        if (&other == this)
            return *this;
        std::ranges::copy(other, begin());
        return *this;
    }

    // :: Move assignment
    constexpr auto operator=(vector &&other) noexcept -> vector & {
        std::ranges::move(other, begin());
        return *this;
    }

    // NOTE: ELEMENT ACCESS

    constexpr auto operator[](const size_t i) -> reference {
        return data[i];
    }
    constexpr auto operator[](const size_t i) const -> const_reference {
        return data[i];
    }
    constexpr auto at(const size_t i) -> std::optional<std::reference_wrapper<value_type>> {
        if (i >= size()) {
            return {};
        }
        return data[i];
    }
    constexpr auto at(const size_t i) const -> std::optional<std::reference_wrapper<const value_type>> {
        if (i >= size()) {
            return {};
        }
        return data[i];
    }

    // :: Named accessors
    constexpr auto x() -> value_type &
        requires(Dims >= 1)
    {
        return data[0];
    }
    constexpr auto x() const -> const value_type &
        requires(Dims >= 1)
    {
        return data[0];
    }
    constexpr auto y() -> value_type &
        requires(Dims >= 2)
    {
        return data[1];
    }
    constexpr auto y() const -> const value_type &
        requires(Dims >= 2)
    {
        return data[1];
    }
    constexpr auto z() -> value_type &
        requires(Dims >= 3)
    {
        return data[2];
    }
    constexpr auto z() const -> const value_type &
        requires(Dims >= 3)
    {
        return data[2];
    }
    constexpr auto w() -> value_type &
        requires(Dims >= 4)
    {
        return data[3];
    }
    constexpr auto w() const -> const value_type &
        requires(Dims >= 4)
    {
        return data[3];
    }

    // NOTE: CONST FUNCTIONS

    // Magnitude & Magnitude squared
    [[nodiscard]] constexpr auto magnitude_squared() const -> compute_type {
        return dot_product(*this, *this);
    }
    // TODO: Make constexpr after C++26
    [[nodiscard]] /*constexpr*/ auto magnitude() const -> compute_type {
        return static_cast<compute_type>(std::sqrt(magnitude_squared()));
    }

    // TODO: Make constexpr after C++26
    inline auto normalized() const -> vector {
        vector result = *this;
        compute_type _magnitude = magnitude();
        for (auto &v : result) {
            v *= static_cast<value_type>(1 / _magnitude);
        }
        return result;
    }

    // NOTE: MODIFIERS

    // Normalize this vector
    // @return Return magnitude of the vector before normalizing
    // TODO: Make constexpr after C++26
    inline auto normalizing() -> value_type {
        compute_type _magnitude = magnitude();
        for (auto &v : data) {
            v *= static_cast<value_type>(1 / _magnitude);
        }
        return _magnitude;
    }

    // NOTE: STATIC PROPERTIES
    static constexpr auto zero() noexcept -> vector
        requires(Dims == 2 || Dims == 3)
    {
        if constexpr (Dims == 2) {
            return vector{0, 0};
        } else {
            return vector{0, 0, 0};
        }
    }
    static constexpr auto one() noexcept -> vector
        requires(Dims == 2 || Dims == 3)
    {
        if constexpr (Dims == 2) {
            return vector{1, 1};
        } else {
            return vector{1, 1, 1};
        }
    }
    static constexpr auto up() noexcept -> vector
        requires(Dims == 2 || Dims == 3)
    {
        if constexpr (Dims == 2) {
            return vector{0, 1};
        } else {
            return vector{0, 1, 0};
        }
    }
    static constexpr auto down() noexcept -> vector
        requires(Dims == 2 || Dims == 3)
    {
        if constexpr (Dims == 2) {
            return vector{0, -1};
        } else {
            return vector{0, -1, 0};
        }
    }
    static constexpr auto left() noexcept -> vector
        requires(Dims == 2 || Dims == 3)
    {
        if constexpr (Dims == 2) {
            return vector{-1, 0};
        } else {
            return vector{-1, 0, 0};
        }
    }
    static constexpr auto right() noexcept -> vector
        requires(Dims == 2 || Dims == 3)
    {
        if constexpr (Dims == 2) {
            return vector{1, 0};
        } else {
            return vector{1, 0, 0};
        }
    }
    static constexpr auto forward() noexcept -> vector
        requires(Dims == 3)
    {
        return vector{0, 0, 1};
    }
    static constexpr auto back() noexcept -> vector
        requires(Dims == 3)
    {
        return vector{0, 0, 1};
    }

    // NOTE: STATIC FUNCTIONS

    // :: Vector operation
    // Hadamard product
    static constexpr auto hadamard_product(const vector &lhs,
                                           const vector &rhs) -> vector {
        vector result{};
        auto range = std::views::zip(result, lhs, rhs);
        for (auto [res_element, l_element, r_element] : range) {
            res_element = l_element * r_element;
        }
        return result;
    }

    // Dot product
    static constexpr auto dot_product(const vector &lhs,
                                      const vector &rhs) -> compute_type {
        compute_type result{};
        auto range = std::views::zip(lhs, rhs);
        for (auto [l_element, r_element] : range) {
            result += static_cast<compute_type>(l_element * r_element);
        }
        return result;
    }

    // Min & Max
    static constexpr auto max(const vector &lhs,
                              const vector &rhs) -> vector {
        vector result;
        auto range = std::views::zip(result, lhs, rhs);
        for (auto [res_element, l_element, r_element] : range) {
            res_element = std::max(l_element, r_element);
        }
        return result;
    }
    static constexpr auto min(const vector &lhs,
                              const vector &rhs) -> vector {
        vector result;
        auto range = std::views::zip(result, lhs, rhs);
        for (auto [res_element, l_element, r_element] : range) {
            res_element = std::min(l_element, r_element);
        }
        return result;
    }

    // Lerp
    static constexpr auto lerp(const vector &from,
                               const vector &to,
                               const compute_type alpha) -> vector {
        vector result;
        const auto one_minus_alpha = static_cast<compute_type>(1.0) - alpha;
        auto range = std::views::zip(result, from, to);
        for (auto [res_element, from_element, to_element] : range) {
            res_element = static_cast<value_type>(one_minus_alpha * static_cast<compute_type>(from_element)
                                                  + alpha * static_cast<compute_type>(to_element));
        }
        return result;
    }

    // FIXME: error when use this in T = int (or other integer types)
    // Distance & Distance squared
    static constexpr auto distance_squared(const vector &lhs,
                                           const vector &rhs) -> compute_type {
        return (rhs - lhs).magnitude_squared();
    }
    // TODO: Make constexpr after C++26
    static inline auto distance(const vector &lhs,
                                const vector &rhs) -> compute_type {
        return (rhs - lhs).magnitude();
    }

    // Angle
    static constexpr auto angle(const vector &from,
                                const vector &to) -> compute_type {
        const compute_type divisor = from.magnitude() * to.magnitude();
        if (divisor == 0)
            return 0;

        const compute_type cos_v = dot_product(from, to) / divisor;
        if (cos_v <= 1) {
            return std::acos(cos_v);
        }

        return 0;
    }

    // Cross product
    // This only work on Dims == 3
    static constexpr auto cross_product(const vector &lhs, const vector &rhs) -> vector
        requires(Dims == 3)
    {
        return vector{lhs[1] * rhs[2] - lhs[2] * rhs[1],
                      lhs[2] * rhs[0] - lhs[0] * rhs[2],
                      lhs[0] * rhs[1] - lhs[1] * rhs[0]};
    }

    // NOTE: OPERATORS

    // :: Compare operators
    constexpr auto operator==(const vector &other) const -> bool {
        auto zip_range = std::views::zip(*this, other);
        for (auto [this_element, other_element] : zip_range) {
            if (this_element != other_element)
                return false;
        }
        return true;
    }
    constexpr auto operator!=(const vector &other) const -> bool {
        return !(operator==(other));
    }

    // :: Operators
    // +, - with vector
    template <typename U>
        requires std::is_convertible_v<U, value_type>
    constexpr auto operator+(const vector<U, Dims> &other) const -> vector {
        vector result;
        auto zip_range = std::views::zip(result, *this, other);
        for (auto [result_element, this_element, other_element] : zip_range) {
            result_element = this_element + static_cast<value_type>(other_element);
        }
        return result;
    }
    template <typename U>
        requires std::is_convertible_v<U, value_type>
    constexpr auto operator-(const vector<U, Dims> &other) const -> vector {
        vector result;
        auto zip_range = std::views::zip(result, *this, other);
        for (auto [result_element, this_element, other_element] : zip_range) {
            result_element = this_element - static_cast<value_type>(other_element);
        }
        return result;
    }

    template <typename U>
        requires std::is_convertible_v<U, value_type>
    constexpr auto operator+=(const vector<U, Dims> &other) -> vector {
        return *this = (*this + other);
    }
    template <typename U>
        requires std::is_convertible_v<U, value_type>
    constexpr auto operator-=(const vector<U, Dims> &other) -> vector {
        return *this = (*this - other);
    }

    // *, / with number
    constexpr auto operator*(const compute_type other) const -> vector {
        vector result;
        auto zip_range = std::views::zip(result, *this);
        for (auto [result_element, this_element] : zip_range) {
            result_element = static_cast<value_type>(static_cast<compute_type>(this_element) * other);
        }
        return result;
    }
    constexpr auto operator/(const compute_type other) const -> vector {
        vector result;
        auto zip_range = std::views::zip(result, *this);
        for (auto [result_element, this_element] : zip_range) {
            result_element = static_cast<value_type>(static_cast<compute_type>(this_element) / other);
        }
        return result;
    }

    template <typename U>
        requires std::is_convertible_v<U, value_type>
    constexpr auto operator*=(const compute_type other) -> vector {
        return *this = (*this * other);
    }
    template <typename U>
        requires std::is_convertible_v<U, value_type>
    constexpr auto operator/=(const compute_type other) -> vector {
        return *this = (*this / other);
    }
};

// Define the rest of operator
template <typename T, size_t Dims>
constexpr auto operator*(const typename vector<T, Dims>::compute_type num, const vector<T, Dims> &vec) -> vector<T, Dims> {
    return vec * num;
}

} // namespace mia
