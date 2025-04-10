#pragma once

#include <cmath>
#include <cstddef>
#include <utility>

#include "./utilities.hpp"

#define MIA_VECTOR_LOOP_OPERATION(Op) MIA_UNROLLED_LOOP(i, Ds, Op)

namespace mia {

template <typename T, size_t Ds>
class vector {
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;

public:
    constexpr vector() = default;
    explicit constexpr vector(T s) { MIA_VECTOR_LOOP_OPERATION(data[i] = s) }
    explicit constexpr vector(T* a) { MIA_VECTOR_LOOP_OPERATION(data[i] = a[i]) }

    constexpr vector(const vector& other) { MIA_VECTOR_LOOP_OPERATION(data[i] = other.data[i]) }
    template <typename U>
    explicit constexpr vector(const vector<U, Ds>& other) {
        MIA_VECTOR_LOOP_OPERATION(data[i] = static_cast<T>(other.data[i]))
    }

    constexpr vector(vector&& other) noexcept {
        MIA_VECTOR_LOOP_OPERATION(std::swap(data[i], other.data[i]))
    }

    constexpr auto operator=(T* a) -> vector& { return *this = vector(a); }

    constexpr auto operator=(const vector& other) -> vector& { return *this = vector(other); }

    constexpr auto operator=(vector&& other) noexcept -> vector& {
        MIA_VECTOR_LOOP_OPERATION(std::swap(data[i], other.data[i]))
        return *this;
    }

    inline auto operator()(const int i) -> T& { return data[i]; }
    constexpr auto operator()(const int i) const -> T& { return data[i]; }

    inline auto operator[](const int i) -> T& { return data[i]; }
    constexpr auto operator[](const int i) const -> T& { return data[i]; }

    constexpr auto magnitude() const -> T { return sqrt(magnitude_squared()); }

    constexpr auto magnitude_squared() const -> T { return dot_product(*this, *this); }

    constexpr auto normalized() const -> vector<T, Ds> {
        vector<T, Ds> res = *this;
        constexpr T length = length();
        MIA_VECTOR_LOOP_OPERATION(res[i] *= (T(1) / length));
        return res;
    }

    constexpr auto normalize() -> T {
        constexpr T length = length();
        MIA_VECTOR_LOOP_OPERATION(data[i] *= (T(1) / length));
        return length;
    }

    static constexpr auto dot_product(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> T {
        T result = 0;
        MIA_VECTOR_LOOP_OPERATION(result += lhs[i] * rhs[i]);
        return result;
    }

    // static constexpr vector<T, Ds> cross_product(const vector<T, Ds>& lhs, const vector<T, Ds>&
    // rhs) {}
    static constexpr auto hadamard_product(const vector<T, Ds>& lhs,
                                           const vector<T, Ds>& rhs) -> vector<T, Ds> {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] * rhs[i]);
        return res;
    }

    static constexpr auto lerp(const vector<T, Ds>& start, const vector<T, Ds>& end,
                               T k) -> vector<T, Ds> {
        vector<T, Ds> res;
        const T one_minus_k = static_cast<T>(1.0) - k;
        MIA_VECTOR_LOOP_OPERATION(res[i] = one_minus_k * start[i] + k * end[i]);
        return res;
    }

    static constexpr auto max(const vector<T, Ds>& lhs,
                              const vector<T, Ds>& rhs) -> const vector<T, Ds>& {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = std::max(lhs[i], rhs[i]));
        return res;
    }
    static constexpr auto min(const vector<T, Ds>& lhs,
                              const vector<T, Ds>& rhs) -> const vector<T, Ds>& {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = std::min(lhs[i], rhs[i]));
        return res;
    }

    static constexpr auto distance(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> T {
        return (rhs - lhs).length();
    }
    static constexpr auto distance_squared(const vector<T, Ds>& lhs,
                                           const vector<T, Ds>& rhs) -> T {
        return (rhs - lhs).length_squared();
    }

    static constexpr auto angle(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> T {
        const T divisor = lhs.Length() * rhs.Length();
        if (divisor == T(0)) {
            return T(0);
        }
        const T cos_v = vector<T, Ds>::dot_product(lhs, rhs) / divisor;
        if (cos_v <= T(1)) {
            return std::acos(cos_v);
        }
        return T(0);
    }

    std::array<T, Ds> data;
};

template <typename T, size_t Ds>
constexpr auto operator==(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> bool {
    MIA_VECTOR_LOOP_OPERATION(if (lhs[i] != rhs[i]) return false)
    return true;
}
template <typename T, size_t Ds>
constexpr auto operator!=(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> bool {
    return !(lhs == rhs);
}

template <typename T, size_t Ds>
constexpr auto operator-(const vector<T, Ds>& v) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = -v.data[i])
    return res;
}

template <typename T, size_t Ds>
constexpr auto operator*(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] * rhs[i])
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator*(const vector<T, Ds>& v, U s) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] * s);
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator*(U s, const vector<T, Ds>& v) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] * s);
    return res;
}

template <typename T, size_t Ds>
constexpr auto operator/(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] / rhs[i])
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator/(const vector<T, Ds>& v, U s) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] / s)
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator/(U s, const vector<T, Ds>& v) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = s / v[i])
    return res;
}

template <typename T, size_t Ds>
constexpr auto operator+(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] + rhs[i])
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator+(const vector<T, Ds>& v, U s) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] + s)
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator+(U s, const vector<T, Ds>& v) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] + s)
    return res;
}

template <typename T, size_t Ds>
constexpr auto operator-(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] - rhs[i])
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator-(const vector<T, Ds>& v, U s) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] - s)
    return res;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator-(U s, const vector<T, Ds>& v) -> vector<T, Ds> {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = s - v[i])
    return res;
}

template <typename T, size_t Ds>
constexpr auto operator*=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] *= rhs[i])
    return lhs;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator*=(vector<T, Ds>& v, U s) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(v[i] *= s);
    return v;
}

template <typename T, size_t Ds>
constexpr auto operator/=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] /= rhs[i])
    return lhs;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator/=(vector<T, Ds>& v, U s) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(v[i] /= s)
    return v;
}

template <typename T, size_t Ds>
constexpr auto operator+=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] += rhs[i])
    return lhs;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator+=(vector<T, Ds>& v, U s) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(v[i] += s)
    return v;
}

template <typename T, size_t Ds>
constexpr auto operator-=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] -= rhs[i])
    return lhs;
}
template <typename T, size_t Ds, typename U = T>
constexpr auto operator-=(vector<T, Ds>& v, U s) -> vector<T, Ds> {
    MIA_VECTOR_LOOP_OPERATION(v[i] -= s)
    return v;
}

}  // namespace mia
