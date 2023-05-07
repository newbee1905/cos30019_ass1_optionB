#include "gui.hh"

int gui(kd::Grid &grid, kd::Agent &a, std::shared_ptr<kd::ISearch> search) {
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

	const int ratio = std::min(max_win_height / grid.height(), max_win_width / grid.width());

	SDL_Rect cell_rect = {0, 0, ratio, ratio};
	SDL_Rect ver_line  = {0, 0, 2, ratio * (int)grid.height()};
	SDL_Rect hor_line  = {0, 0, ratio * (int)grid.width(), 2};

	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> win(
			SDL_CreateWindow("Search Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                     ratio * grid.width(), ratio * grid.height(), SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);

	if (!win)
		throw std::runtime_error(fmt::format("Create window and renderer: {}", SDL_GetError()));

	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> ren(
			SDL_CreateRenderer(win.get(), 0, 0), SDL_DestroyRenderer);

	auto t_start        = std::chrono::high_resolution_clock::now();
	auto t_end          = std::chrono::high_resolution_clock::now();
	double elapsed_time = 0;

	int traced = 0;
	for (int quit = 0, done = 0; !quit;) {
		for (SDL_Event e; SDL_PollEvent(&e);) {
			switch (e.type) {
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_SPACE:
					if (!done) {
						t_start = std::chrono::high_resolution_clock::now();
						done    = search->step();
						t_end   = std::chrono::high_resolution_clock::now();
						elapsed_time += std::chrono::duration<double, std::milli>(t_end - t_start).count();
					} else {
						if (traced == 0 && !search->reached_goal()) {
							traced = -1;
							quit   = 1;
						}
						if (traced == 0) {
							search->trace_path();
							traced = 1;
						} else {
							quit = 1;
						}
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

		for (int i = 0; i < grid.height(); ++i) {
			for (int j = 0; j < grid.width(); ++j) {
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
			for (ver_line.x = 0; ver_line.x < 1 + grid.width() * ratio; ver_line.x += ratio)
				SDL_RenderFillRect(ren.get(), &ver_line);

			for (hor_line.y = 0; hor_line.y < 1 + grid.height() * ratio; hor_line.y += ratio)
				SDL_RenderFillRect(ren.get(), &hor_line);
		}

		SDL_RenderPresent(ren.get());
	}

	return traced;
}
