#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "agent.hh"
#include "e_action.hh"
#include "e_methods.hh"
#include "errors.hh"
#include "fmt/core.h"
#include "grid.hh"
#include "utils.hh"

// turn of warning of not using fopen_s function
// on windows
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#ifdef __linux__
#define CHAR(tmp) (char *)&tmp
#elif _WIN32
#define fscanf    fscanf_s
#define CHAR(tmp) (char *)&tmp, 1
#endif

signed main(int argc, char **argv) {
	if (argc < 3) {
		fmt::print(stderr, "Not enough arguments. "
		                   "Please use: ./search <FILEPATH> <METHOD>\n");
		return 1;
	}

	const char *inp_file_name     = argv[1];
	const std::string_view method = argv[2];

	fmt::print("You select method {}\n", GetEnumMethods(method));
	auto inp_file = fopen(inp_file_name, "rb");

	assert_line(inp_file, "Failed to open input file.");

	std::size_t n, m;
#ifdef __linux__
	fscanf(inp_file, "[%ld, %ld]\n", &n, &m);
#elif _WIN32
	// size_t in windows is long long int
	fscanf(inp_file, "[%lld, %lld]\n", &n, &m);
#endif
	Grid grid(n, m);

	Agent a;
	Cell pos;
	fscanf(inp_file, "(%d,%d)\n", &pos.sec, &pos.fst);
	a.set_pos(pos);

	// use tmp to scanf after ')'
	// to force to stop at the line for getting
	// location of goals
	for (int x{}, y{}, tmp{}; fscanf(inp_file, "(%d, %d)%c| ", &y, &x, CHAR(tmp));
	     grid.insert_goal(x, y))
		;

	for (int x{}, y{}, w{}, h{};
	     fscanf(inp_file, "\n(%d, %d, %d, %d)", &y, &x, &w, &h) && !feof(inp_file);
	     grid.insert_block_area(x, y, w, h))
		;

	fclose(inp_file);

	for (int i{}; i < n; ++i) {
		for (int j{}; j < m; ++j)
			fmt::print("{}, ", grid.at(i, j));
		fmt::println("");
	}
	fmt::println("");

	std::vector<Action> res;

	a.search(GetEnumMethods(method), grid, res);

	// Print the route
	if (res.empty()) {
		fmt::print(stderr, "No solution found.");
		return 1;
	}

	for (int i{}; i < n; ++i) {
		for (int j{}; j < m; ++j)
			fmt::print("{}, ", int(grid.at(i, j)));
		fmt::println("");
	}
	fmt::println("");

	for (std::size_t i = res.size(); i-- > 0; fmt::print("{}; ", res[i]))
		;
}
