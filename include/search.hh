#ifndef SEARCH_HH
#define SEARCH_HH

#include "agent.hh"
#include "e_methods.hh"
#include "grid.hh"
#include "isearch.hh"
#include <memory>

namespace kd {
std::shared_ptr<ISearch> get_search(std::string_view method, kd::Agent &agent, kd::Grid &grid);
}; // namespace kd

#endif // !SEARCH_HH
