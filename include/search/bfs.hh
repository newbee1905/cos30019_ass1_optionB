#ifndef SEARCH_BFS_HH
#define SEARCH_BFS_HH

#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "grid.hh"
#include "isearch.hh"

#include <queue>

namespace kd {
class BFS : public ISearch {
private:
	std::queue<kd::Cell> m_frontier;
	std::map<kd::Cell, kd::pair<kd::Cell, Action>> m_parent;
	std::vector<Action> m_path;
	kd::Agent &m_agent;
	kd::Grid &m_grid;
	kd::Cell m_goal, m_cur;
	int m_nnodes;

public:
	BFS(kd::Agent &agent, kd::Grid &grid) : m_agent(agent), m_grid(grid), m_nnodes(1) {
		m_cur           = m_agent.pos();
		m_parent[m_cur] = kd::pair<kd::Cell, Action>{
				kd::Cell{-1, -1},
        Action::NO_OP
    };
		m_goal = m_grid.goals()[0];
		m_frontier.push(m_cur);
		m_grid[m_cur] = BlockState::VISIT;
	}

	int step();
	int run();
	const int &nnodes();
	const std::vector<Action> &path();
	void print_path();
};
} // namespace kd

#endif // !SEARCH_BFS_HH
