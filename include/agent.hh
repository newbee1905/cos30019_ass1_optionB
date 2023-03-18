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
private:
  kd::pair<int, int> m_pos;

public:
  Agent() {}
  ~Agent() {}

  void dfs(Grid &grid, std::vector<Action> &res);
  void bfs(Grid &grid, std::vector<Action> &res);

  Cell &pos() { return m_pos; }
  void set_pos(const Cell &pos) { m_pos = pos; }
  void set_pos(const Cell &&pos) { m_pos = std::move(pos); }

  void search(Methods method, Grid &grid, std::vector<Action> &res) {
    switch (method) {
    case Methods::DFS:
      return dfs(grid, res);
    case Methods::BFS:
      return bfs(grid, res);
    default:
      fmt::print(stderr, "This method is not implemented yet or not existed");
      return;
    }
  }
};

#endif // !AGENT_HH
