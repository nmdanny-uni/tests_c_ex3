project(TESTERS)

# "ex_tester" runs the tester on your own implementation
add_executable(ex3_cpp_tester tree_tests.cpp)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message("Boost isn't installed")
endif()
target_link_libraries(ex3_cpp_tester PRIVATE ex3_lib Catch2::Catch2 ${Boost_LIBRARIES} tree_visualizer)

# "ex3_tester_school" runs my tester on the school's implementation. (A proper tester should never have errors here,
# and this is mostly for sanity-checking)
add_executable(ex3_cpp_tester_school tree_tests.cpp)
set(SCHOOL_LIB_FILES
        "${CMAKE_SOURCE_DIR}/StructsSchool.a"
        "${CMAKE_SOURCE_DIR}/RBTreeSchool.a")
target_link_libraries(ex3_cpp_tester_school PRIVATE ${SCHOOL_LIB_FILES} Catch2::Catch2 ${Boost_LIBRARIES} tree_visualizer)
