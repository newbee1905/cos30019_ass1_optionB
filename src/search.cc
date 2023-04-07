#include "search.hh"
#include "agent.hh"
#include "search/astar.hh"
#include "search/bfs.hh"
#include "search/dfs.hh"
#include "search/dijkstra.hh"
#include "search/gbfs.hh"
#include "search/idastar.hh"

kd::ISearch *kd::get_search(std::string_view method, std::shared_ptr<kd::Agent> &agent,
                            std::shared_ptr<kd::Grid> &grid) {
	switch (GetEnumMethods(method)) {
	case Methods::DFS:
		return new kd::DFS(agent, grid);
	case Methods::BFS:
		return new kd::BFS(agent, grid);
	case Methods::GBFS:
		return new kd::GBFS(agent, grid);
	case Methods::AS:
		return new kd::ASTAR(agent, grid);
	case Methods::CUS1:
		return new kd::Dijkstra(agent, grid);
	case Methods::CUS2:
		return new kd::IDASTAR(agent, grid);
	default:
		fmt::print(stderr, "This method is not implemented yet or not existed");
		return nullptr;
	}
}
