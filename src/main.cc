#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "SDL.h"
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

#if defined(__WIN32__)
signed wmain(int argc, char **argv) { main(argc, argv); }
#endif

signed main(int argc, char **argv) {
	if (argc < 3) {
		fmt::print(stderr, "Not enough arguments. "
		                   "Please use: ./search <FILEPATH> <METHOD>\n");
		return 1;
	}

	const char *inp_file_name     = argv[1];
	const std::string_view method = argv[2];

	auto inp_file = fopen(inp_file_name, "rb");

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
	kd::Agent a(pos, method);

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

	grid.print();

	if (a.search(grid) != 0) {
		fmt::print(stderr, "No solution found.");
		return 1;
	}

	grid.print();

	fmt::println("{} {} {}", inp_file_name, method, a.nnodes());
	a.print_path();

	/* if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) { */
	/* 	fmt::print(stderr, "SDL_Init Error: {}\n", SDL_GetError()); */
	/* 	return 1; */
	/* } */
	/**/
	/* SDL_Window *win = */
	/* 		SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480,
	 * 0); */
	/* if (!win) { */
	/* 	fmt::print(stderr, "SDL_CreateWindow Error: {}\n", SDL_GetError()); */
	/* 	return 1; */
	/* } */
	/**/
	/* SDL_Surface *sur = SDL_GetWindowSurface(win); */
	/* if (!sur) { */
	/* 	fmt::print(stderr, "SDL_GetWindowSurface Error: {}\n", SDL_GetError()); */
	/* 	return 1; */
	/* } */
	/**/
	/* bool quit = false; */
	/* for (SDL_Event e; !quit;) { */
	/* 	SDL_UpdateWindowSurface(win); */
	/* 	while (SDL_PollEvent(&e)) { */
	/* 		switch (e.type) { */
	/* 		case SDL_KEYDOWN: */
	/* 		case SDL_MOUSEBUTTONDOWN: */
	/* 		case SDL_QUIT: */
	/* 			quit = true; */
	/* 			break; */
	/* 		} */
	/* 	} */
	/* } */
	/**/
	/* SDL_FreeSurface(sur); */
	/* SDL_DestroyWindow(win); */
	/* SDL_Quit(); */
}
