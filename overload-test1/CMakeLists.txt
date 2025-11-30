cmake_minimum_required(VERSION 3.20)

project(Overload-Test1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Overload-Test1
    overloadtest1.cpp
 "overload-test1.h")

if (MSVC)
    target_compile_options(Overload-Test1 PRIVATE /W4 /permissive-)
else()
    target_compile_options(Overload-Test1 PRIVATE -Wall -Wextra -Wpedantic)
endif()
