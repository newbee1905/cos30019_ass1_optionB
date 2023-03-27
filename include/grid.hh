#ifndef GRID_HH
#define GRID_HH

#include <stdio.h>

#include <algorithm>
#include <array>
#include <vector>

#include "e_block.hh"
#include "utils.hh"

namespace kd {
typedef kd::pair<int, int> Cell;

void operator+=(Cell &lhs, const Cell &rhs);
Cell operator+(const Cell &lhs, const Cell &rhs);

class Grid {
private:
	// 2D array in form of a 1D array
	// TODO: Think of a think way to still able to use BlockState
	std::vector<int> m_grid;
	std::size_t m_height, m_width;

public:
	// TODO: Use priority queue
	std::vector<kd::pair<int, int>> m_goals;

	Grid(int n, int m) : m_height(n), m_width(m) {
		// count from 1
		m_grid.resize(n * m);
		std::fill(m_grid.begin(), m_grid.end(), BlockState::EMPTY);
	}

	~Grid() {
		// free vector
		m_grid.clear();
		m_grid.shrink_to_fit();
	}

	inline int height() const { return m_height; }
	inline int width() const { return m_width; }

	inline void insert_block(const kd::pair<int, int> &p);
	inline void insert_block(const int &x, const int &y);

	void insert_block_area(const kd::pair<int, int> &p, const int &w, const int &h);
	void insert_block_area(const int &x, const int &y, const int &w, const int &h);

	void insert_goal(const kd::pair<int, int> &p);
	void insert_goal(const int &x, const int &y);

	inline int &at(const kd::pair<int, int> &p);
	int &at(const int &x, const int &y);
	int &operator[](const kd::pair<int, int> &p);

	void print();

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
