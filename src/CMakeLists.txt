get_filename_component(APP ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(${APP} main.cc)
add_library(_lib errors.cc grid.cc agent.cc search/dfs.cc search/bfs.cc)
target_link_libraries(${APP} _lib)
