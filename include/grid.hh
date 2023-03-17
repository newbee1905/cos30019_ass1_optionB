#ifndef GRID_HH
#define GRID_HH

#include <stdio.h>

#include <algorithm>
#include <array>
#include <vector>

#include "e_block.hh"
#include "utils.hh"

typedef kd::pair<int, int> Cell;

class Grid {
private:
  // 2D array in form of a 1D array
  std::vector<BlockState> m_grid;

public:
  // TODO: Use priority queue
  std::vector<kd::pair<int, int>> goals;
  std::size_t n, m;

  Grid(int n, int m) : n(n), m(m) {
    // count from 1
    m_grid.reserve(n * m);
    std::fill(m_grid.begin(), m_grid.end(), BlockState::EMPTY);
  }

  inline void insert_block(const kd::pair<int, int> &p);
  inline void insert_block(const int &x, const int &y);

  void insert_block_area(const kd::pair<int, int> &p, const int &w,
                         const int &h);
  void insert_block_area(const int &x, const int &y, const int &w,
                         const int &h);

  void insert_goal(const kd::pair<int, int> &p);
  void insert_goal(const int &x, const int &y);

  inline BlockState &at(const kd::pair<int, int> &p);
  BlockState &at(const int &x, const int &y);
  BlockState &operator[](const kd::pair<int, int> &p);

  ~Grid() {
    // free vector
    m_grid.clear();
    m_grid.resize(0);
  }
};

#endif // !GRID_HH
