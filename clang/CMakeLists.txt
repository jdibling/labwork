cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (hacks)

add_definitions (-std=c++11)
add_definitions (-Wall -Wextra -Werror)

include_directories (~/repo/phoenix/include)
include_directories (~/repo/phoenix/include/misapi)

set (SOURCES
  main.cpp
)

link_directories (~/repo/phoenix/lib/miapi)

add_executable (hacks ${SOURCES})

target_link_libraries (hacks
  MisApi)

