#include "agent.hh"
#include "doctest/doctest.h"
#include "e_action.hh"
#include "e_block.hh"
#include "e_methods.hh"
#include "fmt/core.h"
#include "grid.hh"
#include <algorithm>
#include <tuple>

constexpr int height                = 5;
constexpr int width                 = 11;
constexpr Cell start                = Cell{1, 0};
constexpr std::array<Cell, 2> goals = {
		Cell{0,  7},
    Cell{3, 10}
};
constexpr std::array<std::tuple<Cell, int, int>, 7> blocks = {
		std::tuple<Cell, int, int>{ Cell{0, 2}, 2, 2},
      std::tuple<Cell, int, int>{ Cell{0, 8}, 1, 2},
		std::tuple<Cell, int, int>{Cell{0, 10}, 1, 1},
      std::tuple<Cell, int, int>{ Cell{3, 2}, 1, 2},
		std::tuple<Cell, int, int>{ Cell{4, 3}, 3, 1},
      std::tuple<Cell, int, int>{ Cell{3, 9}, 1, 1},
		std::tuple<Cell, int, int>{ Cell{4, 8}, 2, 1},
};

#define SEARCH_TEST_INIT()                                                                         \
	Grid grid(height, width);                                                                        \
	Agent a;                                                                                         \
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
	Cell s = a.pos();                                                                                \
	CHECK(s == start);                                                                               \
	for (std::size_t i = res.size(); i-- > 0;) {                                                     \
		auto action = res[i];                                                                          \
		s += CellFromDirection[action];                                                                \
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
