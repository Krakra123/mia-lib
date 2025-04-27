#pragma once

#include <cstdlib>
#include <type_traits>

#define STRING(x) #x

namespace mia {

// NOTE:
template <typename, typename = void>
constexpr bool is_type_complete_v = false;

template <typename T>
constexpr bool is_type_complete_v<T, std::void_t<decltype(sizeof(T))>> = true;

} // namespace mia
