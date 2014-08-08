cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

project (safe_printf)
set (CMAKE_CXX_FLAGS "-Wall -std=c++11")
add_executable (safe_printf main.cpp)
target_link_libraries (safe_printf)
