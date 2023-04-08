#ifndef SEARCH_IDASTAR_HH
#define SEARCH_IDASTAR_HH

#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "grid.hh"
#include "isearch.hh"

#include <algorithm>
#include <queue>
#include <stack>

namespace kd {
class IDASTAR : public ISearch {
private:
	std::map<kd::Cell, kd::pair<kd::Cell, Action>> m_parent;
	std::vector<Action> m_path;
	kd::Agent &m_agent;
	kd::Grid &m_grid;
	kd::Cell m_goal, m_cur;
	int m_threshold, _min_threshold;
	int m_nnodes;
	/* std::function<bool(const Cell &, const Cell &)> cell_cmp = */
	/* 		[this](const kd::Cell &a, const kd::Cell &b) -> bool { return m_grid[a] > m_grid[b]; }; */
	/* std::priority_queue<kd::Cell, std::vector<kd::Cell>, decltype(cell_cmp)> m_frontier; */
	std::stack<kd::Cell> m_frontier;

public:
	IDASTAR(kd::Agent &agent, kd::Grid &grid) : m_agent(agent), m_grid(grid), m_nnodes(1) {
		/* m_frontier = std::priority_queue<kd::Cell, std::vector<kd::Cell>,
		 * decltype(cell_cmp)>(cell_cmp); */
		m_cur           = m_agent.pos();
		m_parent[m_cur] = kd::pair<kd::Cell, Action>{
				kd::Cell{-1, -1},
        Action::NO_OP
    };
		m_goal = m_grid.goals()[0];
		m_frontier.push(m_cur);
		m_grid[m_cur]  = BlockState::VISIT;
		m_threshold    = m_grid.dist(m_agent.pos(), m_goal) + BlockState::VISIT;
		_min_threshold = 0;
	}

	int step();
	int reset();
	int run();
	const int &nnodes();
	const std::vector<Action> &path();
	int reached_goal();
	void trace_path();
	void print_path();
};
} // namespace kd

#endif // !SEARCH_IDASTAR_HH
