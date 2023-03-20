#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "fmt/ostream.h"
#include "grid.hh"

#include <algorithm>
#include <cmath>
#include <map>
#include <queue>
#include <vector>

void Agent::astar(Grid &grid, std::vector<Action> &res) {
	auto cell_cmp = [&](const Cell &a, const Cell &b) { return grid[a] > grid[b]; };
	auto dist     = [&](const Cell &a, const Cell &b) {
    return std::abs(a.fst - b.fst) + std::abs(a.sec - b.sec);
	};

	std::priority_queue<Cell, std::vector<Cell>, decltype(cell_cmp)> q(cell_cmp);
	std::map<Cell, kd::pair<Cell, Action>> parent;

	parent[this->m_pos] = kd::pair<Cell, Action>{
			Cell{-1, -1},
      Action::NO_OP
  };
	q.push(this->m_pos);
	grid[this->m_pos] = BlockState::VISIT;

	// TODO: just use the first goal for DFS
	const auto goal = grid.m_goals[0];
	for (auto cur = q.top(); !q.empty(); cur = q.top()) {
		q.pop();

		if (cur == goal)
			break;

		for (const auto &c : CellAdjs) {
			const auto ncell = cur + c.fst;
			if (ncell.fst < 0 || ncell.sec < 0 || ncell.fst >= grid.height() || ncell.sec >= grid.width())
				continue;
			// BLOCK or VISITED
			if (grid[ncell] >= BlockState::BLOCK)
				continue;
			grid[ncell] = grid[cur] + 1 + dist(ncell, goal);
			q.push(ncell);
			parent[ncell] = kd::pair<Cell, Action>{cur, c.sec};
		}
	}

	auto cur = goal;
	// Trace back the route
	for (; cur != this->m_pos; res.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
}
