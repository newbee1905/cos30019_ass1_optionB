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

void kd::Agent::astar(kd::Grid &grid, std::vector<Action> &res) {
	auto cell_cmp = [&](const kd::Cell &a, const kd::Cell &b) -> bool { return grid[a] > grid[b]; };
	auto dist     = [&](const kd::Cell &a, const kd::Cell &b) -> int {
    return std::abs(a.fst - b.fst) + std::abs(a.sec - b.sec);
	};
	auto dist_cmp = [&](const kd::Cell &a, const kd::Cell &b, const kd::Cell &goal) -> bool {
		return dist(a, goal) < dist(b, goal);
	};

	std::priority_queue<kd::Cell, std::vector<kd::Cell>, decltype(cell_cmp)> q(cell_cmp);
	std::map<kd::Cell, kd::pair<kd::Cell, Action>> parent;

	parent[this->m_pos] = kd::pair<kd::Cell, Action>{
			kd::Cell{-1, -1},
      Action::NO_OP
  };
	q.push(this->m_pos);
	// TODO: just use the first goal for now
	const auto goal = grid.m_goals[0];

	grid[this->m_pos] = dist(this->m_pos, goal);

	kd::Cell cur;
	for (cur = q.top(); !q.empty(); cur = q.top()) {
		q.pop();

		if (cur == goal)
			break;

		for (const auto &c : kd::CellAdjs) {
			const auto ncell = cur + c.fst;
			if (ncell.fst < 0 || ncell.sec < 0 || ncell.fst >= grid.height() || ncell.sec >= grid.width())
				continue;
			// BLOCK or VISITED
			if (grid[ncell] >= BlockState::BLOCK)
				continue;
			grid[ncell] = grid[cur] + dist_cmp(cur, ncell, goal) + 1;
			q.push(ncell);
			parent[ncell] = kd::pair<kd::Cell, Action>{cur, c.sec};
		}
	}

	if (cur != goal)
		return;
	// Trace back the route
	for (; cur != this->m_pos; res.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
}
