#ifndef GRID_HH
#define GRID_HH

#include <stdio.h>

#include <algorithm>
#include <array>
#include <vector>

#include "e_block.hh"
#include "utils.hh"

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

	~Grid() {
		// free vector
		m_grid.clear();
		m_grid.shrink_to_fit();
	}
};

#endif // !GRID_HH
