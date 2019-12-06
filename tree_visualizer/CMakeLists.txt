project(tree_visualizer)

add_library(tree_visualizer ../RBTree.h graph_drawer.cpp graph_drawer.h util.hpp)
target_link_libraries(tree_visualizer PRIVATE)

# comment the following line if you don't want graph visualization
add_definitions(-DENABLE_TREE_VISUALIZATION)
