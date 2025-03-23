#include "utilities.hpp"
#include <cstdio>
#include <iostream>

int main() {
    std::cout << typeid(mia::simd_allocator<float>::value_type).name() << "\n";
}
