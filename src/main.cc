#include <stdio.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "SDL.h"
#include "fmt/core.h"

#include "agent.hh"
#include "e_action.hh"
#include "e_methods.hh"
#include "errors.hh"
#include "grid.hh"
#include "isearch.hh"
#include "search.hh"
#include "search/dfs.hh"
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

	if (argc < 3) {
		fmt::print(stderr, "Not enough arguments. "
		                   "Please use: ./search <FILEPATH> <METHOD>\n");
		return 1;
	}

	const char *inp_file_name     = argv[1];
	const std::string_view method = argv[2];

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

	grid.print();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
		throw std::runtime_error(fmt::format("SDL_Init Error: {}\n", SDL_GetError()));

	SDL_Color block_colour    = {216, 222, 233, 255};
	SDL_Color visited_colour  = {129, 161, 193, 255};
	SDL_Color start_colour    = {163, 190, 140, 255};
	SDL_Color goal_colour     = {235, 203, 139, 255};
	SDL_Color path_colour     = {94, 129, 172, 255};
	SDL_Color grid_background = {22, 22, 22, 255};
	SDL_Color grid_line_color = {44, 44, 44, 255};

	const int max_win_width  = 1280;
	const int max_win_height = 720;

	const int ratio = std::min(max_win_height / n, max_win_width / m);

	SDL_Rect cell_rect = {0, 0, ratio, ratio};
	SDL_Rect ver_line  = {0, 0, 2, ratio * (int)n};
	SDL_Rect hor_line  = {0, 0, ratio * (int)m, 2};

	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> win(
			SDL_CreateWindow("Search Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                     ratio * m, ratio * n, SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);

	if (!win)
		throw std::runtime_error(fmt::format("Create window and renderer: {}", SDL_GetError()));

	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> ren(
			SDL_CreateRenderer(win.get(), 0, 0), SDL_DestroyRenderer);

	int traced = 0;
	for (int quit = 0, step = 0, done = 0; !quit;) {
		for (SDL_Event e; SDL_PollEvent(&e);) {
			switch (e.type) {
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_SPACE:
					if (!done)
						done = search->step();
					else {
						if (!traced && !search->reached_goal()) {
							quit = 1;
						}
						if (!traced)
							search->trace_path();
						traced = 1;
					}
					break;
				case SDLK_q:
					quit = 1;
					break;
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		// Draw grid background.
		SDL_SetRenderDrawColor(ren.get(), grid_background.r, grid_background.g, grid_background.b,
		                       grid_background.a);
		SDL_RenderClear(ren.get());

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (kd::Cell{i, j} == grid.goals()[0]) {
					SDL_SetRenderDrawColor(ren.get(), goal_colour.r, goal_colour.g, goal_colour.b,
					                       goal_colour.a);
					cell_rect.x = ratio * j;
					cell_rect.y = ratio * i;
					SDL_RenderFillRect(ren.get(), &cell_rect);
					continue;
				}
				if (kd::Cell{i, j} == a.pos()) {
					SDL_SetRenderDrawColor(ren.get(), start_colour.r, start_colour.g, start_colour.b,
					                       start_colour.a);
					cell_rect.x = ratio * j;
					cell_rect.y = ratio * i;
					SDL_RenderFillRect(ren.get(), &cell_rect);
					continue;
				}
				switch (grid.at(i, j)) {
				case BlockState::EMPTY:
					continue;
				case BlockState::BLOCK:
					SDL_SetRenderDrawColor(ren.get(), block_colour.r, block_colour.g, block_colour.b,
					                       block_colour.a);
					cell_rect.x = ratio * j;
					cell_rect.y = ratio * i;
					SDL_RenderFillRect(ren.get(), &cell_rect);
					break;
				default:
					SDL_SetRenderDrawColor(ren.get(), visited_colour.r, visited_colour.g, visited_colour.b,
					                       visited_colour.a);
					cell_rect.x = ratio * j;
					cell_rect.y = ratio * i;
					SDL_RenderFillRect(ren.get(), &cell_rect);
					break;
				}
			}

			if (search->path().size() > 0) {
				kd::Cell cur_cell = kd::Cell{a.pos().fst, a.pos().sec};
				// skip the goal and the start
				for (std::size_t i = search->path().size(); i-- > 1;) {
					cur_cell += kd::CellFromDirection[search->path()[i]];
					SDL_SetRenderDrawColor(ren.get(), path_colour.r, path_colour.g, path_colour.b,
					                       path_colour.a);
					cell_rect.x = ratio * cur_cell.sec;
					cell_rect.y = ratio * cur_cell.fst;
					SDL_RenderFillRect(ren.get(), &cell_rect);
				}
			}

			SDL_SetRenderDrawColor(ren.get(), grid_line_color.r, grid_line_color.g, grid_line_color.b,
			                       grid_line_color.a);
			for (ver_line.x = 0; ver_line.x < 1 + m * ratio; ver_line.x += ratio)
				SDL_RenderFillRect(ren.get(), &ver_line);

			for (hor_line.y = 0; hor_line.y < 1 + n * ratio; hor_line.y += ratio)
				SDL_RenderFillRect(ren.get(), &hor_line);
		}

		SDL_RenderPresent(ren.get());
	}

	grid.print();

	if (!traced) {
		fmt::print(stderr, "No solution found.");
		return 1;
	}

	fmt::println("{} {} {}", inp_file_name, method, search->nnodes());
	search->print_path();

	SDL_Quit();
}
