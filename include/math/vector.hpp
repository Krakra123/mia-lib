#ifndef MIA_VECTOR_GENERAL_HPP_
#define MIA_VECTOR_GENERAL_HPP_

#include <cmath>
#include <utility>

#include "utilities.hpp"

#define MIA_VECTOR_LOOP_OPERATION(Op) MIA_UNROLLED_LOOP(i, Ds, Op)

namespace mia {

template <MIA_Arithmetic T, size_t Ds>
class vector {
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

    constexpr vector& operator=(T* a) { return *this = vector(a); }

    constexpr vector& operator=(const vector& other) { return *this = vector(other); }

    constexpr vector& operator=(vector&& other) noexcept {
        MIA_VECTOR_LOOP_OPERATION(std::swap(data[i], other.data[i]))
        return *this;
    }

    inline T& operator()(const int i) { return data[i]; }
    constexpr T& operator()(const int i) const { return data[i]; }

    inline T& operator[](const int i) { return data[i]; }
    constexpr T& operator[](const int i) const { return data[i]; }

    constexpr T magnitude() const { return sqrt(magnitude_squared()); }

    constexpr T magnitude_squared() const { return dot_product(*this, *this); }

    constexpr vector<T, Ds> normalized() const {
        vector<T, Ds> res = *this;
        constexpr T length = length();
        MIA_VECTOR_LOOP_OPERATION(res[i] *= (T(1) / length));
        return res;
    }

    constexpr T normalize() {
        constexpr T length = length();
        MIA_VECTOR_LOOP_OPERATION(data[i] *= (T(1) / length));
        return length;
    }

    static constexpr T dot_product(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
        T result = 0;
        MIA_VECTOR_LOOP_OPERATION(result += lhs[i] * rhs[i]);
        return result;
    }

    // static constexpr vector<T, Ds> cross_product(const vector<T, Ds>& lhs, const vector<T, Ds>&
    // rhs) {}
    static constexpr vector<T, Ds> hadamard_product(const vector<T, Ds>& lhs,
                                                    const vector<T, Ds>& rhs) {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] * rhs[i]);
        return res;
    }

    static constexpr vector<T, Ds> lerp(const vector<T, Ds>& start, const vector<T, Ds>& end, T k) {
        vector<T, Ds> res;
        const T one_minus_k = static_cast<T>(1.0) - k;
        MIA_VECTOR_LOOP_OPERATION(res[i] = one_minus_k * start[i] + k * end[i]);
        return res;
    }

    static constexpr const vector<T, Ds>& max(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = std::max(lhs[i], rhs[i]));
        return res;
    }
    static constexpr const vector<T, Ds>& min(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = std::min(lhs[i], rhs[i]));
        return res;
    }

    static constexpr T distance(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
        return (rhs - lhs).length();
    }
    static constexpr T distance_squared(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
        return (rhs - lhs).length_squared();
    }

    static constexpr T angle(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
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

    static constexpr vector<T, Ds> random_range(T range) {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = math::random_range(range))
        return res;
    }

    static constexpr vector<T, Ds> random_unit_vector() {
        vector<T, Ds> res;
        MIA_VECTOR_LOOP_OPERATION(res[i] = math::random_range(1))
        constexpr T mag = res.magnitude();
        return res / mag;
    }

    T data[Ds];
};

template <MIA_Arithmetic T, size_t Ds>
constexpr bool operator==(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATION(if (lhs != rhs) return false)
    return true;
}
template <MIA_Arithmetic T, size_t Ds>
constexpr bool operator!=(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    return !(lhs == rhs);
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator-(const vector<T, Ds>& v) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = -v.data[i])
    return res;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator*(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] * rhs[i])
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator*(const vector<T, Ds>& v, U s) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] * s);
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator*(U s, const vector<T, Ds>& v) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] * s);
    return res;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator/(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] / rhs[i])
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator/(const vector<T, Ds>& v, U s) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] / s)
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator/(U s, const vector<T, Ds>& v) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = s / v[i])
    return res;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator+(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] + rhs[i])
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator+(const vector<T, Ds>& v, U s) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] + s)
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator+(U s, const vector<T, Ds>& v) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] + s)
    return res;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator-(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = lhs[i] - rhs[i])
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator-(const vector<T, Ds>& v, U s) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = v[i] - s)
    return res;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator-(U s, const vector<T, Ds>& v) {
    vector<T, Ds> res;
    MIA_VECTOR_LOOP_OPERATION(res[i] = s - v[i])
    return res;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator*=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] *= rhs[i])
    return lhs;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator*=(vector<T, Ds>& v, U s) {
    MIA_VECTOR_LOOP_OPERATION(v[i] *= s);
    return v;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator/=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] /= rhs[i])
    return lhs;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator/=(vector<T, Ds>& v, U s) {
    MIA_VECTOR_LOOP_OPERATION(v[i] /= s)
    return v;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator+=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] += rhs[i])
    return lhs;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator+=(vector<T, Ds>& v, U s) {
    MIA_VECTOR_LOOP_OPERATION(v[i] += s)
    return v;
}

template <MIA_Arithmetic T, size_t Ds>
constexpr vector<T, Ds> operator-=(vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATION(lhs[i] -= rhs[i])
    return lhs;
}
template <MIA_Arithmetic T, size_t Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator-=(vector<T, Ds>& v, U s) {
    MIA_VECTOR_LOOP_OPERATION(v[i] -= s)
    return v;
}

}  // namespace mia

#define _MIA_VECTOR_INTERNAL
#include "internal/vector2.hpp"
#include "internal/vector3.hpp"
#include "internal/vector4.hpp"

#endif  // !MIA_VECTOR_GENERAL_HPP_
