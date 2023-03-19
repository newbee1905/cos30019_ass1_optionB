#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "fmt/ostream.h"
#include "grid.hh"

#include <algorithm>
#include <map>
#include <queue>
#include <vector>

void Agent::bfs(Grid &grid, std::vector<Action> &res) {
	std::queue<Cell> q;
	std::map<Cell, kd::pair<Cell, Action>> parent;

	parent[this->m_pos] = kd::pair<Cell, Action>{
			Cell{-1, -1},
      Action::NO_OP
  };
	q.push(this->m_pos);
	grid[this->m_pos] = BlockState::VISIT;

	// TODO: just use the first goal for DFS
	const auto goal = grid.m_goals[0];
	for (auto cur = q.front(); !q.empty(); cur = q.front()) {
		q.pop();

		if (cur == goal)
			break;

		for (const auto &c : CellAdjs) {
			const auto ncell = cur + c.fst;
			if (ncell.fst < 0 || ncell.sec < 0 || ncell.fst >= grid.height() || ncell.sec >= grid.width())
				continue;
			if (grid[ncell] >= BlockState::BLOCK)
				continue;
			grid[ncell] = BlockState::VISIT;
			q.push(ncell);
			parent[ncell] = kd::pair<Cell, Action>{cur, c.sec};
		}
	}

	auto cur = goal;
	// Trace back the route
	for (; cur != this->m_pos; res.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
}
