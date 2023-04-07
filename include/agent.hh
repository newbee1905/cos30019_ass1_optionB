#ifndef AGENT_HH
#define AGENT_HH

#include "e_action.hh"
#include "e_methods.hh"
#include "fmt/ostream.h"
#include "grid.hh"
#include "isearch.hh"
#include "utils.hh"

#include <algorithm>
#include <array>
#include <exception>
#include <map>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace kd {
class Agent {
private:
	kd::pair<int, int> m_pos;
	// number of nodes checked
	// while searching
	std::vector<Action> m_path;

public:
	Agent(const kd::Cell &pos) : m_pos(pos) {}

	Agent(const kd::Cell &&pos) : m_pos(std::move(pos)) {}
	~Agent() {}

	const kd::Cell &pos();
	void set_pos(const kd::Cell &pos);
	void set_pos(const kd::Cell &&pos);
};
}; // namespace kd

#endif // !AGENT_HH
