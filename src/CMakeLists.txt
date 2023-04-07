get_filename_component(APP search NAME)
add_library(kd errors.cc grid.cc agent.cc search.cc search/dfs.cc search/bfs.cc search/gbfs.cc search/astar.cc search/dijkstra.cc search/idastar.cc)
add_executable(${APP} main.cc)
target_link_libraries(${APP} kd)
