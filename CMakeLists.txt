cmake_minimum_required(VERSION 3.3)
project(ex3_lib C CXX)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

# this is your program(a library)
add_library(ex3_lib RBTree.h RBTree.c Structs.h Structs.c)

# compilation flags. you may remove 'Werror' if you don't want errors to be compilation errors
target_compile_options(ex3_lib PUBLIC -Wall -Wextra -Wvla -Werror -g)

# ensure headers at the root of this program are visible within all included files
include_directories(${CMAKE_SOURCE_DIR})

##### the following is setup for testers #####
# a C++ testing library
add_subdirectory(unit_tests/Catch2)

# "ex_tester" runs my tester on your own implementation
# add_executable(ex3_cpp_tester unit_tests/tree_tests.cpp)
#target_link_libraries(ex3_cpp_tester PRIVATE ex3_lib Catch2::Catch2)

# "ex3_tester_school" runs my tester on the school's implementation. (A proper tester should never have errors here,
# and this is mostly for sanity-checking)


add_executable(ex3_cpp_tester_school Structs.h RBTree.h unit_tests/tree_tests.cpp)
set(SCHOOL_LIB_FILES
        "${CMAKE_SOURCE_DIR}/StructsSchool.a"
        "${CMAKE_SOURCE_DIR}/RBTreeSchool.a")
target_link_libraries(ex3_cpp_tester_school PRIVATE ${SCHOOL_LIB_FILES} Catch2::Catch2)

