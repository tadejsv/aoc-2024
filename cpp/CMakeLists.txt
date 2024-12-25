cmake_minimum_required(VERSION 3.31)

project(AOC2024
    VERSION 1.0
    LANGUAGES CXX
)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_23)

# 3rd party dependencies
find_package(Eigen3 3.4.90 REQUIRED NO_MODULE)
find_package(Catch2 3.7.1 REQUIRED NO_MODULE)

include(CTest)
include(Catch)

add_subdirectory(utils)

add_subdirectory(day01)
add_subdirectory(day02)
add_subdirectory(day03)
add_subdirectory(day04)
add_subdirectory(day05)
