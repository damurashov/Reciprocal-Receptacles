cmake_minimum_required(VERSION 3.12)
project(Rr)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(options)

add_subdirectory(src)
add_subdirectory(examples)
