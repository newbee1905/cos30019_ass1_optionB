#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "SDL.h"
#include "fmt/chrono.h"
#include "fmt/core.h"

#include "agent.hh"
#include "e_action.hh"
#include "e_methods.hh"
#include "errors.hh"
#include "grid.hh"
#include "gui.hh"
#include "isearch.hh"
#include "search.hh"
#include "utils.hh"

// turn of warning of not using fopen_s function
// on windows
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#ifdef __linux__
#define CHAR(TMP)                            (char *)&TMP
#define FOPEN(INP_FILE, INP_FILE_NAME, MODE) INP_FILE = fopen(INP_FILE_NAME, MODE)
#elif _WIN32
#define CHAR(tmp)                            (char *)&tmp, 1
#define FOPEN(INP_FILE, INP_FILE_NAME, MODE) fopen_s(&INP_FILE, INP_FILE_NAME, MODE)
#endif

// Source:
// https://stackoverflow.com/questions/64625905/my-app-wont-run-unless-i-declare-main-as-wmain-why-is-that-visual-studio-19
// not using SDL_main
// using wmain cause errors for fscanf due to lack of unicode support
// use fwscanf instead
#if defined(_WIN32)
#undef main
// #define fscanf fwscanf
// signed wmain(int argc, char **argv) { main(argc, argv); }
#endif

signed main(int argc, char **argv) {
	if (argc < 3 && argc > 4) {
		fmt::print(stderr, "Not enough arguments. "
		                   "Please use: search <FILEPATH> <METHOD>\n");
		return 1;
	}

	++argv; // skip the command name

	bool is_gui = argc == 4;
	if (is_gui)
		++argv;

	const char *inp_file_name = *(argv++);
	std::string method        = *(argv++);
	std::transform(method.begin(), method.end(), method.begin(), ::toupper);

	FILE *inp_file;
	FOPEN(inp_file, inp_file_name, "rb");

	assert_line(inp_file, "Failed to open input file.");

	std::size_t n, m;
#ifdef __linux__
	assert_line(fscanf(inp_file, "[%ld, %ld]\n", &n, &m), "Getting the grid's size");
#elif _WIN32
	// size_t in windows is long long int
	assert_line(fscanf(inp_file, "[%lld, %lld]\n", &n, &m), "Getting the grid's size");
#endif
	kd::Grid grid(n, m);

	kd::Cell pos;
	assert_line(fscanf(inp_file, "(%d,%d)\n", &pos.sec, &pos.fst), "Getting the starting point");
	kd::Agent a(pos);

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

	auto search = kd::get_search(method, a, grid);
	if (!search) {
		return 1;
	}

	/* grid.print(); */

	int traced = 0;

	if (is_gui)
		traced = gui(grid, a, search);
	else
		traced = -search->run();

	/* grid.print(); */

	if (traced == -1) {
		fmt::print(stderr, "No solution found.");
		return 1;
	}

	fmt::println("{} {} {}", inp_file_name, method, search->nnodes());
	search->print_path();

	/* fmt::print("\n{}ms", elapsed_time); */

	SDL_Quit();
}
