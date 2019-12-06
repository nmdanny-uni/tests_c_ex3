cmake_minimum_required(VERSION 3.3)
project(ex3_lib C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

# this is your program(a library)
add_library(ex3_lib RBTree.h RBTree.c Structs.h Structs.c)

# compilation flags. you may remove 'Werror' if you don't want warnings to be compilation errors
target_compile_options(ex3_lib PUBLIC -Wall -Wextra -Wvla -g)

# ensure headers at the root of this program are visible within all included files
include_directories(${CMAKE_SOURCE_DIR})

# utility library for drawing RB trees
add_subdirectory(tree_visualizer)

# your library will be linked with math library(if you need it) and my tree visualizer
target_link_libraries(ex3_lib tree_visualizer m)

# unit tests that can be run via CLion
add_subdirectory(unit_tests)

###### These are the built in presubmission tests, these can also double as a 'main' for exploration and whatnot  #####

# running 'ProductExample' (from presubmit test) on your own implementation
add_executable(product_example_mine ProductExample.c)
target_link_libraries(product_example_mine ex3_lib tree_visualizer)

# running 'ProductExample' (from presubmit test) on school implementation
# you can use this for reference
add_executable(product_example_school ProductExample.c)
set(SCHOOL_LIB_FILES
        "${CMAKE_SOURCE_DIR}/StructsSchool.a"
        "${CMAKE_SOURCE_DIR}/RBTreeSchool.a")
target_link_libraries(product_example_school ${SCHOOL_LIB_FILES} tree_visualizer)
