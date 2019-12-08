project(TESTERS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# "test_my_impl" runs the tester on your own implementation
add_executable(test_my_impl catch.hpp tree_tests.cpp structs_tests.cpp)
target_link_libraries(test_my_impl PRIVATE ex3_lib tree_visualizer stdc++fs)

# "test_school_impl" runs my tester on the school's implementation. (A proper tester should never have errors here,
# and this is mostly for sanity-checking)
add_executable(test_school_impl catch.hpp tree_tests.cpp structs_tests.cpp)
set(SCHOOL_LIB_FILES
        "${CMAKE_SOURCE_DIR}/StructsSchool.a"
        "${CMAKE_SOURCE_DIR}/RBTreeSchool.a")
target_link_libraries(test_school_impl PRIVATE ${SCHOOL_LIB_FILES} tree_visualizer stdc++fs)

target_compile_definitions(test_school_impl PRIVATE USING_SCHOOL_SOLUTION)

