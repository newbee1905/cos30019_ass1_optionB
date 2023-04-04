#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "grid.hh"

#include <fmt/ostream.h>

#include <algorithm>
#include <map>
#include <queue>
#include <vector>

int kd::Agent::dijkstra(kd::Grid &grid) {
	/* std::priority_queue<kd::Cell, std::vector<kd::Cell>, decltype(grid.cell_cmp)> q(grid.cell_cmp);
	 */
	std::queue<kd::Cell> q;

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
	for (cur = q.front(); !q.empty(); cur = q.front()) {
		q.pop();

		if (cur == goal)
			break;

		for (const auto &c : kd::CellAdjs) {
			const auto ncell = cur + c.fst;
			if (!grid.cell_valid(ncell))
				continue;
			// odd number is either block or visited
			if (grid[ncell] & 1)
				continue;
			// if not empty and already smaller
			if (grid[ncell] != BlockState::EMPTY && grid[ncell] <= grid[cur] + 2)
				continue;
			grid[ncell] = grid[cur] + 2;
			q.push(ncell);
			++this->m_nnodes;
			parent[ncell] = kd::pair<kd::Cell, Action>{cur, c.sec};
		}

		// set visited
		++grid[cur];
	}

	if (grid[goal] == BlockState::EMPTY)
		return 1;
	cur = goal;
	// Trace back the route
	for (; cur != this->m_pos; this->m_path.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
	return 0;
}
