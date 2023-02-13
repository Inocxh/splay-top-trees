cmake_minimum_required(VERSION 3.15)
project(top-trees)

set(CXX_STANDARD -c++20)

include_directories(include)

set(TEST_FILES ${TEST_FILES}
    test/tree_test.cpp
    test/test.cpp
    src/tree/underlying_tree.cpp
)

add_subdirectory(src/lib/Catch2)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests PRIVATE Catch2 Catch2WithMain)

#include(CTest)
include(Catch)
catch_discover_tests(tests)

#file(GLOB SOURCES "src/*.cpp")
add_executable(main src/main.cpp src/tree/underlying_tree.cpp)