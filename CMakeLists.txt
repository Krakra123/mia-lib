cmake_minimum_required(VERSION 3.10)
project(mia_util 
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
add_subdirectory(googletest)

add_subdirectory(test)
