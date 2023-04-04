#ifndef AGENT_HH
#define AGENT_HH

#include "e_action.hh"
#include "e_methods.hh"
#include "fmt/ostream.h"
#include "grid.hh"
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
	int m_nnodes = 1;
	std::vector<Action> m_path;
	Methods m_method;

public:
	Agent(const kd::Cell &pos, const std::string_view m) : m_pos(pos) {
		try {
			m_method = GetEnumMethods(m);
		} catch (const std::range_error &e) {
			throw std::runtime_error("This method is not implemented yet or not existed");
		}
	}
	Agent(const kd::Cell &&pos, const std::string_view m) : m_pos(std::move(pos)) {
		try {
			m_method = GetEnumMethods(m);
		} catch (const std::range_error &e) {
			throw std::runtime_error("This method is not implemented yet or not existed");
		}
	}
	~Agent() {}

	int dfs(kd::Grid &grid);
	int bfs(kd::Grid &grid);
	int gbfs(kd::Grid &grid);
	int astar(kd::Grid &grid);
	int dijkstra(kd::Grid &grid);
	int idastar(kd::Grid &grid);

	const int &nnodes();

	const std::vector<Action> &path() const;

	const kd::Cell &pos();
	void set_pos(const kd::Cell &pos);
	void set_pos(const kd::Cell &&pos);

	[[nodiscard]] int search(kd::Grid &grid);
	void print_path();
};
}; // namespace kd

#endif // !AGENT_HH
