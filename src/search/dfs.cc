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

	parent[this->m_pos] = kd::pair<Cell, Action>{
			Cell{-1, -1},
      Action::NO_OP
  };
	s.push(this->m_pos);
	grid[this->m_pos] = BlockState::VISIT;

	// TODO: just use the first goal for now
	const auto goal = grid.m_goals[0];
	for (auto cur = s.top(); !s.empty(); cur = s.top()) {
		s.pop();

		if (cur == goal)
			break;

		for (const auto &c : CellAdjs) {
			const auto ncell = cur + c.fst;
			if (ncell.fst < 0 || ncell.sec < 0 || ncell.fst >= grid.height() || ncell.sec >= grid.width())
				continue;
			if (grid[ncell] >= BlockState::BLOCK)
				continue;
			grid[ncell] = BlockState::VISIT;
			s.push(ncell);
			parent[ncell] = kd::pair<Cell, Action>{cur, c.sec};
		}
	}

	auto cur = goal;
	// Trace back the route
	for (; cur != this->m_pos; res.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
}
