#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "agent.hh"
#include "e_action.hh"
#include "e_methods.hh"
#include "errors.hh"
#include "fmt/core.h"
#include "grid.hh"
#include "utils.hh"

#ifdef __linux__
#define FSCANF fscanf
#define FOPEN fopen
#elif _WIN32
#define FSCANF fscanf_s
#define FOPEN fopen_s
#endif

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
#ifdef __linux__
  assert_line(FSCANF(inp_file, "[%ld, %ld]\n", &n, &m),
              "Failed getting maze's size.");
#elif _WIN32
  assert_line(FSCANF(inp_file, "[%lld, %lld]\n", &n, &m),
              "Failed getting maze's size.");
#endif
  Grid grid(n, m);

  Agent a;
  assert_line(FSCANF(inp_file, "(%d,%d)\n", &a.pos.sec, &a.pos.fst),
              "Failed getting starting point.");

  // use tmp to scanf after ')'
  // to force to stop at the line for getting
  // location of goals
  for (int x{}, y{}, tmp{};
       assert_line(FSCANF(inp_file, "(%d, %d)%c| ", &y, &x, (char *)&tmp),
                   "Failed getting goals.");
       grid.insert_goal(x, y))
    ;

  for (int x{}, y{}, w{}, h{};
       FSCANF(inp_file, "\n(%d, %d, %d, %d)", &y, &x, &w, &h) &&
       !feof(inp_file);
       grid.insert_block_area(x, y, w, h))
    ;

  fclose(inp_file);

  for (int i{}; i < n; ++i) {
    for (int j{}; j < m; ++j)
      fmt::print("{}, ", grid.at(i, j));
    fmt::println("");
  }
  fmt::println("");

  std::vector<Action> res;

  a.search(GetEnumMethods(method), grid, res);

  if (res.empty()) {
    return 1;
  }

  for (int i{}; i < n; ++i) {
    for (int j{}; j < m; ++j)
      fmt::print("{}, ", grid.at(i, j));
    fmt::println("");
  }
  fmt::println("");

  // Print the route
  for (std::size_t i = res.size(); i-- > 0; fmt::print("{}; ", res[i]))
    ;
}
