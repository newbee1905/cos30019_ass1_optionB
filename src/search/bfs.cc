#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "fmt/ostream.h"
#include "grid.hh"

#include <algorithm>
#include <map>
#include <queue>
#include <vector>

int kd::Agent::bfs(kd::Grid &grid) {
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
	for (cur = q.front(); !q.empty(); cur = q.front(), ++this->m_nnodes) {
		q.pop();

		if (cur == goal)
			break;

		for (const auto &c : kd::CellAdjs) {
			const auto ncell = cur + c.fst;
			if (!grid.cell_valid(ncell))
				continue;
			if (grid[ncell] >= BlockState::BLOCK)
				continue;
			grid[ncell] = BlockState::VISIT;
			q.push(ncell);
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
