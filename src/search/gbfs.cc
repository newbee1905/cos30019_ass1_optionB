#include "search/gbfs.hh"

int kd::GBFS::step() {
	if (m_frontier.empty())
		return 1;
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
		m_grid[ncell] = m_grid.dist(ncell, m_goal) + BlockState::VISIT;
		m_frontier.push(ncell);
		++m_nnodes;
		m_parent[ncell] = kd::pair<kd::Cell, Action>{m_cur, c.sec};
	}
	return 0;
}

int kd::GBFS::run() {
	while (step() == 0)
		;
	if (m_cur != m_goal)
		return 1;
	for (; m_cur != m_agent.pos();
	     m_path.emplace_back(m_parent[m_cur].sec), m_cur = m_parent[m_cur].fst)
		;
	return 0;
}

const int &kd::GBFS::nnodes() { return m_nnodes; }
const std::vector<Action> &kd::GBFS::path() { return m_path; }

void kd::GBFS::print_path() {
	for (std::size_t i = m_path.size(); i-- > 0; fmt::print("{}; ", this->m_path[i]))
		;
}
