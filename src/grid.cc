#include "grid.hh"
#include "e_action.hh"
#include "e_block.hh"
#include <fmt/ostream.h>
#include <functional>
#include <random>
#include <stack>

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
	m_grid[x * m_width + y] = BlockState::BLOCK;
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

void kd::Grid::insert_goal(const Cell &p) { m_goals.emplace_back(p); }

void kd::Grid::insert_goal(const int &x, const int &y) { m_goals.emplace_back(Cell{x, y}); }

int &kd::Grid::at(const Cell &p) { return m_grid[p.fst * m_width + p.sec]; }
int &kd::Grid::at(const int &x, const int &y) { return m_grid[x * m_width + y]; }
int &kd::Grid::operator[](const Cell &p) { return at(p); }

void kd::Grid::print() {
	for (int i{}; i < m_height; ++i) {
		for (int j{}; j < m_width; ++j)
			fmt::print("{}, ", this->at(i, j));
		fmt::println("");
	}
	fmt::println("");
}

void kd::Grid::clear() {
	for (int i{}; i < m_height; ++i)
		for (int j{}; j < m_width; ++j)
			if (this->at(i, j) >= BlockState::VISIT)
				this->at(i, j) = BlockState::EMPTY;
}

void kd::Grid::gen(const int &x, const int &y) { gen(Cell{x, y}); }
void kd::Grid::gen(const Cell &start) {
	std::stack<kd::Cell> s;
	Cell cur;
	Cell max_dist = start;
	s.push(start);
	auto adjs = kd::CellAdjs;
	while (!s.empty()) {
		std::shuffle(std::begin(adjs), std::end(adjs), std::mt19937{std::random_device{}()});
		cur           = s.top();
		this->at(cur) = BlockState::EMPTY;
		if (this->dist(cur, start) > this->dist(max_dist, start))
			max_dist = cur;
		s.pop();
		for (int i = 0; i < 2; ++i) {
			const auto c      = adjs[i];
			const auto ncell  = cur + c.fst;
			const auto nncell = ncell + c.fst;
			if (!cell_valid(ncell))
				continue;
			if (this->at(ncell) == BlockState::EMPTY)
				continue;
			if (!cell_valid(nncell))
				goto set_path;
			if (this->at(nncell) == BlockState::EMPTY)
				continue;
		set_path:
			s.push(ncell);
		}
	}
	insert_goal(max_dist);
}

void kd::Grid::write_gen(const int &x, const int &y) {
	fmt::println("[{},{}]\r", m_height, m_width);
	fmt::println("({},{})\r", y, x);
	for (int i = 0; i < m_goals.size(); ++i) {
		const auto g = m_goals[i];
		fmt::print("({},{})", g.sec, g.fst);
		if (i > 0)
			fmt::print(" | ");
	}
	fmt::println("\r");
	for (int i = 0; i < m_height; ++i)
		for (int j = 0; j < m_height; ++j)
			if (this->at(i, j) == BlockState::BLOCK)
				fmt::println("({},{},1,1)\r", j, i);
}
void kd::Grid::write_gen(const Cell &start) { write_gen(start.fst, start.sec); }
