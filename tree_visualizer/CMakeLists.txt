project(tree_visualizer)

add_library(tree_visualizer graph_drawer.cpp graph_drawer.h util.hpp)
target_link_libraries(tree_visualizer PRIVATE ex3_lib)