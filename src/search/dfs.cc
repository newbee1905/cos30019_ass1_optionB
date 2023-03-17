#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "fmt/ostream.h"
#include "grid.hh"

#include <algorithm>
#include <map>
#include <stack>
#include <vector>

void Agent::dfs(Grid &grid, std::vector<Action> &res) {
  std::stack<Cell> s;
  std::map<Cell, kd::pair<Cell, Action>> parent;

  parent[this->pos] = kd::pair<Cell, Action>{Cell{-1, -1}, Action::NO_OP};
  s.push(this->pos);
  grid[this->pos] = BlockState::VISIT;

  // TODO: just use the first goal for DFS
  const auto goal = grid.goals[0];
  for (auto cur = s.top(); !s.empty(); cur = s.top()) {
    s.pop();
    grid[cur] = BlockState::VISIT;

    if (cur == goal)
      break;

    // TODO: may consider generate a vector of
    // possible adjacent cell
    if (cur.sec > 0) { // LEFT
      const auto lcell = Cell{cur.fst, cur.sec - 1};
      if (grid[lcell] != BlockState::VISIT &&
          grid[lcell] != BlockState::BLOCK) {
        s.push(lcell);
        parent[lcell] = kd::pair<Cell, Action>{cur, Action::LEFT};
      }
    }
    if (cur.fst > 0) { // UP
      const auto ucell = Cell{cur.fst - 1, cur.sec};
      if (grid[ucell] != BlockState::VISIT &&
          grid[ucell] != BlockState::BLOCK) {
        s.push(ucell);
        parent[ucell] = kd::pair<Cell, Action>{cur, Action::UP};
      }
    }
    if (cur.fst < grid.n - 1) { // DOWN
      const auto dcell = Cell{cur.fst + 1, cur.sec};
      if (grid[dcell] != BlockState::VISIT &&
          grid[dcell] != BlockState::BLOCK) {
        s.push(dcell);
        parent[dcell] = kd::pair<Cell, Action>{cur, Action::DOWN};
      }
    }
    if (cur.sec < grid.m - 1) { // RIGHT
      const auto rcell = Cell{cur.fst, cur.sec + 1};
      if (grid[rcell] != BlockState::VISIT &&
          grid[rcell] != BlockState::BLOCK) {
        s.push(rcell);
        parent[rcell] = kd::pair<Cell, Action>{cur, Action::RIGHT};
      }
    }
  }

  auto cur = goal;
  // Trace back the route
  for (; cur != this->pos;
       res.push_back(parent[cur].sec), cur = parent[cur].fst)
    ;
}
