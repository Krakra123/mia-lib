#pragma once

#include <cstddef>

#include "./utilities.hpp"
#include "./vector.hpp"

#define MIA_MATRIX_COL_LOOP_OPERATION(Op) MIA_UNROLLED_LOOP(i, Cols, Op)
#define MIA_MATRIX_ROW_LOOP_OPERATION(Op) MIA_UNROLLED_LOOP(i, Rows, Op)
#define MIA_MATRIX_LOOP_OPERATION(Op)     MIA_UNROLLED_LOOP(i, Rows, MIA_UNROLLED_LOOP(j, Cols, Op))

namespace mia {

template <typename T, size_t Rows, size_t Cols>
class matrix {
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;

public:
    constexpr matrix() = default;
    explicit constexpr matrix(T s) { MIA_MATRIX_LOOP_OPERATION(data[i][j] = s) }
    explicit constexpr matrix(T* a) { MIA_MATRIX_ROW_LOOP_OPERATION(data[i] = &a[i * Rows]) }

    constexpr matrix(const matrix& other) { MIA_MATRIX_ROW_LOOP_OPERATION(data[i] = other.data[i]) }
    template <typename U>
    explicit constexpr matrix(const matrix<U, Cols, Rows>& other) {
        MIA_MATRIX_ROW_LOOP_OPERATION(data[i] = other.data[i])
    }

    constexpr matrix(matrix&& other) noexcept {
        MIA_MATRIX_ROW_LOOP_OPERATION(std::swap(data[i], other.data[i]))
    }

    constexpr auto operator=(T* a) -> matrix& { return *this = matrix(a); }

    constexpr auto operator=(const matrix& other) -> matrix& { return *this = matrix(other); }

    constexpr auto operator=(matrix&& other) noexcept -> matrix& {
        MIA_MATRIX_ROW_LOOP_OPERATION(std::swap(data[i], other.data[i])) return *this;
    }

    constexpr auto operator()(const int row, const int col) const -> const T& {
        return data[col][row];
    }
    constexpr auto operator()(const int row, const int col) -> T& { return data[col][row]; }

    constexpr auto operator()(const int i) const -> const T& { return operator[](i); }

    constexpr auto operator()(const int i) -> T& { return operator[](i); }

    constexpr auto operator[](const int i) const -> const T& {
        return reinterpret_cast<T*>(data)[i];
    }
    constexpr auto operator[](const int i) -> T& { return reinterpret_cast<T*>(data)[i]; }

    constexpr auto get_row(const int row) const -> const vector<T, Cols>& { return data[row]; }
    constexpr auto get_row(const int row) -> vector<T, Cols>& { return data[row]; }

    constexpr auto transpose() const -> matrix<T, Cols, Rows> {
        matrix<T, Cols, Rows> res;
        MIA_MATRIX_LOOP_OPERATION(res[j][i] = (*this)[i][j])
        return res;
    }

    static constexpr auto outer_product(const vector<T, Cols>& lhs, const vector<T, Cols>& rhs) {
        matrix res;
        MIA_MATRIX_LOOP_OPERATION(res[i][j] = lhs[i] * rhs[j])
        return res;
    }
    static constexpr auto hadamard_product(const matrix& lhs, const matrix& rhs) {
        matrix res;
        MIA_MATRIX_LOOP_OPERATION(res[i][j] = lhs[i][j] * rhs[i][j])
        return res;
    }

    static constexpr auto identity() -> matrix {}

    std::array<vector<T, Cols>, Rows> data;
};

template <typename T, size_t Rows, size_t Cols>
constexpr auto operator==(const matrix<T, Rows, Cols>& lhs,
                          const matrix<T, Rows, Cols>& rhs) -> bool {
    MIA_MATRIX_LOOP_OPERATION(if (lhs[i][j] != rhs[i][j]) return false)
    return true;
}
template <typename T, size_t Rows, size_t Cols>
constexpr auto operator!=(const matrix<T, Rows, Cols>& lhs,
                          const matrix<T, Rows, Cols>& rhs) -> bool {
    return !(lhs == rhs);
}

template <typename T, size_t Rows, size_t Cols>
constexpr auto operator-(const matrix<T, Rows, Cols>& v) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i] = -v.data[i])
    return res;
}

template <typename T, size_t Size1, size_t Size2, size_t Size3>
constexpr auto operator*(const matrix<T, Size1, Size2>& lhs,
                         const matrix<T, Size2, Size3>& rhs) -> matrix<T, Size1, Size3> {
    matrix<T, Size1, Size3> res;
    MIA_UNROLLED_LOOP(i, Size1, MIA_UNROLLED_LOOP(j, Size3, MIA_UNROLLED_LOOP(k, Size2, {
                                                      res[i][j] += lhs[i][k] + rhs[k][j];
                                                  })))
    return res;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator*(const matrix<T, Rows, Cols>& v, U s) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i] = v[i] * s);
    return res;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator*(U s, const matrix<T, Rows, Cols>& v) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i] = v[i] * s);
    return res;
}

template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator/(const matrix<T, Rows, Cols>& v, U s) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i] = v[i] / s)
    return res;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator/(U s, const matrix<T, Rows, Cols>& v) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i] = s / v[i])
    return res;
}

template <typename T, size_t Rows, size_t Cols>
constexpr auto operator+(const matrix<T, Rows, Cols>& lhs,
                         const matrix<T, Rows, Cols>& rhs) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i][j] = lhs[i][j] + rhs[i][j])
    return res;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator+(const matrix<T, Rows, Cols>& v, U s) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i][j] = v[i][j] + s)
    return res;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator+(U s, const matrix<T, Rows, Cols>& v) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i][j] = v[i][j] + s)
    return res;
}

template <typename T, size_t Rows, size_t Cols>
constexpr auto operator-(const matrix<T, Rows, Cols>& lhs,
                         const matrix<T, Rows, Cols>& rhs) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i][j] = lhs[i][j] - rhs[i][j])
    return res;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator-(const matrix<T, Rows, Cols>& v, U s) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i][j] = v[i][j] - s)
    return res;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator-(U s, const matrix<T, Rows, Cols>& v) -> matrix<T, Rows, Cols> {
    matrix<T, Rows, Cols> res;
    MIA_MATRIX_LOOP_OPERATION(res[i][j] = s - v[i][j])
    return res;
}

template <typename T, size_t Rows, size_t Cols>
constexpr auto operator+=(matrix<T, Rows, Cols>& lhs,
                          const matrix<T, Rows, Cols>& rhs) -> matrix<T, Rows, Cols> {
    MIA_MATRIX_LOOP_OPERATION(lhs[i][j] += rhs[i][j])
    return lhs;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator+=(matrix<T, Rows, Cols>& v, U s) -> matrix<T, Rows, Cols> {
    MIA_MATRIX_LOOP_OPERATION(v[i][j] += s)
    return v;
}

template <typename T, size_t Rows, size_t Cols>
constexpr auto operator-=(matrix<T, Rows, Cols>& lhs,
                          const matrix<T, Rows, Cols>& rhs) -> matrix<T, Rows, Cols> {
    MIA_MATRIX_LOOP_OPERATION(lhs[i][j] -= rhs[i][j])
    return lhs;
}
template <typename T, size_t Rows, size_t Cols, typename U = T>
constexpr auto operator-=(matrix<T, Rows, Cols>& v, U s) -> matrix<T, Rows, Cols> {
    MIA_MATRIX_LOOP_OPERATION(v[i][j] -= s)
    return v;
}

}  // namespace mia
