#ifndef SEARCH_HH
#define SEARCH_HH

#include "agent.hh"
#include "e_methods.hh"
#include "grid.hh"
#include "isearch.hh"
#include <memory>

namespace kd {
ISearch *get_search(std::string_view method, std::shared_ptr<kd::Agent> &agent,
                    std::shared_ptr<kd::Grid> &grid);
}; // namespace kd

#endif // !SEARCH_HH
