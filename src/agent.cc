#include "agent.hh"

const int &kd::Agent::nnodes() { return m_nnodes; }

const kd::Cell &kd::Agent::pos() { return m_pos; }
void kd::Agent::set_pos(const kd::Cell &pos) { m_pos = pos; }
void kd::Agent::set_pos(const kd::Cell &&pos) { m_pos = std::move(pos); }

void kd::Agent::search(Methods method, Grid &grid, std::vector<Action> &res) {
	switch (method) {
	case Methods::DFS:
		return dfs(grid, res);
	case Methods::BFS:
		return bfs(grid, res);
	case Methods::GBFS:
		return gbfs(grid, res);
	case Methods::AS:
		return astar(grid, res);
	case Methods::CUS1:
		return dijkstra(grid, res);
	default:
		fmt::print(stderr, "This method is not implemented yet or not existed");
		return;
	}
}
