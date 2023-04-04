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

int kd::Agent::astar(kd::Grid &grid) {
	std::priority_queue<kd::Cell, std::vector<kd::Cell>, decltype(grid.cell_cmp)> q(grid.cell_cmp);
	std::map<kd::Cell, kd::pair<kd::Cell, Action>> parent;

	parent[this->m_pos] = kd::pair<kd::Cell, Action>{
			kd::Cell{-1, -1},
      Action::NO_OP
  };
	q.push(this->m_pos);
	// TODO: just use the first goal for now
	const auto goal = grid.m_goals[0];

	grid[this->m_pos] = grid.dist(this->m_pos, goal);

	kd::Cell cur;
	for (cur = q.top(); !q.empty(); cur = q.top()) {
		q.pop();

		if (cur == goal)
			break;

		for (const auto &c : kd::CellAdjs) {
			const auto ncell = cur + c.fst;
			if (!grid.cell_valid(ncell))
				continue;
			if (grid[ncell] == BlockState::BLOCK)
				continue;
			if (grid[ncell] != BlockState::EMPTY && grid[ncell] <= grid[cur] + 1)
				continue;
			grid[ncell] = grid[cur] + grid.dist(ncell, goal) - grid.dist(cur, goal) + 1;
			q.push(ncell);
			++this->m_nnodes;
			parent[ncell] = kd::pair<kd::Cell, Action>{cur, c.sec};
		}
	}

	if (cur != goal)
		return 1;
	// Trace back the route
	for (; cur != this->m_pos; this->m_path.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
	return 0;
}
