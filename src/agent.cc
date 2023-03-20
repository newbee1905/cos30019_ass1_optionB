#include "agent.hh"

void Agent::search(Methods method, Grid &grid, std::vector<Action> &res) {
	switch (method) {
	case Methods::DFS:
		return dfs(grid, res);
	case Methods::BFS:
		return bfs(grid, res);
	case Methods::GBFS:
		return gbfs(grid, res);
	case Methods::AS:
		return astar(grid, res);
	default:
		fmt::print(stderr, "This method is not implemented yet or not existed");
		return;
	}
}
