#include "agent.hh"
#include "doctest/doctest.h"
#include "e_action.hh"
#include "e_block.hh"
#include "e_methods.hh"
#include "fmt/core.h"
#include "grid.hh"
#include <algorithm>
#include <tuple>

constexpr int height                    = 5;
constexpr int width                     = 11;
constexpr kd::Cell start                = kd::Cell{1, 0};
constexpr std::array<kd::Cell, 2> goals = {
		kd::Cell{0,  7},
    kd::Cell{3, 10}
};
constexpr std::array<std::tuple<kd::Cell, int, int>, 7> blocks = {
		std::tuple<kd::Cell, int, int>{ kd::Cell{0, 2}, 2, 2},
		std::tuple<kd::Cell, int, int>{ kd::Cell{0, 8}, 1, 2},
		std::tuple<kd::Cell, int, int>{kd::Cell{0, 10}, 1, 1},
		std::tuple<kd::Cell, int, int>{ kd::Cell{3, 2}, 1, 2},
		std::tuple<kd::Cell, int, int>{ kd::Cell{4, 3}, 3, 1},
		std::tuple<kd::Cell, int, int>{ kd::Cell{3, 9}, 1, 1},
		std::tuple<kd::Cell, int, int>{ kd::Cell{4, 8}, 2, 1},
};

#define SEARCH_TEST_INIT()                                                                         \
	kd::Grid grid(height, width);                                                                    \
	kd::Agent a;                                                                                     \
	a.set_pos(start);                                                                                \
	CHECK(a.pos() == start);                                                                         \
                                                                                                   \
	for (const auto &g : goals)                                                                      \
		grid.insert_goal(g);                                                                           \
                                                                                                   \
	for (const auto &b : blocks)                                                                     \
		grid.insert_block_area(std::get<0>(b), std::get<1>(b), std::get<2>(b));                        \
                                                                                                   \
	std::vector<Action> res

#define SEARCH_TEST_CONFIRM()                                                                      \
	kd::Cell s = a.pos();                                                                            \
	CHECK(s == start);                                                                               \
	for (std::size_t i = res.size(); i-- > 0;) {                                                     \
		auto action = res[i];                                                                          \
		s += kd::CellFromDirection[action];                                                            \
		if (grid[s] == BlockState::EMPTY)                                                              \
			FAIL(fmt::format("The cell should not be empty after searched: ({}, {})", s.fst, s.sec));    \
	}                                                                                                \
                                                                                                   \
	auto check = std::find(goals.begin(), goals.end(), s);                                           \
	CHECK(check != goals.end())

TEST_CASE("DFS") {
	SEARCH_TEST_INIT();
	a.search(Methods::DFS, grid, res);
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("BFS") {
	SEARCH_TEST_INIT();
	a.search(Methods::BFS, grid, res);
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("GBFS") {
	SEARCH_TEST_INIT();
	a.search(Methods::GBFS, grid, res);
	SEARCH_TEST_CONFIRM();
}

TEST_CASE("ASTAR") {
	SEARCH_TEST_INIT();
	a.search(Methods::AS, grid, res);
	SEARCH_TEST_CONFIRM();
}
