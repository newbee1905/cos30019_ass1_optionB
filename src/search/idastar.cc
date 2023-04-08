#include "search/idastar.hh"

int kd::IDASTAR::reset() {
	m_grid.clear();
	m_grid[m_agent.pos()] = m_grid.dist(m_agent.pos(), m_goal) + BlockState::VISIT;

	m_frontier.push(m_agent.pos());

	if (_min_threshold <= m_threshold)
		return 1;

	m_threshold    = _min_threshold;
	_min_threshold = 0;
	return 0;
}

int kd::IDASTAR::step() {
	if (m_frontier.empty()) {
		if (m_cur != m_goal)
			return reset();
		return 1;
	}
	m_cur = m_frontier.top();
	m_frontier.pop();

	if (m_cur == m_goal)
		return 1;

	for (const auto &c : kd::CellAdjs) {
		const auto ncell = m_cur + c.fst;
		if (!m_grid.cell_valid(ncell))
			continue;
		if (m_grid[ncell] >= BlockState::BLOCK)
			continue;
		const int estimate = m_grid[m_cur] + m_grid.dist(ncell, m_goal) + 1;
		if (m_threshold < estimate) {
			if (!_min_threshold || _min_threshold > estimate)
				_min_threshold = estimate;
			continue;
		}
		if (m_grid[ncell] == BlockState::EMPTY) {
			m_grid[ncell] = m_grid[m_cur] + 1;
			if (m_parent.find(ncell) == m_parent.end())
				++m_nnodes;
			m_parent[ncell] = kd::pair<kd::Cell, Action>{m_cur, c.sec};
		}
		m_frontier.push(ncell);
	}
	return 0;
}

int kd::IDASTAR::run() {
	while (step() == 0)
		;
	if (m_cur != m_goal)
		return 1;
	for (; m_cur != m_agent.pos();
	     m_path.emplace_back(m_parent[m_cur].sec), m_cur = m_parent[m_cur].fst)
		;
	return 0;
}

const int &kd::IDASTAR::nnodes() { return m_nnodes; }
const std::vector<Action> &kd::IDASTAR::path() { return m_path; }

int kd::IDASTAR::reached_goal() { return m_cur == m_goal; }

void kd::IDASTAR::trace_path() {
	for (; m_cur != m_agent.pos();
	     m_path.emplace_back(m_parent[m_cur].sec), m_cur = m_parent[m_cur].fst)
		;
}

void kd::IDASTAR::print_path() {
	for (std::size_t i = m_path.size(); i-- > 0; fmt::print("{}; ", this->m_path[i]))
		;
}
