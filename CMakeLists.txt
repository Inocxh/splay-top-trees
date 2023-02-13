cmake_minimum_required(VERSION 3.15)
project(top-trees)

set(CXX_STANDARD -c++20)

include_directories(include)
#file(GLOB SOURCES "src/*.cpp")
add_executable(main src/main.cpp src/tree/underlying_tree.cpp include/underlying_tree.h)