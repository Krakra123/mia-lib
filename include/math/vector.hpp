#ifndef MIA_VECTOR_HPP_
#define MIA_VECTOR_HPP_

#include <utility>

#include "utilities.hpp"

#define MIA_VECTOR_LOOP_OPERATION(Op) MIA_UNROLLED_LOOP(i, Ds, Op)
#define MIA_VECTOR_LOOP_OPERATOR(Op)   \
    {                                  \
        mia::vector<T, Ds> res;        \
        MIA_VECTOR_LOOP_OPERATION(Op); \
        return res;                    \
    }

namespace mia {

template <MIA_Arithmetic T, int Ds>
class vector {
public:
    constexpr vector() = default;
    explicit constexpr vector(T s) {
        MIA_VECTOR_LOOP_OPERATION(data[i] = s)
    }
    explicit constexpr vector(T* a) {
        MIA_VECTOR_LOOP_OPERATION(data[i] = a[i])
    }

    constexpr vector(const vector& other) {
        MIA_VECTOR_LOOP_OPERATION(data[i] = other.data[i])
    }
    template <typename U>
    explicit constexpr vector(const vector<U, Ds>& other) {
        MIA_VECTOR_LOOP_OPERATION(data[i] = static_cast<T>(other.data[i]))
    }

    constexpr vector(vector&& other) noexcept {
        MIA_VECTOR_LOOP_OPERATION(std::swap(data[i], other.data[i]))
    }

    constexpr vector& operator=(T* a) {
        return *this = vector(a);
    }

    constexpr vector& operator=(const vector& other) {
        return *this = vector(other);
    }

    constexpr vector& operator=(vector&& other) noexcept {
        MIA_VECTOR_LOOP_OPERATION(std::swap(data[i], other.data[i]))
        return *this;
    }

    inline T& operator()(const int i) {
        return data[i];
    }
    constexpr T& operator()(const int i) const {
        return data[i];
    }

    inline T& operator[](const int i) {
        return data[i];
    }
    constexpr T& operator[](const int i) const {
        return data[i];
    }

    constexpr T length() const {}

    constexpr T length_squared() const {}

    constexpr vector<T, Ds> normalized() const {}

    constexpr T normalize() {}

    static constexpr T dot_product(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}

    static constexpr vector<T, Ds> cross_product(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}

    static constexpr vector<T, Ds> hadamard_product(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}

    static constexpr vector<T, Ds> lerp(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs, T k) {}

    static constexpr const vector<T, Ds>& max(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}
    static constexpr const vector<T, Ds>& min(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}

    static constexpr T distance(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}
    static constexpr T distance_squared(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}

    static constexpr T angle(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {}

    T data[Ds];
};

template <MIA_Arithmetic T, int Ds>
constexpr bool operator==(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATION(if (lhs != rhs) return false)
    return true;
}
template <MIA_Arithmetic T, int Ds>
constexpr bool operator!=(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    return !(lhs == rhs);
}
template <MIA_Arithmetic T, int Ds>
constexpr vector<T, Ds> operator-(const vector<T, Ds>& v) {
    MIA_VECTOR_LOOP_OPERATOR(-v.data[i])
}

template <MIA_Arithmetic T, int Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator*(const vector<T, Ds>& v, U s) {}
template <MIA_Arithmetic T, int Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator*(U s, const vector<T, Ds>& v) {
    return v * s;
}

template <MIA_Arithmetic T, int Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator/(const vector<T, Ds>& v, U s) {}
template <MIA_Arithmetic T, int Ds, MIA_Arithmetic U = T>
constexpr vector<T, Ds> operator/(U s, const vector<T, Ds>& v) {}

template <MIA_Arithmetic T, int Ds>
constexpr vector<T, Ds> operator+(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATOR(lhs.data[i] + rhs.data[i])
}
template <MIA_Arithmetic T, int Ds>
constexpr vector<T, Ds> operator-(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATOR(lhs.data[i] + rhs.data[i])
}

template <MIA_Arithmetic T, int Ds>
constexpr vector<T, Ds> operator*(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATOR(lhs.data[i] + rhs.data[i])
}
template <MIA_Arithmetic T, int Ds>
constexpr vector<T, Ds> operator/(const vector<T, Ds>& lhs, const vector<T, Ds>& rhs) {
    MIA_VECTOR_LOOP_OPERATOR(lhs.data[i] + rhs.data[i])
}

} // namespace mia

#endif // !MIA_VECTOR_HPP_
