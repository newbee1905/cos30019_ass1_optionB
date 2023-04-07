#include "search.hh"
#include "agent.hh"
#include "search/astar.hh"
#include "search/bfs.hh"
#include "search/dfs.hh"
#include "search/dijkstra.hh"
#include "search/gbfs.hh"
#include "search/idastar.hh"
#include <algorithm>
#include <memory>

std::shared_ptr<kd::ISearch> kd::get_search(std::string_view method, kd::Agent &agent,
                                            kd::Grid &grid) {
	switch (GetEnumMethods(method)) {
	case Methods::DFS:
		return std::make_shared<kd::DFS>(agent, grid);
	case Methods::BFS:
		return std::make_shared<kd::BFS>(agent, grid);
	case Methods::GBFS:
		return std::make_shared<kd::GBFS>(agent, grid);
	case Methods::AS:
		return std::make_shared<kd::ASTAR>(agent, grid);
	case Methods::CUS1:
		return std::make_shared<kd::Dijkstra>(agent, grid);
	case Methods::CUS2:
		return std::make_shared<kd::IDASTAR>(agent, grid);
	default:
		fmt::print(stderr, "This method is not implemented yet or not existed");
		return nullptr;
	}
}
