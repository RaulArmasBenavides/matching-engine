cmake_minimum_required(VERSION 3.20)

project(RaiiTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(raii_test
    raii-test.cpp
)

if (MSVC)
    target_compile_options(raii_test PRIVATE /W4 /permissive-)
else()
    target_compile_options(raii_test PRIVATE -Wall -Wextra -Wpedantic)
endif()
