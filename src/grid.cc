#include "grid.hh"
#include <fmt/ostream.h>
#include <functional>

void kd::operator+=(kd::Cell &lhs, const kd::Cell &rhs) {
	lhs.fst += rhs.fst;
	lhs.sec += rhs.sec;
}

kd::Cell kd::operator+(const kd::Cell &lhs, const kd::Cell &rhs) {
	kd::Cell res = kd::Cell{lhs.fst, lhs.sec};
	res += rhs;
	return res;
}

inline void kd::Grid::insert_block(const Cell &p) {
	m_grid[p.fst * p.sec + p.sec] = BlockState::BLOCK;
}
inline void kd::Grid::insert_block(const int &x, const int &y) {
	m_grid[x * width() + y] = BlockState::BLOCK;
}

void kd::Grid::insert_block_area(const Cell &p, const int &w, const int &h) {
	// TODO: may use flood fill to make the process of inserting faster
	// May not actually faster since have to recreate pair
	for (int i{}; i < h; ++i)
		for (int j{}; j < w; ++j)
			insert_block(p.fst + i, p.sec + j);
}

void kd::Grid::insert_block_area(const int &x, const int &y, const int &w, const int &h) {
	// TODO: may use flood fill to make the process of inserting faster
	// May not actually faster since have to recreate pair
	for (int i{}; i < h; ++i)
		for (int j{}; j < w; ++j)
			insert_block(x + i, y + j);
}

void kd::Grid::insert_goal(const Cell &p) { m_goals.push_back(p); }

void kd::Grid::insert_goal(const int &x, const int &y) { m_goals.emplace_back(Cell{x, y}); }

inline int &kd::Grid::at(const Cell &p) { return m_grid[p.fst * width() + p.sec]; }
int &kd::Grid::at(const int &x, const int &y) { return m_grid[x * width() + y]; }
int &kd::Grid::operator[](const Cell &p) { return at(p); }

void kd::Grid::print() {
	for (int i{}; i < m_height; ++i) {
		for (int j{}; j < m_width; ++j)
			fmt::print("{}, ", this->at(i, j));
		fmt::println("");
	}
	fmt::println("");
}

// void kd::Grid::gui_update() {
// 	if (!this->m_gui)
// 		return;
//
// 	bool quit = false;
// 	for (SDL_Event *e; !quit;) {
// 		/* while (SDL_PollEvent(e)) { */
// 		/* 	switch (e->type) { */
// 		/* 	case SDL_KEYDOWN: */
// 		/* 	case SDL_MOUSEBUTTONDOWN: */
// 		/* 	case SDL_QUIT: */
// 		/* 		quit = true; */
// 		/* 		break; */
// 		/* 	} */
// 		/* } */
// 	}
// }
