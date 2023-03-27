#include "agent.hh"
#include "e_action.hh"
#include "fmt/ostream.h"

const int &kd::Agent::nnodes() { return m_nnodes; }

const std::vector<Action> &kd::Agent::path() const { return this->m_path; }

const kd::Cell &kd::Agent::pos() { return m_pos; }
void kd::Agent::set_pos(const kd::Cell &pos) { m_pos = pos; }
void kd::Agent::set_pos(const kd::Cell &&pos) { m_pos = std::move(pos); }

int kd::Agent::search(Grid &grid) {
	switch (this->m_method) {
	case Methods::DFS:
		return dfs(grid);
	case Methods::BFS:
		return bfs(grid);
	case Methods::GBFS:
		return gbfs(grid);
	case Methods::AS:
		return astar(grid);
	case Methods::CUS1:
		return dijkstra(grid);
	default:
		fmt::print(stderr, "This method is not implemented yet or not existed");
		return 1;
	}
}

void kd::Agent::print_path() {
	switch (this->m_method) {
	default:
		for (std::size_t i = this->m_path.size(); i-- > 0; fmt::print("{}; ", this->m_path[i]))
			;
	}
}
