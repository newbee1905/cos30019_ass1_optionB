#ifndef GRID_HH
#define GRID_HH

#include <stdexcept>
#include <stdio.h>

#include <algorithm>
#include <array>
#include <vector>

// #include <SDL.h>

#include "e_block.hh"
#include "utils.hh"

namespace kd {
typedef kd::pair<int, int> Cell;

void operator+=(Cell &lhs, const Cell &rhs);
Cell operator+(const Cell &lhs, const Cell &rhs);

// TODO: move the visualizer outside of Grid
// since the grid is also being used in the test
// which does not need GUI
class Grid {
private:
	// 2D array in form of a 1D array
	// TODO: Think of a think way to still able to use BlockState
	std::vector<int> m_grid;
	std::size_t m_height, m_width;
	// TODO: Use priority queue
	std::vector<kd::pair<int, int>> m_goals;

	// SDL_Window *m_win      = nullptr;
	// SDL_Renderer *m_ren    = nullptr;
	// const int m_win_height = 680;
	// const int m_win_width  = 480;

public:
	Grid(int n, int m) : m_height(n), m_width(m) {
		// count from 1
		m_grid.resize(n * m);
		std::fill(m_grid.begin(), m_grid.end(), BlockState::EMPTY);

		// TODO: change gui mode with command line flags
		// if (!m_gui)
		// 	return;

		// if (SDL_Init(SDL_INIT_VIDEO) != 0)
		// 	throw std::runtime_error(fmt::format("SDL_Init Error: {}\n", SDL_GetError()));

		// if (SDL_CreateWindowAndRenderer(m_win_width, m_win_height, 0, &m_win, &m_ren) != 0)
		// 	throw std::runtime_error(fmt::format("Create window and renderer: {}", SDL_GetError()));

		// /* SDL_SetWindowTitle(m_win, "Search Visualizer"); */
	}

	// TODO: uodate the visualizer
	// - Maybe using separate thread so that it would not make the search too slow
	// - If implement it in the search function -> require sleep so that the window would not close
	// too soon
	// - Maybe a cache system with queue?
	// 	- A seperate storage for visualizer??? -> have functions for the user to go back or forward
	void gui_update();

	~Grid() {
		// free vector
		m_grid.clear();
		m_grid.shrink_to_fit();

		m_goals.clear();
		m_goals.shrink_to_fit();

		// if (!m_gui)
		// 	return;

		// SDL_DestroyRenderer(m_ren);
		// SDL_DestroyWindow(m_win);
		// SDL_Quit();
	}

	inline int height() const { return m_height; }
	inline int width() const { return m_width; }

	const std::vector<kd::pair<int, int>> &goals() { return m_goals; };

	inline void insert_block(const kd::pair<int, int> &p);
	inline void insert_block(const int &x, const int &y);

	void insert_block_area(const kd::pair<int, int> &p, const int &w, const int &h);
	void insert_block_area(const int &x, const int &y, const int &w, const int &h);

	void insert_goal(const kd::pair<int, int> &p);
	void insert_goal(const int &x, const int &y);

	int &at(const kd::pair<int, int> &p);
	int &at(const int &x, const int &y);
	int &operator[](const kd::pair<int, int> &p);

	void print();
	void clear();

	std::function<bool(const Cell &, const Cell &)> cell_cmp = [this](const kd::Cell &a,
	                                                                  const kd::Cell &b) -> bool {
		return this->at(a.fst, a.sec) > this->at(b.fst, b.sec);
	};

	std::function<bool(const Cell &)> cell_valid = [this](const kd::Cell &a) -> bool {
		return a.fst >= 0 && a.sec >= 0 && a.fst < m_height && a.sec < m_width;
	};

	static int dist(const kd::Cell &a, const kd::Cell &b) {
		return std::abs(a.fst - b.fst) + std::abs(a.sec - b.sec);
	}
};
}; // namespace kd

#endif // !GRID_HH
