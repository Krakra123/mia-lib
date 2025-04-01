#ifndef MIA_UTILITIES_HPP_
#define MIA_UTILITIES_HPP_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <type_traits>

template <typename T>
concept MIA_Arithmetic = std::is_arithmetic_v<T>;
template <typename T>
concept MIA_Integral = std::is_integral_v<T>;
template <typename T>
concept MIA_SignedIntegral = MIA_Integral<T> && std::is_signed<T>::value;
template <typename T>
concept MIA_UnsignedIntegral = MIA_Integral<T> && !MIA_SignedIntegral<T>;
template <typename T>
concept MIA_FloatingPoint = std::is_floating_point_v<T>;
template <typename T>
concept MIA_SignedFloatingPoint = MIA_FloatingPoint<T> && std::is_signed<T>::value;
template <typename T>
concept MIA_UnsignedFloatingPoint = MIA_FloatingPoint<T> && !MIA_SignedFloatingPoint<T>;

#define MIA_STRING_EXPAND(X) #X
#define MIA_STRING(X)        MIA_STRING_EXPAND(X)

#define MIA_UNROLLED_LOOP(iterator, number_of_iterations, operation)                            \
    {                                                                                           \
        _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wshadow\"")           \
            _Pragma("GCC diagnostic ignored \"-Wunused-variable\"") const int iterator = 0;     \
        { operation; }                                                                          \
        if ((number_of_iterations) > 1) {                                                       \
            const int iterator = 1;                                                             \
            { operation; }                                                                      \
            if ((number_of_iterations) > 2) {                                                   \
                const int iterator = 2;                                                         \
                { operation; }                                                                  \
                if ((number_of_iterations) > 3) {                                               \
                    const int iterator = 3;                                                     \
                    { operation; }                                                              \
                    if ((number_of_iterations) > 4) {                                           \
                        for (int iterator = 4; iterator < (number_of_iterations); ++iterator) { \
                            operation;                                                          \
                        }                                                                       \
                    }                                                                           \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        _Pragma("GCC diagnostic pop")                                                           \
    }

#define MIA_DEFAULT_ALIGNMENT 16

namespace mia {

template <typename T, uint8_t Alignment = MIA_DEFAULT_ALIGNMENT>
class simd_allocator : public std::allocator<T> {
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    template <typename _Tp1>
    struct rebind {
        typedef simd_allocator<_Tp1> other;
    };

    constexpr simd_allocator() noexcept : std::allocator<T>() {}
    constexpr simd_allocator(const simd_allocator& other) noexcept : std::allocator<T>(other) {}
    template <class U>
    constexpr simd_allocator(const simd_allocator<U>& other) noexcept : std::allocator<T>(other) {}

    virtual constexpr ~simd_allocator() = default;

    constexpr pointer allocate(size_type n) {
        return reinterpret_cast<pointer>(std::aligned_alloc(Alignment, n * sizeof(T)));
    }

    constexpr void deallocate(pointer p, size_type n) { std::free(p, n); }
};

template <typename T1, typename T2>
constexpr bool operator==([[maybe_unused]] const simd_allocator<T1> lhs,
                          [[maybe_unused]] const simd_allocator<T2> rhs) noexcept {
    return true;
}
template <typename T1, typename T2>
constexpr bool operator!=([[maybe_unused]] const simd_allocator<T1> lhs,
                          [[maybe_unused]] const simd_allocator<T2> rhs) noexcept {
    return false;
}

namespace math {

// Clamp
template <typename T>
constexpr T clamp(const T& x, const T& lower, const T& upper) {
    return std::max<T>(lower, std::min<T>(x, upper));
}

// Lerp
template <typename T, typename T2 = T>
constexpr T lerp(const T& range_start, const T& range_end, const T2& k) {
    const T2 one_minus_k = static_cast<T2>(1.0) - k;
    return range_start * one_minus_k + range_end * k;
}

// In Range
template <typename T, typename T2 = T>
constexpr bool in_range(const T& val, const T2& range_start, const T2& range_end) {
    return val >= range_start && val < range_end;
}

// Random
template <typename T>
inline T random() {
    return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}
template <>
inline float random<float>() {
    return static_cast<float>(rand() >> 8) / (static_cast<float>((RAND_MAX >> 8) + 1));
}
template <>
inline double random<double>() {
    return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX + 1LL));
}
inline double random() { return random<double>(); }

// Random Range
template <typename T>
constexpr T random_range(T range) {
    return (random<T>() * range * 2) - range;
}
template <typename T>
constexpr T random_in_range(T range_start, T range_end) {
    return lerp(range_start, range_end, random<T>());
}
template <>
constexpr int random_in_range<int>(int range_start, int range_end) {
    return static_cast<int>(
        random_in_range<float>(static_cast<float>(range_start), static_cast<float>(range_end)));
}

// Other
template <typename T>
constexpr T round_up_power_of_2(T x) {
    return static_cast<T>(pow(static_cast<T>(2), ceil(log(x) / log(static_cast<T>(2)))));
}
template <>
constexpr int32_t round_up_power_of_2<>(int32_t x) {
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
uint32_t round_up_type_bound(uint32_t v) {
    return (v + sizeof(T) - 1) & ~(sizeof(T) - 1);
}

}  // namespace math

}  // namespace mia

#endif  // !_MIA_UTILITIES_HPP
