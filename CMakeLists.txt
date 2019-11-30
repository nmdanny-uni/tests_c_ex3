cmake_minimum_required(VERSION 3.1)
project(ex3_lib C)

set(CMAKE_C_STANDARD 99)

add_executable(ex3_tester unit_tests/main.c)


add_library(ex3_lib RBTree.h RBTree.c Structs.h Structs.c)
target_link_libraries(ex3_tester ex3_lib)

target_compile_options(ex3_tester PUBLIC -Wall -Wextra -Wvla -Werror)