cmake_minimum_required(VERSION 3.10)
project(mia_util 
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mia_util main.cpp)

target_include_directories(mia_util PRIVATE include)

include(CTest)
add_subdirectory(dependencies/googletest)

add_subdirectory(test)
