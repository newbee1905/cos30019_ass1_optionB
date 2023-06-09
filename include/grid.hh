#ifndef GRID_HH
#define GRID_HH

#include <stdexcept>
#include <stdio.h>

#include <algorithm>
#include <iterator>
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

public:
	Grid(int n, int m, int gen = 0) : m_height(n), m_width(m) {
		// count from 1
		m_grid.resize(n * m);
		// if genrating mode on -> fill grid with block
		std::fill(m_grid.begin(), m_grid.end(), gen);
	}

	~Grid() {
		// free vector
		m_grid.clear();
		m_grid.shrink_to_fit();

		m_goals.clear();
		m_goals.shrink_to_fit();
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

	// get starting point to generate maze
	void gen(const int &x, const int &y);
	void gen(const Cell &start);

	// write out grid to test files
	void write_gen(const int &x, const int &y);
	void write_gen(const Cell &start);

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
