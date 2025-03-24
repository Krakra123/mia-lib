#include <cstdio>
#include <iostream>
#include <type_traits>

#include "math/vector.hpp"

int main() {
    std::cout << "bool: " << std::alignment_of_v<bool> << "\n"; 
    std::cout << "char: " << std::alignment_of_v<char> << "\n"; 
    std::cout << "int: " << std::alignment_of_v<int> << "\n"; 
    std::cout << "long: " << std::alignment_of_v<long> << "\n"; 
    std::cout << "long long: " << std::alignment_of_v<long long> << "\n"; 
    std::cout << "float: " << std::alignment_of_v<float> << "\n"; 
    std::cout << "double: " << std::alignment_of_v<double> << "\n"; 
}
