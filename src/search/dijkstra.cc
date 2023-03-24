#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "fmt/ostream.h"
#include "grid.hh"

#include <algorithm>
#include <map>
#include <queue>
#include <vector>

void kd::Agent::dijkstra(kd::Grid &grid, std::vector<Action> &res) {
	std::priority_queue<kd::Cell, std::vector<kd::Cell>, decltype(grid.cell_cmp)> q(grid.cell_cmp);

	std::map<kd::Cell, kd::pair<kd::Cell, Action>> parent;

	parent[this->m_pos] = kd::pair<kd::Cell, Action>{
			kd::Cell{-1, -1},
      Action::NO_OP
  };
	q.push(this->m_pos);
	grid[this->m_pos] = BlockState::VISIT;

	// TODO: just use the first goal for now
	const auto goal = grid.m_goals[0];
	kd::Cell cur;
	for (cur = q.top(); !q.empty(); cur = q.top(), ++this->m_nnodes) {
		q.pop();

		if (cur == goal)
			break;

		for (const auto &c : kd::CellAdjs) {
			const auto ncell = cur + c.fst;
			if (ncell.fst < 0 || ncell.sec < 0 || ncell.fst >= grid.height() || ncell.sec >= grid.width())
				continue;
			if (grid[ncell] == BlockState::BLOCK ||
			    (grid[ncell] >= BlockState::VISIT && grid[ncell] < grid[cur] + 1))
				continue;
			grid[ncell] = grid[cur] + 1;
			q.push(ncell);
			parent[ncell] = kd::pair<kd::Cell, Action>{cur, c.sec};
		}
	}

	if (grid[goal] == BlockState::EMPTY)
		return;
	cur = goal;
	// Trace back the route
	for (; cur != this->m_pos; res.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
}
