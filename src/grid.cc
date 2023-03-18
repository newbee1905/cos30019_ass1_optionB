#include "grid.hh"
#include "fmt/ostream.h"

void operator+=(Cell &lhs, const Cell &rhs) {
  lhs.fst += rhs.fst;
  lhs.sec += rhs.sec;
}

Cell operator+(const Cell &lhs, const Cell &rhs) {
  Cell res = Cell{lhs.fst, lhs.sec};
  res += rhs;
  return res;
}

inline void Grid::insert_block(const Cell &p) {
  m_grid[p.fst * p.sec + p.sec] = BlockState::BLOCK;
}
inline void Grid::insert_block(const int &x, const int &y) {
  m_grid[x * width() + y] = BlockState::BLOCK;
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

void Grid::insert_goal(const Cell &p) { m_goals.push_back(p); }

void Grid::insert_goal(const int &x, const int &y) {
  m_goals.emplace_back(Cell{x, y});
}

inline BlockState &Grid::at(const Cell &p) {
  return m_grid[p.fst * width() + p.sec];
}
BlockState &Grid::at(const int &x, const int &y) {
  return m_grid[x * width() + y];
}
BlockState &Grid::operator[](const Cell &p) { return at(p); }
