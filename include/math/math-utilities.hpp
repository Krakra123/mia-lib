#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>

#include "../utilities.hpp"

constexpr size_t MIA_DEFAULT_ALIGNMENT = 16;

namespace mia {

// SIMD allocation
template <typename T, uint8_t Alignment = MIA_DEFAULT_ALIGNMENT>
class simd_allocator : public std::allocator<T> {
  public:
    using size_type     = size_t;
    using pointer       = T *;
    using const_pointer = const T *;
    template <typename Tp1>
    struct rebind {
        using other = simd_allocator<Tp1>;
    };

    constexpr simd_allocator() noexcept
        : std::allocator<T>() {
    }
    constexpr simd_allocator(const simd_allocator &other) noexcept
        : std::allocator<T>(other) {
    }
    template <class U>
    constexpr simd_allocator(const simd_allocator<U> &other) noexcept
        : std::allocator<T>(other) {
    }

    virtual constexpr ~simd_allocator() = default;

    constexpr auto allocate(size_type n) -> pointer {
        return reinterpret_cast<pointer>(std::aligned_alloc(Alignment, n * sizeof(T)));
    }

    constexpr void deallocate(pointer p, size_type n) {
        std::free(p, n);
    }

    constexpr auto operator==([[maybe_unused]] const simd_allocator other) noexcept -> bool {
        return true;
    }
    constexpr auto operator!=([[maybe_unused]] const simd_allocator other) noexcept -> bool {
        return false;
    }
};

// Math
namespace math {

// Clamp
template <typename T>
constexpr auto clamp(const T &x, const T &lower, const T &upper) -> T {
    return std::max<T>(lower, std::min<T>(x, upper));
}

// Lerp
template <typename T, typename T2 = T>
constexpr auto lerp(const T &range_start, const T &range_end, const T2 &k) -> T {
    const T2 one_minus_k = static_cast<T2>(1.0) - k;
    return range_start * one_minus_k + range_end * k;
}

// In Range
template <typename T, typename T2 = T>
constexpr auto in_range(const T &val, const T2 &range_start, const T2 &range_end) -> bool {
    return val >= range_start && val < range_end;
}

// Other
template <typename T>
constexpr auto round_up_power_of_2(T x) -> T {
    return static_cast<T>(pow(static_cast<T>(2), ceil(log(x) / log(static_cast<T>(2)))));
}
template <>
constexpr auto round_up_power_of_2<>(int32_t x) -> int32_t {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

template <typename T>
auto round_up_type_bound(uint32_t v) -> uint32_t {
    return (v + sizeof(T) - 1) & ~(sizeof(T) - 1);
}

} // namespace math

} // namespace mia
