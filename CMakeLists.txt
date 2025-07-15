cmake_minimum_required(VERSION 3.18)
project(pixel_vectors LANGUAGES CXX)

enable_testing()

find_package(cxxopts REQUIRED)
find_package(opencv REQUIRED)

add_subdirectory(modules)
add_subdirectory(src)

