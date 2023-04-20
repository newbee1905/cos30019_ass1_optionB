#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "e_methods.hh"
#include "fmt/core.h"
#include "grid.hh"
#include "isearch.hh"
#include "search.hh"
#include <algorithm>
#include <tuple>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

constexpr int height     = 200;
constexpr int width      = 200;
constexpr kd::Cell start = kd::Cell{1, 0};

kd::Grid grid_test(height, width, 1);
bool __init_grid__ = false;

kd::Grid init_grid() {
	if (!__init_grid__) {
		grid_test.gen(start);
		__init_grid__ = true;
	}
	return grid_test;
}

// TODO: add argument STATE to check for no solution maze
#define SEARCH_TEST_INIT(METHOD)                                                                   \
	kd::Grid grid = init_grid();                                                                     \
	kd::Agent a(start);                                                                              \
	CHECK(a.pos() == start);                                                                         \
                                                                                                   \
	auto t_start = std::chrono::high_resolution_clock::now();                                        \
	auto search  = kd::get_search(METHOD, a, grid);                                                  \
	CHECK(search->run() == 0);                                                                       \
	auto t_end = std::chrono::high_resolution_clock::now();                                          \
	MESSAGE("Time: ", std::chrono::duration<double, std::milli>(t_end - t_start).count(), "ms")

#define SEARCH_TEST_CONFIRM()                                                                      \
	kd::Cell s = a.pos();                                                                            \
	CHECK(s == start);                                                                               \
	for (std::size_t i = search->path().size(); i-- > 0;) {                                          \
		auto action = search->path()[i];                                                               \
		s += kd::CellFromDirection[action];                                                            \
		if (grid[s] == BlockState::EMPTY)                                                              \
			FAIL(fmt::format("The cell should not be empty after searched: ({}, {})", s.fst, s.sec));    \
	}                                                                                                \
                                                                                                   \
	auto check = std::find(grid.goals().begin(), grid.goals().end(), s);                             \
	CHECK(check != grid.goals().end())

TEST_CASE("DFS") {
	SEARCH_TEST_INIT("DFS");
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("BFS") {
	SEARCH_TEST_INIT("BFS");
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("GBFS") {
	SEARCH_TEST_INIT("GBFS");
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("ASTAR") {
	SEARCH_TEST_INIT("AS");
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("DIJKSTRA") {
	SEARCH_TEST_INIT("CUS1");
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("IDASTAR") {
	SEARCH_TEST_INIT("CUS2");
	SEARCH_TEST_CONFIRM();
}
