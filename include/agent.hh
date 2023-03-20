#ifndef AGENT_HH
#define AGENT_HH

#include "e_action.hh"
#include "e_methods.hh"
#include "fmt/ostream.h"
#include "grid.hh"
#include "utils.hh"
#include <array>
#include <vector>

namespace kd {
class Agent {
private:
	kd::pair<int, int> m_pos;
	// number of nodes checked
	// while searching
	int m_nnodes = 1;

public:
	Agent() {}
	~Agent() {}

	void dfs(kd::Grid &grid, std::vector<Action> &res);
	void bfs(kd::Grid &grid, std::vector<Action> &res);
	void gbfs(kd::Grid &grid, std::vector<Action> &res);
	void astar(kd::Grid &grid, std::vector<Action> &res);

	const int &nnodes() { return m_nnodes; }

	const kd::Cell &pos() { return m_pos; }
	void set_pos(const kd::Cell &pos) { m_pos = pos; }
	void set_pos(const kd::Cell &&pos) { m_pos = std::move(pos); }

	void search(Methods method, kd::Grid &grid, std::vector<Action> &res);
};
}; // namespace kd

#endif // !AGENT_HH
