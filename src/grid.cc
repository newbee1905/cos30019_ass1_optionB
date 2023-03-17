#include "grid.hh"
#include "fmt/ostream.h"

inline void Grid::insert_block(const Cell &p) {
  m_grid[p.fst * p.sec + p.sec] = BlockState::BLOCK;
}
inline void Grid::insert_block(const int &x, const int &y) {
  m_grid[x * m + y] = BlockState::BLOCK;
}

void Grid::insert_block_area(const Cell &p, const int &w, const int &h) {
  // TODO: may use flood fill to make the process of inserting faster
  // May not actually faster since have to recreate pair
  for (int i{}; i < h; ++i)
    for (int j{}; j < w; ++j)
      insert_block(p.fst + i, p.sec + j);
}

void Grid::insert_block_area(const int &x, const int &y, const int &w,
                             const int &h) {
  // TODO: may use flood fill to make the process of inserting faster
  // May not actually faster since have to recreate pair
  for (int i{}; i < h; ++i)
    for (int j{}; j < w; ++j)
      insert_block(x + i, y + j);
}

void Grid::insert_goal(const Cell &p) { goals.push_back(p); }

void Grid::insert_goal(const int &x, const int &y) {
  goals.push_back(Cell{x, y});
}

inline BlockState &Grid::at(const Cell &p) { return m_grid[p.fst * m + p.sec]; }
BlockState &Grid::at(const int &x, const int &y) { return m_grid[x * m + y]; }
BlockState &Grid::operator[](const Cell &p) { return at(p); }
