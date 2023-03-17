#ifndef AGENT_HH
#define AGENT_HH

#include "e_action.hh"
#include "e_methods.hh"
#include "fmt/ostream.h"
#include "grid.hh"
#include "utils.hh"
#include <array>
#include <vector>

class Agent {
public:
  kd::pair<int, int> pos;
  Agent() {}
  ~Agent() {}

  void dfs(Grid &grid, std::vector<Action> &res);

  void search(Methods method, Grid &grid, std::vector<Action> &res) {
    switch (method) {
    case Methods::DFS:
      return dfs(grid, res);
    default:
      fmt::print(stderr, "This method is not implemented yet or not existed");
      return;
    }
  }
};

#endif // !AGENT_HH
