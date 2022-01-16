cmake_minimum_required(VERSION 3.10)
project(Rr)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(options)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)
