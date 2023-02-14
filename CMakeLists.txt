cmake_minimum_required(VERSION 3.15)
project(top-trees)

set(CXX_STANDARD -c++20)

include_directories(include)

set(TEST_FILES ${TEST_FILES}
test/tree_test.cpp
test/test.cpp
)

set(TOP_TREE_FILES ${TOP_TREE_FILES}
src/tree/underlying_tree.cpp
)

#Main target
add_executable(main src/main.cpp ${TOP_TREE_FILES})

#Testing target 

add_subdirectory(src/lib/Catch2)
#Removes extra CTest targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#Add Ctest and Catch to CMake
include(CTest)
include(Catch)
enable_testing()

#Create tests target and add it to test list
add_executable(tests ${TEST_FILES} ${TOP_TREE_FILES})
target_link_libraries(tests PRIVATE Catch2 Catch2WithMain)
catch_discover_tests(tests)
