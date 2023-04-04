#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "fmt/ostream.h"
#include "grid.hh"

#include <algorithm>
#include <map>
#include <queue>
#include <vector>

int kd::Agent::hill(kd::Grid &grid) {
	grid[this->m_pos] = BlockState::VISIT;

	// TODO: just use the first goal for now
	const auto goal = grid.m_goals[0];
	kd::pair<kd::Cell, Action> cur, next, tmp;
	for (cur.fst = m_pos; cur.fst != goal; ) {
		tmp = cur;

		for (const auto &c : kd::CellAdjs) {
			next = c;
			next.fst += cur.fst;

			if (!grid.cell_valid(next.fst))
				continue;

			if (grid[next.fst] == BlockState::BLOCK)
				continue;

			fmt::println("{}, {}", next.fst.fst, next.fst.sec);

			if (grid.dist(next.fst, goal) <= grid.dist(tmp.fst, goal)) {
				tmp = next;
				fmt::println("Next: {}, {}", tmp.fst.fst, tmp.fst.sec);
			}
		}

		if (cur.fst == tmp.fst)
			break;
		cur = tmp;
		grid[cur.fst] = BlockState::VISIT;
		this->m_path.emplace_back(cur.sec);
	}

	if (cur.fst != goal)
		return 1;

	return 0;
}
