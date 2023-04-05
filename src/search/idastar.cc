#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "grid.hh"

#include <algorithm>
#include <fmt/ostream.h>

#include <array>
#include <cmath>
#include <iterator>
#include <map>
#include <stack>
#include <vector>

int kd::Agent::idastar(kd::Grid &grid) {
	kd::Cell cur;
	std::map<kd::Cell, kd::pair<kd::Cell, Action>> parent;

	// TODO: just use the first goal for now
	const auto goal   = grid.m_goals[0];
	grid[this->m_pos] = BlockState::VISIT;

	int threshold     = grid.dist(this->m_pos, goal);
	int min_threshold = 0;

	for (; cur != goal; threshold = std::max(min_threshold + 1, threshold), min_threshold = 0) {
		std::stack<kd::Cell> s;

		parent.clear();
		grid.clear();

		grid[this->m_pos]   = BlockState::VISIT;
		parent[this->m_pos] = kd::pair<kd::Cell, Action>{
				kd::Cell{-1, -1},
        Action::NO_OP
    };
		s.push(this->m_pos);

		cur = s.top();
		for (; !s.empty(); cur = s.top()) {
			s.pop();

			if (cur == goal)
				break;

			for (const auto &c : kd::DfsCellAdjs) {
				const auto ncell = cur + c.fst;
				if (!grid.cell_valid(ncell))
					continue;
				if (grid[ncell] >= BlockState::BLOCK)
					continue;
				const int estimate = grid[cur] + 1 + grid.dist(ncell, goal);
				grid[ncell]        = grid[cur] + 1;
				if (threshold < estimate) {
					if (!min_threshold || min_threshold > estimate)
						min_threshold = estimate;
					continue;
				}
				s.push(ncell);
				++this->m_nnodes;
				parent[ncell] = kd::pair<kd::Cell, Action>{cur, c.sec};
			}
		}
	}

	if (cur != goal)
		return 1;
	// Trace back the route
	for (; cur != this->m_pos; this->m_path.emplace_back(parent[cur].sec), cur = parent[cur].fst)
		;
	return 0;
}
