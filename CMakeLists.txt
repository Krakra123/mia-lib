cmake_minimum_required(VERSION 3.10)
project(mia-lib 
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -Wall        # Enable most warnings
        -Wextra      # Enable extra warnings
        -Wpedantic   # Strict ISO compliance
        -Wconversion # Warn about implicit type conversions
        -Wsign-conversion  # Warn on implicit sign conversions
        -Wshadow     # Warn when a variable hides another
        -Wdouble-promotion # Warn about float to double promotions
        -Wformat=2   # Stronger format string warnings
        -Wundef      # Warn about undefined macros
        -Werror      # Treat warnings as errors
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
        /W4  # High warning level
        /permissive- # Strict standard conformance
        /sdl  # Enable additional security checks
        /WX   # Treat warnings as errors
    )
endif()

if(SANITIZER STREQUAL "address")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
elseif(SANITIZER STREQUAL "thread")
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
elseif(SANITIZER STREQUAL "memory")
    add_compile_options(-fsanitize=memory)
    add_link_options(-fsanitize=memory)
elseif(SANITIZER STREQUAL "undefined")
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()
if(NOT DEFINED SANITIZER)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

# set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,bugprone-*,cert-*,modernize-*,performance-*,readability-*,zircon-*,-system-headers)

add_compile_options(-fdiagnostics-color=always)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mia-lib main.cpp)

target_include_directories(mia-lib PRIVATE include)

include(CTest)
add_subdirectory(dependencies/googletest)

add_subdirectory(test)
