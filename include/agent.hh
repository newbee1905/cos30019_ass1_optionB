#ifndef AGENT_HH
#define AGENT_HH

#include "e_methods.hh"
#include "grid.hh"
#include "utils.hh"
#include <array>

class Agent {
private:
  constexpr std::array<std::function<void(Agent &a, Grid &g)>,
                       GetEnumMethodsLength()>
      m_methods;

public:
  kd::pair<int, int> pos;
  Agent() {}
  ~Agent() {}

  constexpr void search(Methods method) {}
};

#endif // !AGENT_HH
