#include <stdio.h>

#include <ext/stdio_filebuf.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "agent.hh"
#include "e_methods.hh"
#include "errors.hh"
#include "fmt/core.h"
#include "grid.hh"
#include "utils.hh"

signed main(int argc, char **argv) {
  if (argc < 3) {
    fmt::print(stderr, "Not enough arguments. "
                       "Please use: ./search <FILEPATH> <METHOD>\n");
    return 1;
  }

  const char *inp_file_name = argv[1];
  const std::string_view method = argv[2];

  fmt::print("You select method {}\n", GetEnumMethods(method));
  auto inp_file = fopen(inp_file_name, "r");

  assert_line(inp_file, "Failed to open input file.");

  std::size_t n, m;
  fscanf(inp_file, "[%ld, %ld]\n", &n, &m);
  Grid grid(n, m);

  Agent a;
  fscanf(inp_file, "(%d,%d)\n", &a.pos.fst, &a.pos.sec);

  // use tmp to scanf after ')'
  // to force to stop at the line for getting
  // location of goals
  for (int x{}, y{}, tmp{};
       fscanf(inp_file, "(%d, %d)%c| ", &x, &y, (char *)&tmp);
       grid.insert_goal(x, y))
    ;

  for (int x{}, y{}, w{}, h{};
       fscanf(inp_file, "\n(%d, %d, %d, %d)", &y, &x, &w, &h) &&
       !feof(inp_file);
       grid.insert_block_area(x, y, w, h))
    ;

  fclose(inp_file);

  for (int i{}; i < n; ++i) {
    for (int j{}; j < m; ++j)
      fmt::print("{}, ", grid.at(i, j));
    fmt::println("");
  }
}
